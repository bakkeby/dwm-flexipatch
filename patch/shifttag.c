/* Sends a window to the next/prev tag */
void
shifttag(const Arg *arg)
{
	Arg shifted = shift(arg, 0);
	tag(&shifted);
}
