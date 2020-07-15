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
		#if BAR_PANGO_PATCH
		sw += drw_font_getwidth(drw, tp, True);
		#else
		sw += drw_fontset_getwidth(drw, tp);
		#endif // BAR_PANGO_PATCH
		*ts = ctmp;
		if (ctmp == '\0')
			break;
		tp = ++ts;
	}

	return sw;
}