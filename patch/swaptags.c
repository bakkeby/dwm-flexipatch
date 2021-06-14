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

   selmon->tagset[selmon->seltags] = newtag;

   focus(NULL);
   arrange(selmon);
}

