static XkbInfo xkbGlobal;
static XkbInfo *xkbSaved = NULL;

static XkbInfo *
createxkb(Window w)
{
	XkbInfo *xkb;

	xkb = malloc(sizeof *xkb);
	if (xkb == NULL)
		die("fatal: could not malloc() %u bytes\n", sizeof *xkb);
	xkb->group = xkbGlobal.group;
	xkb->w = w;
	xkb->next = xkbSaved;
	if (xkbSaved != NULL)
		xkbSaved->prev = xkb;
	xkb->prev = NULL;
	xkbSaved = xkb;

	return xkb;
}

XkbInfo *
findxkb(Window w)
{
	XkbInfo *xkb;
	for (xkb = xkbSaved; xkb != NULL; xkb = xkb->next)
		if (xkb->w == w)
			return xkb;
	return NULL;
}

void
xkbeventnotify(XEvent *e)
{
	XkbEvent *ev;

	ev = (XkbEvent *) e;
	switch (ev->any.xkb_type) {
	case XkbStateNotify:
		xkbGlobal.group = ev->state.locked_group;
		if (selmon != NULL && selmon->sel != NULL)
			selmon->sel->xkb->group = xkbGlobal.group;
		drawbars();
		break;
	}
}

/* xkb bar module */
int
width_xkb(Bar *bar, BarArg *a)
{
	return TEXTW(xkb_layouts[xkbGlobal.group]);
}

int
draw_xkb(Bar *bar, BarArg *a)
{
	drw_text(drw, a->x, a->y, a->w, a->h, lrpad / 2, xkb_layouts[xkbGlobal.group], 0, False);
	return 1;
}

int
click_xkb(Bar *bar, Arg *arg, BarArg *a)
{
	return ClkXKB;
}

