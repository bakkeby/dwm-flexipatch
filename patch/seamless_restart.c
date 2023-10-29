void
persistmonitorstate(Monitor *m)
{
	Client *c;
	unsigned int i;

	setmonitortags(m);
	setmonitorfields(m);

	/* Set client atoms */
	for (i = 1, c = m->clients; c; c = c->next, ++i) {
		c->idx = i;
		persistclientstate(c);
		#if SWALLOW_PATCH
		if (c->swallowing) {
			c->swallowing->idx = i;
			persistclientstate(c->swallowing);
		}
		#endif // SWALLOW_PATCH
	}
}

int
restoremonitorstate(Monitor *m)
{
	return getmonitortags(m) | getmonitorfields(m);
}

void
persistclientstate(Client *c)
{
	setclienttags(c);
	setclientfields(c);
	#if SAVEFLOATS_PATCH
	savewindowfloatposition(c, c->mon);
	#endif // SAVEFLOATS_PATCH
}

int
restoreclientstate(Client *c)
{
	return getclienttags(c)
		| getclientfields(c)
		#if SAVEFLOATS_PATCH
		| restorewindowfloatposition(c, c->mon ? c->mon : selmon)
		#endif // SAVEFLOATS_PATCH
	;
}

void setmonitorfields(Monitor *m)
{
	#if PERTAG_PATCH
	unsigned int i;
	#endif // PERTAG_PATCH
	char atom[22] = {0};
	Atom monitor_fields;
	#if FLEXTILE_DELUXE_LAYOUT
	unsigned int flextile_deluxe_bitmask;
	#endif // FLEXTILE_DELUXE_LAYOUT

	sprintf(atom, "_DWM_MONITOR_FIELDS_%u", m->num);
	monitor_fields = XInternAtom(dpy, atom, False);

	/* Perists workspace information in 32 bits laid out like this:
	 *
	 * |0|0000|0|0000|0000|0000|0000|0000|000|000
	 * | |    | |    |    |    |    |    |   |-- nmaster
	 * | |    | |    |    |    |    |    |-- nstack
	 * | |    | |    |    |    |    |-- layout
	 * | |    | |    |    |    |-- flextile LAYOUT (split)
	 * | |    | |    |    |-- flextile MASTER
	 * | |    | |    |-- flextile STACK1
	 * | |    | |-- flextile STACK2
	 * | |    |-- flextile mirror layout (indicated by negative layout)
	 * | |
	 * | |-- reserved
	 * |-- showbar
	 */
	#if PERTAG_PATCH
	for (i = 0; i <= NUMTAGS; i++) {
		#if FLEXTILE_DELUXE_LAYOUT
		flextile_deluxe_bitmask = (m->pertag->nstacks[i] & 0x7) << 3;
		if (m->pertag->ltidxs[i][m->pertag->sellts[i]]->arrange == flextile) {
			flextile_deluxe_bitmask |=
				(abs(m->pertag->ltaxis[i][LAYOUT]) & 0xF) << 10 |
				(m->pertag->ltaxis[i][MASTER] & 0xF) << 14 |
				(m->pertag->ltaxis[i][STACK]  & 0xF) << 18 |
				(m->pertag->ltaxis[i][STACK2] & 0xF) << 22 |
				(m->pertag->ltaxis[i][LAYOUT] < 0 ? 1 : 0) << 24;
		}
		#endif // FLEXTILE_DELUXE_L1AYOUT
		uint32_t data[] = {
			#if FLEXTILE_DELUXE_LAYOUT
			flextile_deluxe_bitmask |
			#endif // FLEXTILE_DELUXE_LAYOUT
			(m->pertag->nmasters[i] & 0x7) |
			(getlayoutindex(m->pertag->ltidxs[i][m->pertag->sellts[i]]) & 0xF) << 6 |
			#if PERTAGBAR_PATCH
			m->pertag->showbars[i] << 31
			#else
			m->showbar << 31
			#endif // PERTAGBAR_PATCH
		};

		XChangeProperty(dpy, root, monitor_fields, XA_CARDINAL, 32,
			i ? PropModeAppend : PropModeReplace, (unsigned char *)data, 1);
	}
	#else // !PERTAG_PATCH
	#if FLEXTILE_DELUXE_LAYOUT
	flextile_deluxe_bitmask = (m->nstack & 0x7) << 3;
	if (m->lt[m->sellt]->arrange == flextile) {
		flextile_deluxe_bitmask |=
			(abs(m->ltaxis[LAYOUT]) & 0xF) << 10 |
			(m->ltaxis[MASTER] & 0xF) << 14 |
			(m->ltaxis[STACK]  & 0xF) << 18 |
			(m->ltaxis[STACK2] & 0xF) << 22 |
			(m->ltaxis[LAYOUT] < 0 ? 1 : 0) << 24;
	}
	#endif // FLEXTILE_DELUXE_L1AYOUT
	uint32_t data[] = {
		#if FLEXTILE_DELUXE_LAYOUT
		flextile_deluxe_bitmask |
		#endif // FLEXTILE_DELUXE_LAYOUT
		(m->nmaster & 0x7) |
		(getlayoutindex(m->lt[m->sellt]) & 0xF) << 6 |
		m->showbar << 31
	};

	XChangeProperty(dpy, root, monitor_fields, XA_CARDINAL, 32, PropModeReplace,
		(unsigned char *)data, 1);
	#endif // PERTAG_PATCH
}

int
getlayoutindex(const Layout *layout)
{
	int i;

	for (i = 0; i < LENGTH(layouts) && &layouts[i] != layout; i++);
	if (i == LENGTH(layouts))
		i = 0;
	return i;
}

int
getmonitorfields(Monitor *m)
{
	int di, layout_index;
	#if PERTAG_PATCH
	unsigned int i, restored = 0;
	unsigned int tags = m->tagset[m->seltags] << 1;
	#endif // PERTAG_PATCH
	unsigned long dl, nitems;
	unsigned char *p = NULL;
	char atom[22] = {0};
	Atom da, state = None;

	sprintf(atom, "_DWM_MONITOR_FIELDS_%u", m->num);
	Atom dwm_monitor = XInternAtom(dpy, atom, False);
	if (!dwm_monitor)
		return 0;

	#if PERTAG_PATCH
	for (i = 0; i <= NUMTAGS; i++) {
		if (!(XGetWindowProperty(dpy, root, dwm_monitor, i, (NUMTAGS + 1) * sizeof dl,
				False, AnyPropertyType, &da, &di, &nitems, &dl, &p) == Success && p)) {
			break;
		}

		if (!nitems) {
			XFree(p);
			break;
		}

		/* See bit layout in the persistmonitorstate function */
		state = *(Atom *)p;

		m->pertag->nmasters[i] = state & 0x7;
		layout_index = (state >> 6) & 0xF;
		if (layout_index < LENGTH(layouts))
			m->pertag->ltidxs[i][m->pertag->sellts[i]] = &layouts[layout_index];
		#if FLEXTILE_DELUXE_LAYOUT
		m->pertag->nstacks[i] = (state >> 3) & 0x7;
		if (m->pertag->ltidxs[i][m->pertag->sellts[i]]->arrange == flextile) {
			m->pertag->ltaxis[i][LAYOUT] = (state >> 10) & 0xF;
			m->pertag->ltaxis[i][MASTER] = (state >> 14) & 0xF;
			m->pertag->ltaxis[i][STACK]  = (state >> 18) & 0xF;
			m->pertag->ltaxis[i][STACK2] = (state >> 22) & 0xF;
			if (state >> 24 & 0x1) {
				m->pertag->ltaxis[i][LAYOUT] *= -1;
			}
		}
		#endif // FLEXTILE_DELUXE_LAYOUT
		#if PERTAGBAR_PATCH
		m->pertag->showbars[i] = (state >> 31) & 0x1;
		#endif // PERTAGBAR_PATCH

		if (!restored && i && (tags & (1 << i))) {
			m->nmaster = m->pertag->nmasters[i];
			m->sellt = m->pertag->sellts[i];
			m->lt[m->sellt] = m->pertag->ltidxs[i][m->sellt];
			#if FLEXTILE_DELUXE_LAYOUT
			m->nstack = m->pertag->nstacks[i];
			if (m->lt[m->sellt]->arrange == flextile) {
				m->ltaxis[LAYOUT] = m->pertag->ltaxis[i][LAYOUT];
				m->ltaxis[MASTER] = m->pertag->ltaxis[i][MASTER];
				m->ltaxis[STACK]  = m->pertag->ltaxis[i][STACK];
				m->ltaxis[STACK2] = m->pertag->ltaxis[i][STACK2];
			}
			#endif // FLEXTILE_DELUXE_LAYOUT
			#if PERTAGBAR_PATCH
			m->showbar = m->pertag->showbars[i];
			#else
			m->showbar = (state >> 31) & 0x1;
			#endif // PERTAGBAR_PATCH
			restored = 1;
		}

		XFree(p);
	}

	return restored;
	#else // !PERTAG_PATCH
	if (!(XGetWindowProperty(dpy, root, dwm_monitor, 0L, sizeof dl,
			False, AnyPropertyType, &da, &di, &nitems, &dl, &p) == Success && p)) {
		return 0;
	}

	if (nitems) {
		state = *(Atom *)p;

		/* See bit layout in the persistmonitorstate function */
		m->nmaster = state & 0x7;
		#if FLEXTILE_DELUXE_LAYOUT
		m->nstack = (state >> 3) & 0x7;
		#endif // FLEXTILE_DELUXE_LAYOUT
		layout_index = (state >> 6) & 0xF;
		if (layout_index < LENGTH(layouts))
			m->lt[m->sellt] = &layouts[layout_index];
		#if FLEXTILE_DELUXE_LAYOUT
		if (m->lt[m->sellt]->arrange == flextile) {
			m->ltaxis[LAYOUT] = (state >> 10) & 0xF;
			m->ltaxis[MASTER] = (state >> 14) & 0xF;
			m->ltaxis[STACK]  = (state >> 18) & 0xF;
			m->ltaxis[STACK2] = (state >> 22) & 0xF;
		}
		#endif // FLEXTILE_DELUXE_LAYOUT
		m->showbar = (state >> 31) & 0x1;
	}

	XFree(p);
	return 1;
	#endif // PERTAG_PATCH
}

void
setmonitortags(Monitor *m)
{
	char atom[22] = {0};
	Atom monitor_tags;

	sprintf(atom, "_DWM_MONITOR_TAGS_%u", m->num);
	monitor_tags = XInternAtom(dpy, atom, False);

	uint32_t data[] = { m->tagset[m->seltags] };
	XChangeProperty(dpy, root, monitor_tags, XA_CARDINAL, 32, PropModeReplace, (unsigned char *)data, 1);
}

int
getmonitortags(Monitor *m)
{
	int di;
	unsigned long dl, nitems;
	unsigned char *p = NULL;
	char atom[22] = {0};
	Atom da, monitor_tags = None, tags;

	sprintf(atom, "_DWM_MONITOR_TAGS_%u", m->num);
	monitor_tags = XInternAtom(dpy, atom, False);

	if (!(XGetWindowProperty(dpy, root, monitor_tags, 0L, sizeof dl,
			False, AnyPropertyType, &da, &di, &nitems, &dl, &p) == Success && p)) {
		return 0;
	}

	if (nitems) {
		tags = *(Atom *)p;
		m->tagset[m->seltags] = tags & TAGMASK;
	}

	XFree(p);
	return 1;
}

void
setclientfields(Client *c)
{
	/* Perists client information in 32 bits laid out like this:
	 *
	 * |00000000|00000|0|0|0|0|0|0|0|0|00000000|000
	 * |        |     | | | | | | | | |        |-- monitor index
	 * |        |     | | | | | | | | |-- client index
	 * |        |     | | | | | | | |-- isfloating
	 * |        |     | | | | | | |-- ispermanent
	 * |        |     | | | | | |-- isterminal
	 * |        |     | | | | |-- noswallow
	 * |        |     | | | |-- issteam
	 * |        |     | | |-- issticky
	 * |        |     | |-- fakefullscreen
	 * |        |     |-- isfreesize
	 * |        |
	 * |        |-- reserved
	 * |-- scratchkey (for scratchpads)
	 */
	uint32_t data[] = {
		(c->mon->num & 0x7)
		| (c->idx & 0xFF) << 3
		| (c->isfloating & 0x1) << 11
		#if ISPERMANENT_PATCH
		| (c->ispermanent & 0x1) << 12
		#endif // ISPERMANENT_PATCH
		#if SWALLOW_PATCH
		| (c->isterminal & 0x1) << 13
		| (c->noswallow & 0x1) << 14
		#endif // SWALLOW_PATCH
		#if STEAM_PATCH
		| (c->issteam & 0x1) << 15
		#endif // STEAM_PATCH
		#if STICKY_PATCH
		| (c->issticky & 0x1) << 16
		#endif // STICKY_PATCH
		#if !FAKEFULLSCREEN_PATCH && FAKEFULLSCREEN_CLIENT_PATCH
		| (c->fakefullscreen & 0x1) << 17
		#endif // FAKEFULLSCREEN_CLIENT_PATCH
		#if SIZEHINTS_ISFREESIZE_PATCH
		| (c->isfreesize & 0x1) << 18
		#endif // SIZEHINTS_ISFREESIZE_PATCH
		#if RENAMED_SCRATCHPADS_PATCH
		| (c->scratchkey & 0xFF) << 24
		#endif // RENAMED_SCRATCHPADS_PATCH
	};
	XChangeProperty(dpy, c->win, clientatom[ClientFields], XA_CARDINAL, 32, PropModeReplace, (unsigned char *)data, 1);
}

int
getclientfields(Client *c)
{
	Monitor *m;
	Atom fields = getatomprop(c, clientatom[ClientFields], AnyPropertyType);
	if (fields == None)
		return 0;

	/* See bit layout in the setclientfields function */
	for (m = mons; m; m = m->next)
		if (m->num == (fields & 0x7)) {
			c->mon = m;
			break;
		}
	c->idx = (fields >> 3) & 0xFF;
	c->isfloating = (fields >> 11) & 0x1;
	#if ISPERMANENT_PATCH
	c->ispermanent = (fields >> 12) & 0x1;
	#endif // ISPERMANENT_PATCH
	#if SWALLOW_PATCH
	c->isterminal = (fields >> 13) & 0x1;
	c->noswallow = (fields >> 14) & 0x1;
	#endif // SWALLOW_PATCH
	#if STEAM_PATCH
	c->issteam = (fields >> 15) & 0x1;
	#endif // STEAM_PATCH
	#if STICKY_PATCH
	c->issticky = (fields >> 16) & 0x1;
	#endif // STICKY_PATCH
	#if !FAKEFULLSCREEN_PATCH && FAKEFULLSCREEN_CLIENT_PATCH
	c->fakefullscreen = (fields >> 17) & 0x1;
	#endif // FAKEFULLSCREEN_CLIENT_PATCH
	#if SIZEHINTS_ISFREESIZE_PATCH
	c->isfreesize = (fields >> 18) & 0x1;
	#endif // SIZEHINTS_ISFREESIZE_PATCH
	#if RENAMED_SCRATCHPADS_PATCH
	c->scratchkey = (fields >> 24) & 0xFF;
	#endif // RENAMED_SCRATCHPADS_PATCH
	return 1;
}

void
setclienttags(Client *c)
{
	uint32_t data[] = { c->tags };
	XChangeProperty(dpy, c->win, clientatom[ClientTags], XA_CARDINAL, 32, PropModeReplace, (unsigned char *)data, 1);
}

int
getclienttags(Client *c)
{
	Atom tags = getatomprop(c, clientatom[ClientTags], AnyPropertyType);
	if (tags == None)
		return 0;

	c->tags = tags & TAGMASK;
	return 1;
}

#if SAVEFLOATS_PATCH
void
savewindowfloatposition(Client *c, Monitor *m)
{
	char atom[22] = {0};
	if (c->sfx == -9999)
		return;

	sprintf(atom, "_DWM_FLOATPOS_%u", m->num);
	uint32_t pos[] = { (MAX(c->sfx - m->mx, 0) & 0xffff) | ((MAX(c->sfy - m->my, 0) & 0xffff) << 16) };
	XChangeProperty(dpy, c->win, XInternAtom(dpy, atom, False), XA_CARDINAL, 32, PropModeReplace, (unsigned char *)pos, 1);

	sprintf(atom, "_DWM_FLOATSIZE_%u", m->num);
	uint32_t size[] = { (c->sfw & 0xffff) | ((c->sfh & 0xffff) << 16) };
	XChangeProperty(dpy, c->win, XInternAtom(dpy, atom, False), XA_CARDINAL, 32, PropModeReplace, (unsigned char *)size, 1);

	XSync(dpy, False);
}

int
restorewindowfloatposition(Client *c, Monitor *m)
{
	char atom[22] = {0};
	Atom key, value;
	int x, y, w, h;

	if (m == NULL)
		return 0;

	sprintf(atom, "_DWM_FLOATPOS_%u", m->num);

	key = XInternAtom(dpy, atom, False);
	if (!key)
		return 0;

	value = getatomprop(c, key, AnyPropertyType);
	if (!value)
		return 0;

	x = value & 0xffff;
	y = value >> 16;

	sprintf(atom, "_DWM_FLOATSIZE_%u", m->num);

	key = XInternAtom(dpy, atom, False);
	if (!key)
		return 0;

	value = getatomprop(c, key, AnyPropertyType);
	if (!value)
		return 0;

	w = value & 0xffff;
	h = value >> 16;

	if (w <= 0 || h <= 0) {
		fprintf(stderr, "restorewindowfloatposition: bad float values x = %d, y = %d, w = %d, h = %d for client = %s\n", x, y, w, h, c->name);
		return 0;
	}

	c->sfx = m->mx + x;
	c->sfy = m->my + y;
	c->sfw = w;
	c->sfh = h;

	if (c->isfloating) {
		c->x = c->sfx;
		c->y = c->sfy;
		c->w = c->sfw;
		c->h = c->sfh;
	}

	return 1;
}
#endif // SAVEFLOATS_PATCH
