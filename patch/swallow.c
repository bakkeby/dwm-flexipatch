#include <X11/Xlib-xcb.h>
#include <xcb/res.h>
#ifdef __OpenBSD__
#include <sys/sysctl.h>
#include <kvm.h>
#endif /* __OpenBSD__ */

static int scanner;
static xcb_connection_t *xcon;

int
swallow(Client *p, Client *c)
{
	Client *s;
	XWindowChanges wc;
	#if NOBORDER_PATCH
	int border_padding = 0;
	#endif // NOBORDER_PATCH

	if (c->noswallow > 0 || c->isterminal)
		return 0;
	if (c->noswallow < 0 && !swallowfloating && c->isfloating)
		return 0;

	XMapWindow(dpy, c->win);

	detach(c);
	detachstack(c);

	setclientstate(c, WithdrawnState);
	XUnmapWindow(dpy, p->win);

	p->swallowing = c;
	c->mon = p->mon;

	Window w = p->win;
	p->win = c->win;
	c->win = w;

	XChangeProperty(dpy, c->win, netatom[NetClientList], XA_WINDOW, 32, PropModeReplace,
		(unsigned char *) &(p->win), 1);

	#if BAR_WINICON_PATCH
	updateicon(p);
	#endif
	updatetitle(p);
	s = scanner ? c : p;
	#if BAR_EWMHTAGS_PATCH
	setfloatinghint(s);
	#endif // BAR_EWMHTAGS_PATCH

	#if NOBORDER_PATCH
	wc.border_width = p->bw;
	if (noborder(p)) {
		wc.border_width = 0;
		border_padding = p->bw * 2;
	}

	XConfigureWindow(dpy, p->win, CWBorderWidth, &wc);
	XMoveResizeWindow(dpy, p->win, s->x, s->y, s->w + border_padding, s->h + border_padding);
	#else
	wc.border_width = p->bw;
	XConfigureWindow(dpy, p->win, CWBorderWidth, &wc);
	XMoveResizeWindow(dpy, p->win, s->x, s->y, s->w, s->h);
	#endif // NOBORDER_PATCH

	#if !BAR_FLEXWINTITLE_PATCH
	XSetWindowBorder(dpy, p->win, scheme[SchemeNorm][ColBorder].pixel);
	#endif // BAR_FLEXWINTITLE_PATCH

	arrange(p->mon);
	configure(p);
	updateclientlist();

	return 1;
}

void
unswallow(Client *c)
{
	XWindowChanges wc;
	c->win = c->swallowing->win;
	#if NOBORDER_PATCH
	int border_padding = 0;
	#endif // NOBORDER_PATCH

	free(c->swallowing);
	c->swallowing = NULL;

	XDeleteProperty(dpy, c->win, netatom[NetClientList]);

	/* unfullscreen the client */
	setfullscreen(c, 0);
	#if BAR_WINICON_PATCH
	updateicon(c);
	#endif
	updatetitle(c);
	arrange(c->mon);
	XMapWindow(dpy, c->win);

	#if NOBORDER_PATCH
	wc.border_width = c->bw;
	if (noborder(c)) {
		wc.border_width = 0;
		border_padding = c->bw * 2;
	}

	XConfigureWindow(dpy, c->win, CWBorderWidth, &wc);
	XMoveResizeWindow(dpy, c->win, c->x, c->y, c->w + border_padding, c->h + border_padding);
	#else
	wc.border_width = c->bw;
	XConfigureWindow(dpy, c->win, CWBorderWidth, &wc);
	XMoveResizeWindow(dpy, c->win, c->x, c->y, c->w, c->h);
	#endif // NOBORDER_PATCH
	#if !BAR_FLEXWINTITLE_PATCH
	XSetWindowBorder(dpy, c->win, scheme[SchemeNorm][ColBorder].pixel);
	#endif // BAR_FLEXWINTITLE_PATCH

	#if BAR_EWMHTAGS_PATCH
	setfloatinghint(c);
	#endif // BAR_EWMHTAGS_PATCH
	setclientstate(c, NormalState);
	arrange(c->mon);
	focus(NULL);
}

pid_t
winpid(Window w)
{
	pid_t result = 0;

	#ifdef __linux__
	xcb_res_client_id_spec_t spec = {0};
	spec.client = w;
	spec.mask = XCB_RES_CLIENT_ID_MASK_LOCAL_CLIENT_PID;

	xcb_generic_error_t *e = NULL;
	xcb_res_query_client_ids_cookie_t c = xcb_res_query_client_ids(xcon, 1, &spec);
	xcb_res_query_client_ids_reply_t *r = xcb_res_query_client_ids_reply(xcon, c, &e);

	if (!r)
		return (pid_t)0;

	xcb_res_client_id_value_iterator_t i = xcb_res_query_client_ids_ids_iterator(r);
	for (; i.rem; xcb_res_client_id_value_next(&i)) {
		spec = i.data->spec;
		if (spec.mask & XCB_RES_CLIENT_ID_MASK_LOCAL_CLIENT_PID) {
			uint32_t *t = xcb_res_client_id_value_value(i.data);
			result = *t;
			break;
		}
	}

	free(r);

	if (result == (pid_t)-1)
		result = 0;

	#endif /* __linux__ */
	#ifdef __OpenBSD__
	Atom type;
	int format;
	unsigned long len, bytes;
	unsigned char *prop;
	pid_t ret;

	if (XGetWindowProperty(dpy, w, XInternAtom(dpy, "_NET_WM_PID", 1), 0, 1, False, AnyPropertyType, &type, &format, &len, &bytes, &prop) != Success || !prop)
		return 0;

	ret = *(pid_t*)prop;
	XFree(prop);
	result = ret;
	#endif /* __OpenBSD__ */

	return result;
}

pid_t
getparentprocess(pid_t p)
{
	unsigned int v = 0;

#ifdef __linux__
	FILE *f;
	char buf[256];
	snprintf(buf, sizeof(buf) - 1, "/proc/%u/stat", (unsigned)p);

	if (!(f = fopen(buf, "r")))
		return (pid_t)0;

	if (fscanf(f, "%*u %*s %*c %u", (unsigned *)&v) != 1)
		v = (pid_t)0;
	fclose(f);
#endif /* __linux__ */
#ifdef __OpenBSD__
	int n;
	kvm_t *kd;
	struct kinfo_proc *kp;

	kd = kvm_openfiles(NULL, NULL, NULL, KVM_NO_FILES, NULL);
	if (!kd)
		return 0;

	kp = kvm_getprocs(kd, KERN_PROC_PID, p, sizeof(*kp), &n);
	v = kp->p_ppid;
#endif /* __OpenBSD__ */
	return (pid_t)v;
}

int
isdescprocess(pid_t p, pid_t c)
{
	while (p != c && c != 0)
		c = getparentprocess(c);

	return (int)c;
}

Client *
termforwin(const Client *w)
{
	Client *c;
	Monitor *m;

	if (!w->pid || w->isterminal)
		return NULL;

	c = selmon->sel;
	if (c && c->isterminal && !c->swallowing && c->pid && isdescprocess(c->pid, w->pid))
		return c;

	for (m = mons; m; m = m->next) {
		for (c = m->clients; c; c = c->next) {
			if (c->isterminal && !c->swallowing && c->pid && isdescprocess(c->pid, w->pid))
				return c;
		}
	}

	return NULL;
}

Client *
swallowingclient(Window w)
{
	Client *c;
	Monitor *m;

	for (m = mons; m; m = m->next) {
		for (c = m->clients; c; c = c->next) {
			if (c->swallowing && c->swallowing->win == w)
				return c;
		}
	}

	return NULL;
}

