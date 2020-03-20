void
togglefullscreen(const Arg *arg)
{
    if (!selmon->sel)
        return;

    #if !FAKEFULLSCREEN_PATCH && FAKEFULLSCREEN_CLIENT_PATCH
    if (selmon->sel->fakefullscreen) {
    	selmon->sel->fakefullscreen = 0;
    	if (selmon->sel->isfullscreen) {
    		selmon->sel->isfullscreen = 0;
    	}
    }
    #endif // FAKEFULLSCREEN_CLIENT_PATCH

    setfullscreen(selmon->sel, !selmon->sel->isfullscreen);
}