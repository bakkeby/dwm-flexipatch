void
togglefakefullscreen(const Arg *arg)
{
    if (!selmon->sel)
        return;

    if (selmon->sel->fakefullscreen) {
        if (selmon->sel->isfullscreen)
            selmon->sel->fakefullscreen = 0;
        else
            selmon->sel->isfullscreen = 0;
    } else {
        if (selmon->sel->isfullscreen) {
            selmon->sel->isfloating = selmon->sel->oldstate;
            selmon->sel->bw = selmon->sel->oldbw;
            selmon->sel->x = selmon->sel->oldx;
            selmon->sel->y = selmon->sel->oldy;
            selmon->sel->w = selmon->sel->oldw;
            selmon->sel->h = selmon->sel->oldh;
            resizeclient(selmon->sel, selmon->sel->x, selmon->sel->y, selmon->sel->w, selmon->sel->h);
        }
        selmon->sel->fakefullscreen = 1;
        selmon->sel->isfullscreen = 0;
    }
    setfullscreen(selmon->sel, !selmon->sel->isfullscreen);
}