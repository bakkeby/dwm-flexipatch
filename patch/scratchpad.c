void
togglescratch(const Arg *arg)
{
       Client *c;
       unsigned int found = 0;
       unsigned int scratchtag = SPTAG(arg->ui);
       Arg sparg = {.v = scratchpads[arg->ui].cmd};

       for (c = selmon->clients; c && !(found = c->tags & scratchtag); c = c->next);
       if (found) {
               unsigned int newtagset = selmon->tagset[selmon->seltags] ^ scratchtag;
               if (newtagset) {
                       selmon->tagset[selmon->seltags] = newtagset;
                       focus(NULL);
                       arrange(selmon);
               }
               if (ISVISIBLE(c)) {
                       focus(c);
                       restack(selmon);
               }
       } else {
               selmon->tagset[selmon->seltags] |= scratchtag;
               spawn(&sparg);
       }
}