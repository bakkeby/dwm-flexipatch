void
setcurrentdesktop(void)
{
	long data[] = { 0 };
	XChangeProperty(dpy, root, netatom[NetCurrentDesktop], XA_CARDINAL, 32, PropModeReplace, (unsigned char *)data, 1);
}

void
setdesktopnames(void)
{
	XTextProperty text;
	Xutf8TextListToTextProperty(dpy, tags, TAGSLENGTH, XUTF8StringStyle, &text);
	XSetTextProperty(dpy, root, &text, netatom[NetDesktopNames]);
}

void
setnumdesktops(void)
{
	long data[] = { TAGSLENGTH };
	XChangeProperty(dpy, root, netatom[NetNumberOfDesktops], XA_CARDINAL, 32, PropModeReplace, (unsigned char *)data, 1);
}

void
setviewport(void)
{
	long data[] = { 0, 0 };
	XChangeProperty(dpy, root, netatom[NetDesktopViewport], XA_CARDINAL, 32, PropModeReplace, (unsigned char *)data, 2);
}

void
updatecurrentdesktop(void)
{
	long rawdata[] = { selmon->tagset[selmon->seltags] };
	int i = 0;
	while (*rawdata >> (i + 1)) {
		i++;
	}
	long data[] = { i };
	XChangeProperty(dpy, root, netatom[NetCurrentDesktop], XA_CARDINAL, 32, PropModeReplace, (unsigned char *)data, 1);
}