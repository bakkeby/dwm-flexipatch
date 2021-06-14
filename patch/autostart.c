void
runautostart(void)
{
	char *pathpfx;
	char *path;
	char *xdgdatahome;
	char *home;

	if ((home = getenv("HOME")) == NULL)
		/* this is almost impossible */
		return;

	/* if $XDG_DATA_HOME is defined, use $XDG_DATA_HOME/dwm,
	 * otherwise use ~/.local/share/dwm as autostart script directory
	 */
	if ((xdgdatahome = getenv("XDG_DATA_HOME")) != NULL) {
		/* space for path segments, separators and nul */
		if ((pathpfx = malloc(strlen(xdgdatahome) + strlen(dwmdir) + 2)) == NULL)
			return;

		if (sprintf(pathpfx, "%s/%s", xdgdatahome, dwmdir) <= 0) {
			free(pathpfx);
			return;
		}
	} else {
		/* space for path segments, separators and nul */
		if ((pathpfx = malloc(strlen(home) + strlen(localshare) + strlen(dwmdir) + 3)) == NULL)
			return;

		if (sprintf(pathpfx, "%s/%s/%s", home, localshare, dwmdir) < 0) {
			free(pathpfx);
			return;
		}
	}

	/* check if the autostart script directory exists */
	struct stat sb;

	if (! (stat(pathpfx, &sb) == 0 && S_ISDIR(sb.st_mode))) {
		/* the XDG conformant path does not exist or are not directories
		 * so we try ~/.dwm instead
		 */
		if (realloc(pathpfx, strlen(home) + strlen(dwmdir) + 3) == NULL) {
			free(pathpfx);
			return;
		}

		if (sprintf(pathpfx, "%s/.%s", home, dwmdir) <= 0) {
			free(pathpfx);
			return;
		}
	}

	/* try the blocking script first */
	if ((path = malloc(strlen(pathpfx) + strlen(autostartblocksh) + 2)) == NULL) {
		free(pathpfx);
		return;
	} else
		if (sprintf(path, "%s/%s", pathpfx, autostartblocksh) <= 0) {
			free(path);
			free(pathpfx);
		}

	if (access(path, X_OK) == 0)
		system(path);

	/* now the non-blocking script */
	if ((path = realloc(path, strlen(pathpfx) + strlen(autostartsh) + 4)) == NULL) {
		free(pathpfx);
		free(path);
		return;
	} else
		if (sprintf(path, "%s/%s", pathpfx, autostartsh) <= 0) {
			free(path);
			free(pathpfx);
		}

	if (access(path, X_OK) == 0) {
		system(strcat(path, " &"));
		free(pathpfx);
		free(path);
	}
}

