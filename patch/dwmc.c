void
setlayoutex(const Arg *arg)
{
	setlayout(&((Arg) { .v = &layouts[arg->i] }));
}

void
viewex(const Arg *arg)
{
	view(&((Arg) { .ui = 1 << arg->ui }));
}

void
viewallex(const Arg *arg)
{
	#if SCRATCHPADS_PATCH
	view(&((Arg){.ui = ~SPTAGMASK}));
	#else
	view(&((Arg){.ui = ~0}));
	#endif // SCRATCHPADS_PATCH
}

void
toggleviewex(const Arg *arg)
{
	toggleview(&((Arg) { .ui = 1 << arg->ui }));
}

void
tagex(const Arg *arg)
{
	tag(&((Arg) { .ui = 1 << arg->ui }));
}

void
toggletagex(const Arg *arg)
{
	toggletag(&((Arg) { .ui = 1 << arg->ui }));
}

void
tagallex(const Arg *arg)
{
	#if SCRATCHPADS_PATCH
	tag(&((Arg){.ui = ~SPTAGMASK}));
	#else
	tag(&((Arg){.ui = ~0}));
	#endif // SCRATCHPADS_PATCH
}

int
fake_signal(void)
{
	char fsignal[256];
	char indicator[9] = "fsignal:";
	char str_sig[50];
	char param[16];
	int i, len_str_sig, n, paramn;
	size_t len_fsignal, len_indicator = strlen(indicator);
	Arg arg;

	// Get root name property
	if (gettextprop(root, XA_WM_NAME, fsignal, sizeof(fsignal))) {
		len_fsignal = strlen(fsignal);

		// Check if this is indeed a fake signal
		if (len_indicator > len_fsignal ? 0 : strncmp(indicator, fsignal, len_indicator) == 0) {
			paramn = sscanf(fsignal+len_indicator, "%s%n%s%n", str_sig, &len_str_sig, param, &n);

			if (paramn == 1) arg = (Arg) {0};
			else if (paramn > 2) return 1;
			else if (strncmp(param, "i", n - len_str_sig) == 0)
				#if IPC_PATCH
				sscanf(fsignal + len_indicator + n, "%li", &(arg.i));
				#else
				sscanf(fsignal + len_indicator + n, "%i", &(arg.i));
				#endif // IPC_PATCH
			else if (strncmp(param, "ui", n - len_str_sig) == 0)
				#if IPC_PATCH
				sscanf(fsignal + len_indicator + n, "%lu", &(arg.ui));
				#else
				sscanf(fsignal + len_indicator + n, "%u", &(arg.ui));
				#endif // IPC_PATCH
			else if (strncmp(param, "f", n - len_str_sig) == 0)
				sscanf(fsignal + len_indicator + n, "%f", &(arg.f));
			else return 1;

			// Check if a signal was found, and if so handle it
			for (i = 0; i < LENGTH(signals); i++)
				if (strncmp(str_sig, signals[i].sig, len_str_sig) == 0 && signals[i].func)
					signals[i].func(&(arg));

			// A fake signal was sent
			return 1;
		}
	}

	// No fake signal was sent, so proceed with update
	return 0;
}

