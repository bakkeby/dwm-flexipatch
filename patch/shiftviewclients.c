void
shiftviewclients(const Arg *arg)
{
	Arg shifted = shift(arg, 1);
	view(&shifted);
}
