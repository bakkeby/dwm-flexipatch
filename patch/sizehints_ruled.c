void
checkfloatingrules(Client *c)
{
	const char *class, *instance;
	Atom wintype;
	#if WINDOWROLERULE_PATCH
	char role[64];
	#endif // WINDOWROLERULE_PATCH
	unsigned int i;
	const Rule *r;
	XClassHint ch = { NULL, NULL };

	XGetClassHint(dpy, c->win, &ch);
	class    = ch.res_class ? ch.res_class : broken;
	instance = ch.res_name  ? ch.res_name  : broken;
	wintype  = getatomprop(c, netatom[NetWMWindowType], XA_ATOM);
	#if WINDOWROLERULE_PATCH
	gettextprop(c->win, wmatom[WMWindowRole], role, sizeof(role));
	#endif // WINDOWROLERULE_PATCH

	for (i = 0; i < LENGTH(rules); i++) {
		r = &rules[i];
		if ((!r->title || strstr(c->name, r->title))
		&& (!r->class || strstr(class, r->class))
		#if WINDOWROLERULE_PATCH
		&& (!r->role || strstr(role, r->role))
		#endif // WINDOWROLERULE_PATCH
		&& (!r->instance || strstr(instance, r->instance))
		&& (!r->wintype || wintype == XInternAtom(dpy, r->wintype, False)))
		{
			c->isfloating = r->isfloating;
		}
	}
	if (ch.res_class)
		XFree(ch.res_class);
	if (ch.res_name)
		XFree(ch.res_name);
}

