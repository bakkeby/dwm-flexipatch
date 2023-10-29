/* Sends a window to the next/prev tag that has a client, else it moves it to the next/prev one. */
void
shifttagclients(const Arg *arg)
{
	Arg shifted = shift(arg, 1);
	tag(&shifted);
}
