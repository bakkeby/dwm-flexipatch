int
width_status2d_eb(Monitor *m, int max_width)
{
	return status2dtextlength(rawestext);
}

int
draw_status2d_eb(Monitor *m, int x, int w)
{
	return drawstatusbar(m, x, w, rawestext);
}