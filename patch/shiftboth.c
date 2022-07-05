void
shiftboth(const Arg *arg)
{
	Arg shifted = shift(arg, 0);
	tag(&shifted);
	view(&shifted);
}
