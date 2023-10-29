void
togglealttag()
{
	selmon->alttag = !selmon->alttag;
	drawbar(selmon);
}

