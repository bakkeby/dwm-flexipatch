int
width_status2d_eb(Bar *bar, BarWidthArg *a)
{
	return status2dtextlength(rawestext);
}

int
draw_status2d_eb(Bar *bar, BarDrawArg *a)
{
	return drawstatusbar(a->x, rawestext);
}