void
unfloatvisible(const Arg *arg)
{
    Client *c;

    for (c = selmon->clients; c; c = c->next)
        if (ISVISIBLE(c) && c->isfloating)
            c->isfloating = c->isfixed;

    if (arg && arg->v)
        setlayout(arg);
    else
        arrange(selmon);
}

