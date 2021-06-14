/* function implementations */
void
clearcmd(const Arg *arg)
{
	unsigned int i;

	for (i = 0; i < LENGTH(cmdkeysym); i++) {
		cmdkeysym[i] = 0;
		cmdmod[i] = 0;
	}
}

void
grabkeys(void)
{
	if (keymode == INSERTMODE) {
		grabdefkeys();
	} else if (keymode == COMMANDMODE) {
		XUngrabKey(dpy, AnyKey, AnyModifier, root);
		XGrabKey(dpy, AnyKey, AnyModifier, root,
			 True, GrabModeAsync, GrabModeAsync);
	}
}

int
isprotodel(Client *c)
{
	int n;
	Atom *protocols;
	int ret = 0;

	if (XGetWMProtocols(dpy, c->win, &protocols, &n)) {
		while (!ret && n--)
			ret = protocols[n] == wmatom[WMDelete];
		XFree(protocols);
	}
	return ret;
}


void
keypress(XEvent *e)
{
	unsigned int i, j;
	Arg a = {0};
	Bool ismatch = False, maybematch = False;
	KeySym keysym;
	XKeyEvent *ev;

	if (keymode == INSERTMODE)
		keydefpress(e);
	else if (keymode == COMMANDMODE) {
		ev = &e->xkey;
		keysym = XKeycodeToKeysym(dpy, (KeyCode)ev->keycode, 0);
		if (keysym < XK_Shift_L || keysym > XK_Hyper_R) {
			for (i = 0; i < LENGTH(cmdkeys); i++)
				if (keysym == cmdkeys[i].keysym
				&& CLEANMASK(cmdkeys[i].mod) == CLEANMASK(ev->state)
				&& cmdkeys[i].func) {
					cmdkeys[i].func(&(cmdkeys[i].arg));
					ismatch = True;
					break;
				}
			if (!ismatch) {
				for (j = 0; j < LENGTH(cmdkeysym); j++)
					if (cmdkeysym[j] == 0) {
						cmdkeysym[j] = keysym;
						cmdmod[j] = ev->state;
						break;
					}
				for (i = 0; i < LENGTH(commands); i++) {
					for (j = 0; j < LENGTH(cmdkeysym); j++) {
						if (cmdkeysym[j] == commands[i].keysym[j]
						&& CLEANMASK(cmdmod[j]) == CLEANMASK(commands[i].mod[j]))
							ismatch = True;
						else if (cmdkeysym[j] == 0
						&& cmdmod[j] == 0) {
							ismatch = False;
							maybematch = True;
							break;
						} else {
							ismatch = False;
							break;
						}
					}
					if (ismatch) {
						if (commands[i].func)
							commands[i].func(&(commands[i].arg));
						clearcmd(&a);
						break;
					}

				}
				if (!maybematch)
					clearcmd(&a);
			}
		}
	}
}

void
onlyclient(const Arg *arg)
{
	Client *c;
	XEvent ev;

	if (!selmon->sel)
		return;
	for (c = selmon->clients; c; c = c->next) {
		if (c != selmon->sel && ISVISIBLE(c)) {
			if (isprotodel(c)) {
				ev.type = ClientMessage;
				ev.xclient.window = c->win;
				ev.xclient.message_type = wmatom[WMProtocols];
				ev.xclient.format = 32;
				ev.xclient.data.l[0] = wmatom[WMDelete];
				ev.xclient.data.l[1] = CurrentTime;
				XSendEvent(dpy, c->win, False, NoEventMask, &ev);
			}
			else {
				XGrabServer(dpy);
				XSetErrorHandler(xerrordummy);
				XSetCloseDownMode(dpy, DestroyAll);
				XKillClient(dpy, c->win);
				XSync(dpy, False);
				XSetErrorHandler(xerror);
				XUngrabServer(dpy);
			}
		}
	}
}

void
setkeymode(const Arg *arg)
{
	Arg a = {0};

	if (!arg)
		return;
	keymode = arg->ui;
	clearcmd(&a);
	grabkeys();
}

