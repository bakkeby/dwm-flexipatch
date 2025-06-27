static int cursor_hidden = 0;
/* mouse_x and mouse_y are used to store the actual co-ordinates of the mouse cursor when
 * hidden. These can be manipulated freely, e.g. when using the warp patch, to set a new
 * cursor position for when the cursor is to be revealed again. */
static int mouse_x = 0;
static int mouse_y = 0;
static int xi_opcode;
static unsigned long long last_button_press = 0;

void
genericevent(XEvent *e)
{
	if (e->xcookie.extension != xi_opcode)
		return;

	if (!XGetEventData(dpy, &e->xcookie))
		return;

	switch (e->xcookie.evtype) {
	case XI_RawMotion:
		if (cursor_hidden)
			showcursor(NULL);
		break;
	case XI_RawTouchBegin:
	case XI_RawTouchEnd:
	case XI_RawTouchUpdate:
		if (!cursor_hidden)
			hidecursor(NULL);
		break;
	case XI_RawKeyRelease:
		if (now() - last_button_press > 2000 && !cursor_hidden) {
			hidecursor(NULL);
		}
		break;
	}

	XFreeEventData(dpy, &e->xcookie);
}

void
hidecursor(const Arg *arg)
{
	if (cursor_hidden)
		return;

	XFixesHideCursor(dpy, root);
	if (getrootptr(&mouse_x, &mouse_y)) {
		XWarpPointer(dpy, None, root, 0, 0, 0, 0, selmon->mx + selmon->mw, selmon->my);
	}

	cursor_hidden = 1;
}

unsigned long long
now(void) {
	struct timespec currentTime;
	clock_gettime(CLOCK_REALTIME, &currentTime);
	return currentTime.tv_sec * 1000LL + currentTime.tv_nsec / 1000000LL;
}

Client *
recttoclient(int x, int y, int w, int h, int include_floating)
{
	Client *c, *r = NULL;
	int a, area = 1;

	for (c = selmon->stack; c; c = c->snext) {
		if (!ISVISIBLE(c) || (c->isfloating && !include_floating))
			continue;
		if ((a = INTERSECTC(x, y, w, h, c)) >= area) {
			area = a;
			r = c;
		}
	}
	return r;
}

void
showcursor(const Arg *arg)
{
	if (!cursor_hidden)
		return;

	XWarpPointer(dpy, None, root, 0, 0, 0, 0, mouse_x, mouse_y);
	XFixesShowCursor(dpy, root);

	cursor_hidden = 0;
}
