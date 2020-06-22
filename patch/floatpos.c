void
floatpos(const Arg *arg)
{
	Client *c = selmon->sel;

	if (!c || !c->isfloating)
		return;

	setfloatpos(c, (char *)arg->v);
	resizeclient(c, c->x, c->y, c->w, c->h);

	#if !FOCUSONCLICK_PATCH
	XRaiseWindow(dpy, c->win);
	XWarpPointer(dpy, None, c->win, 0, 0, 0, 0, c->w/2, c->h/2);
	#endif // FOCUSONCLICK_PATCH
}

void
setfloatpos(Client *c, const char *floatpos)
{
	char xCh, yCh, wCh, hCh;
	int x, y, w, h, cx, cy, cw, ch, wx, ww, wy, wh, bw;
	unsigned int i;
	#if FLOATPOS_RESPECT_GAPS_PATCH
	int oh, ov, ih, iv;
	unsigned int n;
	#endif // FLOATPOS_RESPECT_GAPS_PATCH
	int absx, absy, absw, absh, delta, rest;

	if (!c || !floatpos)
		return;
	if (selmon->lt[selmon->sellt]->arrange && !c->isfloating)
		return;
	switch(sscanf(floatpos, "%d%c %d%c %d%c %d%c", &x, &xCh, &y, &yCh, &w, &wCh, &h, &hCh)) {
		case 4:
			if (xCh == 'w' || xCh == 'W') {
				w = x; wCh = xCh;
				h = y; hCh = yCh;
				x = -1; xCh = 'C';
				y = -1; yCh = 'C';
			} else if (xCh == 'p' || xCh == 'P') {
				w = x; wCh = xCh;
				h = y; hCh = yCh;
				x = 0; xCh = 'G';
				y = 0; yCh = 'G';
			} else {
				w = 0; wCh = 0;
				h = 0; hCh = 0;
			}
			break;
		case 8:
			break;
		default:
			return;
	}

	absx = xCh == 'A' || xCh == 'a';
	absy = yCh == 'A' || yCh == 'a';
	absw = wCh == 'A' || wCh == 'a';
	absh = hCh == 'A' || hCh == 'a';

	#if FLOATPOS_RESPECT_GAPS_PATCH
	getgaps(c->mon, &oh, &ov, &ih, &iv, &n);
	wx = c->mon->wx + ov;
	wy = c->mon->wy + oh;
	ww = c->mon->ww - 2*ov;
	wh = c->mon->wh - 2*oh;
	#else
	wx = c->mon->wx;
	wy = c->mon->wy;
	ww = c->mon->ww;
	wh = c->mon->wh;
	#endif // FLOATPOS_RESPECT_GAPS_PATCH

	cx = (!absx && c->x < wx ? wx : c->x);
	cy = (!absy && c->y < wy ? wy : c->y);
	cw = (!absw && c->w > ww ? ww : c->w);
	ch = (!absh && c->h > wh ? wh : c->h);

	switch(xCh) {
	case 'A': // absolute position
		cx = x;
		break;
	case 'a': // absolute relative position
		cx += x;
		break;
	case 'x': // client relative position
		if (cx + x > ww)
			cx = ww;
		else
			cx += x;
		break;
	case 'X': // client position relative to monitor
		if (x > ww)
			x = ww;
		cx = wx + x;
		break;
	case 'S': // fixed client position (sticky)
	case 'C': // fixed client position (center)
	case 'Z': // fixed client right-hand position (position + width)
		if (x == -1)
			break;
		if (x > ww)
			x = ww;
		if (x < 0)
			x = 0;
		if (xCh == 'Z')
			cw = abs((cx + cw) - (wx + x));
		else if (xCh == 'C')
			cw = abs((cx + cw / 2) - (wx + x));
		else
			cw = abs(cx - (wx + x));
		cx = wx + x;
		wCh = 0; // size determined by position, override defined width
		break;
	case 'G': // grid
		if (x <= 0)
			x = floatposgrid_x; // default configurable
		if (w == 0 || x < 2 || (wCh != 'p' && wCh != 'P'))
			break;
		delta = (ww - cw - 2*c->bw) / (x - 1);
		rest = ww - cw - 2*c->bw - delta * (x - 1);
		if (wCh == 'P') {
			if (w < 1 || w > x)
				break;
			cx = wx + delta * (w - 1);
		} else {
			for (i = 0; i < x -1 && cx >= wx + delta * i + (i > x - rest ? i + rest - x + 1 : 0); i++);
			cx = wx + delta * (MAX(MIN(i + w, x), 1) - 1) + (i > x - rest ? i + rest - x + 1 : 0);
		}
		break;
	}

	switch(yCh) {
	case 'A': // absolute position
		cy = y;
		break;
	case 'a': // absolute relative position
		cy += y;
		break;
	case 'y': // client relative position
		if (cy + y > wh)
			cy = wh;
		else
			cy += y;
		break;
	case 'Y': // client position relative to monitor
		if (y > wh)
			y = wh;
		cy = wy + y;
		break;
	case 'S': // fixed client y position (sticky)
	case 'C': // fixed client position (center)
	case 'Z': // fixed client right-hand position (position + height)
		if (y == -1)
			break;
		if (y > wh)
			y = wh;
		if (y < 0)
			y = 0;
		if (yCh == 'Z')
			ch = abs((cy + ch) - (wy + y));
		else if (yCh == 'C')
			ch = abs((cy + ch / 2) - (wy + y));
		else
			ch = abs(cy - (wy + y));
		cy = wy + y;
		hCh = 0; // size determined by position, override defined width
		break;
	case 'G': // grid
		if (y <= 0)
			y = floatposgrid_y; // default configurable
		if (h == 0 || y < 1 || (hCh != 'p' && hCh != 'P'))
			break;
		delta = (wh - ch - 2*c->bw) / (y - 1);
		rest = wh - ch - 2*c->bw - delta * (y - 1);
		if (hCh == 'P') {
			if (h < 1 || h > y)
				break;
			cy = wy + delta * (h - 1);
		} else {
			for (i = 0; i < y - 1 && cy >= wy + delta * i + (i > y - rest ? i + rest - y + 1: 0); i++);
			cy = wy + delta * (MAX(MIN(i + h, y), 1) - 1) + (i > y - rest ? i + rest - y + 1: 0);
		}
		hCh = 0; // size determined by position, override defined width
		break;
	}

	bw = 2*c->bw;
	switch(wCh) {
	case 'A': // absolute width
		cw = w - 2*c->bw;
		break;
	case 'a': // absolute relative width
		if (cw + w < 1)
			cw = 1;
		else
			cw += w;
		break;
	case '%': // client width percentage in relation to monitor window area width
		if (w > 100)
			w = 100;
		if (w <= 0)
			break;
		w = ww * w / 100;
		/* falls through */
	case 'w': // width relative to client
		if (wCh == 'w') {
			if (w == 0)
				break;
			else {
				w += cw;
				bw = 0;
			}
		}
		/* falls through */
	case 'W': // normal width, position takes precedence
		if (xCh == 'S' && cx + w > wx + ww)
			w = wx + ww - cx;
		else if (xCh == 'C' && cx + cw / 2 + w / 2 > wx + ww)
			w = wx + ww - cx + cw / 2;
		else if (xCh == 'Z' && w > cx - ww)
			w = cx - wx;
		else if (w > ww)
			w = ww;

		if (xCh == 'C') { // fixed client center, expand or contract client
			delta = w - bw - cw;
			if (delta < 0 || (cx - delta / 2 + w + 2*c->bw <= wx + ww))
				cx -= delta / 2;
			else if (cx - delta / 2 < wx)
				cx = wx;
			else if (delta)
				cx = wx + ww - cw - bw;
		}

		cw = w - bw;
		break;
	}

	bw = 2*c->bw;
	switch(hCh) {
	case 'A': // absolute height
		ch = h - 2*c->bw;
		break;
	case 'a': // absolute relative height
		if (ch + h < 1)
			ch = 1;
		else
			ch += h;
		break;
	case '%': // client height percentage in relation to monitor window area height
		if (h > 100)
			h = 100;
		if (h <= 0)
			break;
		h = wh * h / 100;
		/* falls through */
	case 'h': // height relative to client
		if (hCh == 'h') {
			if (h == 0)
				break;
			else {
				h += ch;
				bw = 0;
			}
		}
		/* falls through */
	case 'H': /* normal height, position takes precedence */
		if (yCh == 'S' && cy + h > wy + wh)
			h = wy + wh - cy;
		else if (yCh == 'C' && cy + ch / 2 + h / 2 > wy + wh)
			h = wy + wh - cy + ch / 2;
		else if (yCh == 'Z' && h > cy - wh)
			h = cy - wy;
		else if (h > wh)
			h = wh;

		if (yCh == 'C') { // fixed client center, expand or contract client
			delta = h - bw - ch;
			if (delta < 0 || (cy - delta / 2 + ch + 2*c->bw <= wy + wh))
				cy -= delta / 2;
			else if (cy - delta / 2 < wy)
				cy = wy;
			else if (delta)
				cy = wy + wh - ch - bw;
		}

		ch = h - bw;
		break;
	}

	if (xCh == '%') { // client mid-point position in relation to monitor window area width
		if (x > 100)
			x = 100;
		if (x < 0)
			x = 0;
		cx = wx + ww * x / 100 - (cw + 2*c->bw) / 2;
	}

	if (yCh == '%') { // client mid-point position in relation to monitor window area height
		if (y > 100)
			y = 100;
		if (y < 0)
			y = 0;
		cy = wy + wh * y / 100 - (ch + 2*c->bw) / 2;
	}

	if (!absx && cx < wx)
		cx = wx;
	if (cx + cw + 2*c->bw > wx + ww && !(absx && absw)) {
		if (absx || cx == wx)
			cw = ww - cx + wx - 2*c->bw;
		else
			cx = wx + ww - cw - 2*c->bw;
	}

	if (!absy && cy < wy)
		cy = wy;
	if (cy + ch + 2*c->bw > wy + wh && !(absy && absh)) {
		if (absy || cy == wy)
			ch = wh - cy + wy - 2*c->bw;
		else
			cy = wy + wh - ch - 2*c->bw;
	}

	c->x = cx;
	c->y = cy;
	c->w = MAX(cw, 1);
	c->h = MAX(ch, 1);
}