void
reorganizetags(const Arg *arg)
{
   Client *c;
   unsigned int occ, unocc, i;
   unsigned int tagdest[NUMTAGS];

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
   arrange(selmon);
}

