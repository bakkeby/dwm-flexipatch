void
managealtbar(Window win, XWindowAttributes *wa)
{
	Monitor *m;
	Bar *bar;
	int i = 0;
	if (!(m = recttomon(wa->x, wa->y, wa->width, wa->height)))
		return;
	for (bar = m->bar; bar && bar->win && bar->next; bar = bar->next); // find last bar
	if (!bar) {
		bar = m->bar = ecalloc(1, sizeof(Bar));
		bar->topbar = topbar;
	} else if (bar && bar->win) {
		i = bar->idx + 1;
		bar->next = ecalloc(1, sizeof(Bar));
		#if BAR_ANYBAR_TOP_AND_BOTTOM_BARS_PATCH
		bar->next->topbar = !bar->topbar;
		#else
		bar->next->topbar = topbar;
		#endif // BAR_ANYBAR_TOP_AND_BOTTOM_BARS_PATCH
		bar = bar->next;
	}
	bar->external = 1;
	bar->showbar = 1;
	bar->mon = m;
	bar->idx = i;
	bar->borderpx = 0;
	bar->win = win;
	bar->bw = wa->width;
	bar->bh = wa->height;
	updatebarpos(m);
	arrange(m);
	XSelectInput(dpy, win, EnterWindowMask|FocusChangeMask|PropertyChangeMask|StructureNotifyMask);
	XMapWindow(dpy, win);
	XMoveResizeWindow(dpy, bar->win, bar->bx, bar->by, bar->bw, bar->bh);
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
	Bar *bar, *next, *prev = NULL;

	if (!m)
		return;

	for (bar = m->bar; bar && bar->win; bar = next) {
		next = bar->next;
		if (bar->win == w) {
			if (prev)
				prev->next = next;
			else
				m->bar = next;
			free(bar);
			break;
		}
		prev = bar;
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

