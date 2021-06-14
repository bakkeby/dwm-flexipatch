#define EXPAND_LEFT  (1 << 0)
#define EXPAND_RIGHT (1 << 2)
#define EXPAND_UP    (1 << 4)
#define EXPAND_DOWN  (1 << 6)

#define IS_SET(q, w) ((q & w) != 0)
#define IS_FORCED(q, w) IS_SET((q << 1), w)

#define EXPANDALL       (EXPAND_LEFT | EXPAND_RIGHT | EXPAND_UP | EXPAND_DOWN)
#define UNEXPAND       (EXPANDALL << 1) // Force all directions to 0
#define FORCE_EXPANDALL ~0 // Force expand in all directions

void
exresize(const Arg *arg)
{
	Client *c;
	int x, y, nx, ny, nw, nh;
	c = selmon->sel;

	if (!c || !arg) return;
	if (selmon->lt[selmon->sellt]->arrange && !c->isfloating)
		togglefloating(NULL);
	if (c->expandmask != 0)
		expand(UNEXPAND);

	x = ((int *)arg->v)[0];
	y = ((int *)arg->v)[1];

	nw = MIN(selmon->ww - c->bw*2, c->w + x);
	nh = MIN(selmon->wh - c->bw*2, c->h + y);
	nx = c->x - x/2;
	ny = c->y - y/2;

	if (!((abs(c->x + c->w/2 - (selmon->wx + selmon->ww/2)) < snap))) {
		if ((nw == selmon->ww) ||
				(nx < selmon->wx) ||
				(abs(selmon->wx - c->x) < snap))
			nx = selmon->wx;
		else if ((nx+nw > (selmon->wx + selmon->ww)) ||
				(abs((selmon->wx + selmon->ww) - (c->x + c->w)) < snap))
			nx = (selmon->wx + selmon->ww) - nw - c->bw*2;
    } else
		nx = selmon->wx + selmon->ww/2 - nw/2;

    if (!((abs(c->y + c->h/2 - (selmon->wy + selmon->wh/2)) < snap))) {

		if ((nh == selmon->wh) ||
				(ny < selmon->wy) ||
				(abs(selmon->wy - c->y) < snap))
			ny = selmon->wy;
		else if ((ny+nh > (selmon->wy + selmon->wh)) ||
				(abs((selmon->wy + selmon->wh) - (c->y + c->h)) < snap))
			ny = (selmon->wy + selmon->wh) - nh - c->bw*2;
	} else
        ny = selmon->wy + selmon->wh/2 - nh/2;


	resizeclient(c, nx, ny, MAX(nw, 32), MAX(nh, 32));
	XWarpPointer(dpy, None, c->win, 0, 0, 0, 0, c->w/2, c->h/2);
}

void
explace(const Arg *arg)
{
	Client *c;
	int nx, ny;

	c = selmon->sel;
	if (!c || (arg->ui >= 9)) return;
	if (selmon->lt[selmon->sellt]->arrange && !c->isfloating)
		togglefloating(NULL);

	nx = (arg->ui % 3) - 1;
	ny = (arg->ui / 3) - 1;

	if (nx < 0) nx = selmon->wx;
	else if (nx > 0) nx = selmon->wx + selmon->ww - c->w - c->bw*2;
	else nx = selmon->wx + selmon->ww/2 - c->w/2;

	if (ny < 0) ny = selmon->wy;
	else if (ny > 0) ny = selmon->wy + selmon->wh - c->h - c->bw*2;
	else ny = selmon->wy + selmon->wh/2 - c->h/2;

	resize(c, nx, ny, c->w, c->h, True);
	XWarpPointer(dpy, None, c->win, 0, 0, 0, 0, c->w/2, c->h/2);
}

int
calculate_expand(unsigned char mask, unsigned char curmask,
		unsigned char *newmask, unsigned char key,
		int *reset_value, int new_reset_value,
		int max_value, int old_value)
{
	if (IS_SET(key, mask) ||
			(IS_SET(key, curmask) && (!IS_SET(key, mask) && IS_FORCED(key, mask))) ||
			(!IS_SET(key, curmask) && (IS_SET(key, mask) && IS_FORCED(key, mask)))) {

		if (IS_SET(key, mask) && (!IS_SET(key,curmask) || IS_FORCED(key,mask))) {
			if (!IS_SET(key, curmask))
				*reset_value = new_reset_value;
			*newmask |= key;
			return max_value;
		} else if ((IS_SET(key,curmask) && IS_SET(key, mask)) ||
				(!IS_SET(key, mask) && IS_FORCED(key, mask))) {
			*newmask &= ~key;
			return *reset_value;
		} else {
			*newmask &= ~key;
			return old_value;
		}
	} else
		return new_reset_value;
}

void
expand(unsigned char mask)
{
	XEvent ev;
	int nx1, ny1, nx2, ny2;
	#if SETBORDERPX_PATCH
	int bp = selmon->borderpx;
	#else
	int bp = borderpx;
	#endif // SETBORDERPX_PATCH
	unsigned char curmask;
	unsigned char newmask;

	if (!selmon->sel || selmon->sel->isfixed)
		return;
	XRaiseWindow(dpy, selmon->sel->win);
	newmask = curmask = selmon->sel->expandmask;

	if (curmask == 0) {
		if(!selmon->lt[selmon->sellt]->arrange || selmon->sel->isfloating)
			selmon->sel->wasfloating = 1;
		else {
			togglefloating(NULL);
			selmon->sel->wasfloating = 0;
		}
	}

	nx1 = calculate_expand(mask, curmask, &newmask,
			EXPAND_LEFT, &selmon->sel->expandx1,
			selmon->sel->x,
			selmon->wx,
			selmon->sel->oldx);
	nx2 = calculate_expand(mask, curmask, &newmask,
			EXPAND_RIGHT, &selmon->sel->expandx2,
			selmon->sel->x + selmon->sel->w,
			selmon->wx + selmon->ww - 2*bp,
			selmon->sel->oldw + selmon->sel->x);
	ny1 = calculate_expand(mask, curmask, &newmask,
			EXPAND_UP, &selmon->sel->expandy1,
			selmon->sel->y,
			selmon->wy,
			selmon->sel->oldy);
	ny2 = calculate_expand(mask, curmask, &newmask,
			EXPAND_DOWN, &selmon->sel->expandy2,
			selmon->sel->y + selmon->sel->h,
			selmon->wy + selmon->wh - 2*bp,
			selmon->sel->oldh + selmon->sel->y);


	resizeclient(selmon->sel, nx1, ny1, MAX(nx2-nx1, 32), MAX(ny2-ny1, 32));

	if ((newmask == 0) && (!selmon->sel->wasfloating))
		togglefloating(NULL);
	selmon->sel->expandmask = newmask;
	drawbar(selmon);
	while(XCheckMaskEvent(dpy, EnterWindowMask, &ev));
}

void
togglemaximize(const Arg *arg)
{
	if (arg->i > 0) expand(FORCE_EXPANDALL);
	else if (arg->i < 0) expand(UNEXPAND);
	else expand(EXPANDALL);
}

void
toggleverticalexpand(const Arg *arg)
{
	if (arg->i < 0) expand(EXPAND_DOWN);
	else if (arg->i > 0) expand(EXPAND_UP);
	else expand(EXPAND_DOWN | EXPAND_UP);
}

void
togglehorizontalexpand(const Arg *arg)
{
	if (arg->i < 0) expand(EXPAND_LEFT);
	else if (arg->i > 0) expand(EXPAND_RIGHT);
	else expand(EXPAND_LEFT | EXPAND_RIGHT);
}

