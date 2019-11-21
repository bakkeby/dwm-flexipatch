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
        selmon->sel->fakefullscreen = 1;
        selmon->sel->isfullscreen = 0;
    }
    setfullscreen(selmon->sel, !selmon->sel->isfullscreen);
}