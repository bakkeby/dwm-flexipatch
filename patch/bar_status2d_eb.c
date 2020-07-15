int
width_status2d_eb(Monitor *m, BarWidthArg *a)
{
	return status2dtextlength(rawestext);
}

int
draw_status2d_eb(Monitor *m, BarDrawArg *a)
{
	return drawstatusbar(m, a->x, a->w, rawestext);
}