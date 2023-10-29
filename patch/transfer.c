void
transfer(const Arg *arg)
{
	Client *c, *mtail = selmon->clients, *stail = NULL, *insertafter;
	int transfertostack = 0, i, nmasterclients;

	for (i = 0, c = selmon->clients; c; c = c->next) {
		if (!ISVISIBLE(c) || c->isfloating) continue;
		if (selmon->sel == c) { transfertostack = i < selmon->nmaster && selmon->nmaster != 0; }
		if (i < selmon->nmaster) { nmasterclients++; mtail = c; }
		stail = c;
		i++;
	}
	if (!selmon->sel || selmon->sel->isfloating || i == 0) {
		return;
	} else if (transfertostack) {
		selmon->nmaster = MIN(i, selmon->nmaster) - 1;
		insertafter = stail;
	} else {
		selmon->nmaster = selmon->nmaster + 1;
		insertafter = mtail;
	}
	if (insertafter != selmon->sel) {
		detach(selmon->sel);
		if (selmon->nmaster == 1 && !transfertostack) {
		 attach(selmon->sel); // Head prepend case
		} else {
			selmon->sel->next = insertafter->next;
			insertafter->next = selmon->sel;
		}
	}
	arrange(selmon);
}

