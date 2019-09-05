void
togglefullscreen(const Arg *arg) {
    if (!selmon->sel)
        return;

    setfullscreen(selmon->sel, !selmon->sel->isfullscreen);
}