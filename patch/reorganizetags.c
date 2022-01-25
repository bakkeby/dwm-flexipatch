void
reorganizetags(const Arg *arg)
{
    Client *c;
    unsigned int occ, unocc, i;
    unsigned int tagdest[NUMTAGS];

    #if TAGSYNC_PATCH
    Monitor *origselmon = selmon;
    for (selmon = mons; selmon; selmon = selmon->next) {
    #endif // TAGSYNC_PATCH
    occ = 0;
    for (c = selmon->clients; c; c = c->next)
        occ |= (1 << (ffs(c->tags)-1));
    unocc = 0;
    for (i = 0; i < NUMTAGS; ++i) {
        while (unocc < i && (occ & (1 << unocc)))
            unocc++;
        if (occ & (1 << i)) {
            tagdest[i] = unocc;
            occ &= ~(1 << i);
            occ |= 1 << unocc;
        }
    }

    for (c = selmon->clients; c; c = c->next)
        c->tags = 1 << tagdest[ffs(c->tags)-1];
    if (selmon->sel)
        selmon->tagset[selmon->seltags] = selmon->sel->tags;
    #if TAGSYNC_PATCH
	}
	selmon = origselmon;
    arrange(NULL);
    #else
    arrange(selmon);
	#endif // TAGSYNC_PATCH
}

