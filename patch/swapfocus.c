void
swapfocus(const Arg *arg)
{
   if (!selmon->sel)
       return;
   if (selmon->pertag->prevclient[selmon->pertag->curtag] != NULL
           && ISVISIBLE(selmon->pertag->prevclient[selmon->pertag->curtag])) {
       focus(selmon->pertag->prevclient[selmon->pertag->curtag]);
       restack(selmon->pertag->prevclient[selmon->pertag->curtag]->mon);
   }
   else {
       Client *c = NULL;
       for (c = selmon->sel->next; c && !ISVISIBLE(c); c = c->next);
       if (!c)
           for (c = selmon->clients; c && !ISVISIBLE(c); c = c->next);
       if (c) {
           focus(c);
           restack(selmon);
       }
   }
}

