void
managealtbar(Window win, XWindowAttributes *wa)
{
	Monitor *m;
	Bar *bar;
	int i;
	if (!(m = recttomon(wa->x, wa->y, wa->width, wa->height)))
		return;
	for (i = 0, bar = m->bar; bar && bar->win && bar->next; bar = bar->next, ++i); // find last bar
	if (!bar) {
		bar = m->bar = ecalloc(1, sizeof(Bar));
		bar->topbar = topbar;
	} else if (bar && bar->win) {
		bar->next = ecalloc(1, sizeof(Bar));
		bar->next->topbar = !bar->topbar;
		bar = bar->next;
	}
	bar->external = 1;
	bar->showbar = 1;
	bar->mon = m;
	bar->idx = i;
	bar->borderpx = 0;
	bar->win = win;
	bar->bh = wa->height;
	updatebarpos(m);
	arrange(m);
	XSelectInput(dpy, win, EnterWindowMask|FocusChangeMask|PropertyChangeMask|StructureNotifyMask);
	XMapWindow(dpy, win);
	XMoveResizeWindow(dpy, bar->win, bar->bx, -bar->by, wa->width, bar->bh);
	arrange(selmon);
	XChangeProperty(dpy, root, netatom[NetClientList], XA_WINDOW, 32, PropModeAppend,
		(unsigned char *) &win, 1);
}

void
spawnbar()
{
	if (*altbarcmd)
		system(altbarcmd);
}

void
unmanagealtbar(Window w)
{
	Monitor *m = wintomon(w);
	Bar *bar;

	if (!m)
		return;

	for (bar = m->bar; bar && bar->win; bar = bar->next)
		if (bar->win == w) {
			bar->win = 0;
			bar->by = 0;
			bar->bh = 0;
			break;
		}
	updatebarpos(m);
	arrange(m);
}

int
wmclasscontains(Window win, const char *class, const char *name)
{
	XClassHint ch = { NULL, NULL };
	int res = 1;

	if (XGetClassHint(dpy, win, &ch)) {
		if (ch.res_name && strstr(ch.res_name, name) == NULL)
			res = 0;
		if (ch.res_class && strstr(ch.res_class, class) == NULL)
			res = 0;
	} else
		res = 0;

	if (ch.res_class)
		XFree(ch.res_class);
	if (ch.res_name)
		XFree(ch.res_name);

	return res;
}