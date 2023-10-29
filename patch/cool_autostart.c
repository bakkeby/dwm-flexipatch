/* dwm will keep pid's of processes from autostart array and kill them at quit */
static pid_t *autostart_pids;
static size_t autostart_len;

/* execute command from autostart array */
static void
autostart_exec()
{
	const char *const *p;
	struct sigaction sa;
	size_t i = 0;

	/* count entries */
	for (p = autostart; *p; autostart_len++, p++)
		while (*++p);

	autostart_pids = malloc(autostart_len * sizeof(pid_t));
	for (p = autostart; *p; i++, p++) {
		if ((autostart_pids[i] = fork()) == 0) {
			setsid();

			/* Restore SIGCHLD sighandler to default before spawning a program */
			sigemptyset(&sa.sa_mask);
			sa.sa_flags = 0;
			sa.sa_handler = SIG_DFL;
			sigaction(SIGCHLD, &sa, NULL);

			execvp(*p, (char *const *)p);
			fprintf(stderr, "dwm: execvp %s\n", *p);
			perror(" failed");
			_exit(EXIT_FAILURE);
		}
		/* skip arguments */
		while (*++p);
	}
}

