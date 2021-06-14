int
textw_wosc(char *s)
{
	char *ts = s;
	char *tp = s;
	int sw = 0;
	char ctmp;
	while (1) {
		if ((unsigned int)*ts > LENGTH(colors)) {
			ts++;
			continue;
		}
		ctmp = *ts;
		*ts = '\0';
		sw += drw_fontset_getwidth(drw, tp, True);
		*ts = ctmp;
		if (ctmp == '\0')
			break;
		tp = ++ts;
	}

	return sw;
}

