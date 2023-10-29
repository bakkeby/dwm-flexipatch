/* swaps "tags" (all the clients) with the next/prev tag. */
void
shiftswaptags(const Arg *arg)
{
	Arg shifted = shift(arg, 0);
	swaptags(&shifted);
}
