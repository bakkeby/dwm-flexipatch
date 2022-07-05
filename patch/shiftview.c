void
shiftview(const Arg *arg)
{
	Arg shifted = shift(arg, 0);
	view(&shifted);
}
