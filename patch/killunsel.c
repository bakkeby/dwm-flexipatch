void
killunsel(const Arg *arg)
{
	Client *i = NULL;

	if (!selmon->sel)
		return;

	for (i = selmon->clients; i; i = i->next) {
		if (ISVISIBLE(i) && i != selmon->sel) {
			#if BAR_SYSTRAY_PATCH
			if (!sendevent(i->win, wmatom[WMDelete], NoEventMask, wmatom[WMDelete], CurrentTime, 0, 0, 0))
			#else
			if (!sendevent(i, wmatom[WMDelete]))
			#endif // BAR_SYSTRAY_PATCH
			{
				XGrabServer(dpy);
				XSetErrorHandler(xerrordummy);
				XSetCloseDownMode(dpy, DestroyAll);
				XKillClient(dpy, i->win);
				XSync(dpy, False);
				XSetErrorHandler(xerror);
				XUngrabServer(dpy);
			}
		}
	}
}

