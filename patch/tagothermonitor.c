#if IPC_PATCH || DWMC_PATCH
void
tagnextmonex(const Arg *arg)
{
	tagnextmon(&((Arg) { .ui = 1 << arg->ui }));
}

void
tagprevmonex(const Arg *arg)
{
	tagprevmon(&((Arg) { .ui = 1 << arg->ui }));
}
#endif // IPC_PATCH | DWMC_PATCH

void
tagnextmon(const Arg *arg)
{
	tagothermon(arg, 1);
}

void
tagprevmon(const Arg *arg)
{
	tagothermon(arg, -1);
}

void
tagothermon(const Arg *arg, int dir)
{
	Client *sel;
	Monitor *newmon;

	if (!selmon->sel || !mons->next)
		return;
	sel = selmon->sel;
	newmon = dirtomon(dir);
	sendmon(sel, newmon);
	if (arg->ui & TAGMASK) {
		sel->tags = arg->ui & TAGMASK;
		arrange(newmon);
		focus(NULL);
	}
}

