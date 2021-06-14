void
sortscreens(XineramaScreenInfo *screens, int n)
{
       int i, j;
       XineramaScreenInfo *screen = ecalloc(1, sizeof(XineramaScreenInfo));

       for (i = 0; i < n; i++)
               for (j = i + 1; j < n; j++)
                       if (RIGHTOF(screens[i], screens[j])) {
                               memcpy(&screen[0], &screens[i], sizeof(XineramaScreenInfo));
                               memcpy(&screens[i], &screens[j], sizeof(XineramaScreenInfo));
                               memcpy(&screens[j], &screen[0], sizeof(XineramaScreenInfo));
                       }
       XFree(screen);
}

