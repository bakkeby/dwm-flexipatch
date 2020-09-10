void
managealtbar(Window win, XWindowAttributes *wa)
{
	Monitor *m;
	if (!(m = recttomon(wa->x, wa->y, wa->width, wa->height)))
		return;

	m->bar->win = win;
	m->bar->by = wa->y;
	bh = m->bar->bh = wa->height;
	updatebarpos(m);
	arrange(m);
	XSelectInput(dpy, win, EnterWindowMask|FocusChangeMask|PropertyChangeMask|StructureNotifyMask);
	XMoveResizeWindow(dpy, win, wa->x, wa->y, wa->width, wa->height);
	XMapWindow(dpy, win);
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

    if (!m)
        return;

    m->bar->win = 0;
    m->bar->by = 0;
    m->bar->bh = 0;
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