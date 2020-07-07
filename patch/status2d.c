int
drawstatusbar(Monitor *m, int bh, char* stext, int stw, int stp, int align)
{
	int ret, i, w, x, len;
	short isCode = 0;
	char *text;
	char *p;

	len = strlen(stext) + 1;
	if (!(text = (char*) malloc(sizeof(char)*len)))
		die("malloc");
	p = text;
	#if STATUSCMD_PATCH
	copyvalidchars(text, stext);
	#else
	memcpy(text, stext, len);
	#endif // STATUSCMD_PATCH

	/* compute width of the status text */
	w = stp * 2;
	i = -1;
	while (text[++i]) {
		if (text[i] == '^') {
			if (!isCode) {
				isCode = 1;
				text[i] = '\0';
				#if PANGO_PATCH
				w += TEXTWM(text) - lrpad;
				#else
				w += TEXTW(text) - lrpad;
				#endif // PANGO_PATCH
				text[i] = '^';
				if (text[++i] == 'f')
					w += atoi(text + ++i);
			} else {
				isCode = 0;
				text = text + i + 1;
				i = -1;
			}
		}
	}
	if (!isCode)
		#if PANGO_PATCH
		w += TEXTWM(text) - lrpad;
		#else
		w += TEXTW(text) - lrpad;
		#endif // PANGO_PATCH
	else
		isCode = 0;
	text = p;
	w += 2; /* 1px padding on both sides */
	if (align == 0)
		x = 0 + stp; // left
	else if (align == 1)
		x = m->ww - w - stw + stp; // right
	else
		x = m->ww / 2 - w / 2; // center
	ret = w;

	drw_setscheme(drw, scheme[LENGTH(colors)]);
	drw->scheme[ColFg] = scheme[SchemeNorm][ColFg];
	drw->scheme[ColBg] = scheme[SchemeNorm][ColBg];
	drw_rect(drw, x - stp - 1, 0, w, bh, 1, 1);
	x++;

	/* process status text */
	i = -1;
	while (text[++i]) {
		if (text[i] == '^' && !isCode) {
			isCode = 1;

			text[i] = '\0';
			#if PANGO_PATCH
			w = TEXTWM(text) - lrpad;
			drw_text(drw, x, 0, w, bh, 0, text, 0, True);
			#else
			w = TEXTW(text) - lrpad;
			drw_text(drw, x, 0, w, bh, 0, text, 0);
			#endif // PANGO_PATCH

			x += w;

			/* process code */
			while (text[++i] != '^') {
				if (text[i] == 'c') {
					char buf[8];
					if (i + 7 > len) {
						i += 7;
						break;
					}
					memcpy(buf, (char*)text+i+1, 7);
					buf[7] = '\0';
					#if ALPHA_PATCH && STATUS2D_NO_ALPHA_PATCH
					drw_clr_create(drw, &drw->scheme[ColFg], buf, 0xff);
					#elif ALPHA_PATCH
					drw_clr_create(drw, &drw->scheme[ColFg], buf, alphas[SchemeNorm][ColFg]);
					#else
					drw_clr_create(drw, &drw->scheme[ColFg], buf);
					#endif // ALPHA_PATCH
					i += 7;
				} else if (text[i] == 'b') {
					char buf[8];
					if (i + 7 > len) {
						i += 7;
						break;
					}
					memcpy(buf, (char*)text+i+1, 7);
					buf[7] = '\0';
					#if ALPHA_PATCH && STATUS2D_NO_ALPHA_PATCH
					drw_clr_create(drw, &drw->scheme[ColBg], buf, 0xff);
					#elif ALPHA_PATCH
					drw_clr_create(drw, &drw->scheme[ColBg], buf, alphas[SchemeNorm][ColBg]);
					#else
					drw_clr_create(drw, &drw->scheme[ColBg], buf);
					#endif // ALPHA_PATCH
					i += 7;
				} else if (text[i] == 'd') {
					drw->scheme[ColFg] = scheme[SchemeNorm][ColFg];
					drw->scheme[ColBg] = scheme[SchemeNorm][ColBg];
				} else if (text[i] == 'r') {
					int rx = atoi(text + ++i);
					while (text[++i] != ',');
					int ry = atoi(text + ++i);
					while (text[++i] != ',');
					int rw = atoi(text + ++i);
					while (text[++i] != ',');
					int rh = atoi(text + ++i);

					if (ry < 0)
						ry = 0;
					if (rx < 0)
						rx = 0;

					drw_rect(drw, rx + x, ry, rw, rh, 1, 0);
				} else if (text[i] == 'f') {
					x += atoi(text + ++i);
				}
			}

			text = text + i + 1;
			i=-1;
			isCode = 0;
		}
	}

	if (!isCode) {
		#if PANGO_PATCH
		w = TEXTWM(text) - lrpad;
		drw_text(drw, x, 0, w, bh, 0, text, 0, True);
		#else
		w = TEXTW(text) - lrpad;
		drw_text(drw, x, 0, w, bh, 0, text, 0);
		#endif // PANGO_PATCH
	}

	drw_setscheme(drw, scheme[SchemeNorm]);
	free(p);

	return ret;
}

int
status2dtextlength(char* stext)
{
	int i, w, len;
	short isCode = 0;
	char *text;

	len = strlen(stext) + 1;
	if (!(text = (char*) malloc(sizeof(char)*len)))
		die("malloc");

	#if STATUSCMD_PATCH
	copyvalidchars(text, stext);
	#else
	memcpy(text, stext, len);
	#endif // STATUSCMD_PATCH

	/* compute width of the status text */
	w = 0;
	i = -1;
	while (text[++i]) {
		if (text[i] == '^') {
			if (!isCode) {
				isCode = 1;
				text[i] = '\0';
				#if PANGO_PATCH
				w += TEXTWM(text) - lrpad;
				#else
				w += TEXTW(text) - lrpad;
				#endif // PANGO_PATCH
				text[i] = '^';
				if (text[++i] == 'f')
					w += atoi(text + ++i);
			} else {
				isCode = 0;
				text = text + i + 1;
				i = -1;
			}
		}
	}
	if (!isCode)
		#if PANGO_PATCH
		w += TEXTWM(text) - lrpad;
		#else
		w += TEXTW(text) - lrpad;
		#endif // PANGO_PATCH
	return w;
}
