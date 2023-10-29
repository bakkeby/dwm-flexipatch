static int th = 0; /* tab bar geometry */

void
drawtabs(void)
{
	Monitor *m;

	for (m = mons; m; m = m->next)
		drawtab(m);
}

static int
cmpint(const void *p1, const void *p2)
{
  /* The actual arguments to this function are "pointers to
     pointers to char", but strcmp(3) arguments are "pointers
     to char", hence the following cast plus dereference */
  return *((int*) p1) > * (int*) p2;
}

void
drawtab(Monitor *m)
{
	Client *c;
	int i;
	int itag = -1;
	char view_info[50];
	int view_info_w = 0;
	int sorted_label_widths[MAXTABS];
	int tot_width;
	int maxsize = bh;
	int x = 0;
	int w = 0;

	// view_info: indicate the tag which is displayed in the view
	for (i = 0; i < NUMTAGS; ++i) {
		if ((selmon->tagset[selmon->seltags] >> i) & 1) {
			if (itag >=0) { // more than one tag selected
				itag = -1;
				break;
			}
			itag = i;
		}
	}

	if (0 <= itag  && itag < NUMTAGS) {
		snprintf(view_info, sizeof view_info, "[%s]", tagicon(m, itag));
	} else {
		strncpy(view_info, "[...]", sizeof view_info);
	}
	view_info[sizeof(view_info) - 1 ] = 0;
	view_info_w = TEXTW(view_info);
	tot_width = view_info_w;

	/* Calculates number of labels and their width */
	m->ntabs = 0;
	for (c = m->clients; c; c = c->next) {
		if (!ISVISIBLE(c) || HIDDEN(c))
			continue;
		m->tab_widths[m->ntabs] = TEXTW(c->name);
		tot_width += m->tab_widths[m->ntabs];
		++m->ntabs;
		if (m->ntabs >= MAXTABS)
			break;
	}

	if (tot_width > m->ww) { // not enough space to display the labels, they need to be truncated
		memcpy(sorted_label_widths, m->tab_widths, sizeof(int) * m->ntabs);
		qsort(sorted_label_widths, m->ntabs, sizeof(int), cmpint);
		tot_width = view_info_w;
		for (i = 0; i < m->ntabs; ++i) {
			if (tot_width + (m->ntabs - i) * sorted_label_widths[i] > m->ww)
				break;
			tot_width += sorted_label_widths[i];
		}
		maxsize = (m->ww - tot_width) / (m->ntabs - i);
	} else {
		maxsize = m->ww;
	}
	i = 0;
	for (c = m->clients; c; c = c->next) {
		if (!ISVISIBLE(c) || HIDDEN(c))
			continue;
		if (i >= m->ntabs)
			break;
		if (m->tab_widths[i] > maxsize)
			m->tab_widths[i] = maxsize;
		w = m->tab_widths[i];
		drw_setscheme(drw, scheme[(c == m->sel) ? SchemeSel : SchemeNorm]);
		drw_text(drw, x, 0, w, th, 0, c->name, 0, False);
		x += w;
		++i;
	}

	drw_setscheme(drw, scheme[SchemeNorm]);

	/* cleans interspace between window names and current viewed tag label */
	w = m->ww - view_info_w - x;
	drw_text(drw, x, 0, w, th, 0, "", 0, False);

	/* view info */
	x += w;
	w = view_info_w;
	drw_text(drw, x, 0, w, th, 0, view_info, 0, False);

	drw_map(drw, m->tabwin, 0, 0, m->ww, th);
}

void
focuswin(const Arg* arg)
{
	int iwin = arg->i;
	Client* c = NULL;
	for (c = selmon->clients; c && (iwin || !ISVISIBLE(c)) ; c = c->next) {
		if (ISVISIBLE(c) && !HIDDEN(c))
			--iwin;
	};
	if (c) {
		focus(c);
		restack(selmon);
	}
}

void
tabmode(const Arg *arg)
{
	if (arg && arg->i >= 0)
		selmon->showtab = arg->ui % showtab_nmodes;
	else
		selmon->showtab = (selmon->showtab + 1 ) % showtab_nmodes;
	arrange(selmon);
}

