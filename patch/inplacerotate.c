void
insertclient(Client *item, Client *insertItem, int after)
{
	Client *c;
	if (item == NULL || insertItem == NULL || item == insertItem)
		return;
	detach(insertItem);
	if (!after && selmon->clients == item) {
		attach(insertItem);
		return;
	}
	if (after) {
		c = item;
	} else {
		for (c = selmon->clients; c; c = c->next) {
			if (c->next == item)
				break;
		}
	}
	insertItem->next = c->next;
	c->next = insertItem;
}

void
inplacerotate(const Arg *arg)
{
	if (!selmon->sel || (selmon->sel->isfloating && !arg->f))
		return;

	unsigned int selidx = 0, i = 0;
	Client *c = NULL, *stail = NULL, *mhead = NULL, *mtail = NULL, *shead = NULL;

	// Determine positionings for insertclient
	for (c = selmon->clients; c; c = c->next) {
		if (ISVISIBLE(c) && !(c->isfloating)) {
			if (selmon->sel == c)
				selidx = i;
			if (i == selmon->nmaster - 1)
				mtail = c;
			if (i == selmon->nmaster)
				shead = c;
			if (mhead == NULL)
				mhead = c;
			stail = c;
			i++;
		}
	}

	switch(arg->i) {
	case 1:
		if (selidx >= selmon->nmaster)
			insertclient(shead, stail, 0);
		else
			insertclient(mhead, mtail, 0);
		break;
	case -1:
		if (selidx >= selmon->nmaster)
			insertclient(stail, shead, 1);
		else
			insertclient(mtail, mhead, 1);
		break;
	case 2:
		insertclient(selmon->clients, stail, 0);
		break;
	case -2:
		insertclient(stail, selmon->clients, 1);
		break;
	}

	// Restore focus position
	i = 0;
	for (c = selmon->clients; c; c = c->next) {
		if (!ISVISIBLE(c) || (c->isfloating))
			continue;
		if (i == selidx) {
			focus(c);
			break;
		}
		i++;
	}
	arrange(selmon);
	focus(c);
}

