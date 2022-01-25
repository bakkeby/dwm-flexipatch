void
swaptags(const Arg *arg)
{
    unsigned int newtag = arg->ui & TAGMASK;
    unsigned int curtag = selmon->tagset[selmon->seltags];

    if (newtag == curtag || !curtag || (curtag & (curtag-1)))
        return;

    for (Client *c = selmon->clients; c != NULL; c = c->next) {
        if ((c->tags & newtag) || (c->tags & curtag))
            c->tags ^= curtag ^ newtag;

        if (!c->tags)
                c->tags = newtag;
    }

    Monitor *m;
    for (m = mons; m; m = m->next)
        m->tagset[m->seltags] = newtag;

    focus(NULL);
    #if TAGSYNC_PATCH
    arrange(NULL);
    #else
    arrange(selmon);
    #endif // TAGSYNC_PATCH
}

