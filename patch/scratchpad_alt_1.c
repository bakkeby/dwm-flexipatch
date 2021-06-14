static Client * scratchpad_last_showed = NULL;

void
scratchpad_hide()
{
	if (selmon->sel) {
		selmon->sel->tags = SCRATCHPAD_MASK;
		selmon->sel->isfloating = 1;
		focus(NULL);
		arrange(selmon);
	}
}

_Bool
scratchpad_last_showed_is_killed(void)
{
	Client *c;
	for (c = selmon->clients; c && c != scratchpad_last_showed; c = c->next);
	return (c == NULL);
}

void
scratchpad_remove()
{
	if (selmon->sel && scratchpad_last_showed != NULL && selmon->sel == scratchpad_last_showed)
		scratchpad_last_showed = NULL;
}

void
scratchpad_show()
{
	if (scratchpad_last_showed == NULL || scratchpad_last_showed_is_killed()) {
		scratchpad_show_first();
		return;
	}

	if (scratchpad_last_showed->tags != SCRATCHPAD_MASK) {
		scratchpad_last_showed->tags = SCRATCHPAD_MASK;
		focus(NULL);
		arrange(selmon);
		return;
	}

	Client *c;

	for (c = selmon->clients; c && c != scratchpad_last_showed; c = c->next);
	for (c = (c ? c->next : NULL); c && c->tags != SCRATCHPAD_MASK; c = c->next);

	if (c)
		scratchpad_show_client(c);
	else
		scratchpad_show_first();
}

void
scratchpad_show_client(Client* c)
{
	scratchpad_last_showed = c;
	c->tags = selmon->tagset[selmon->seltags];
	focus(c);
	arrange(selmon);
}

void
scratchpad_show_first(void)
{
	Client *c;
	for (c = selmon->clients; c && c->tags != SCRATCHPAD_MASK; c = c->next);
	if (c)
		scratchpad_show_client(c);
}

