#if BAR_STATUS2D_XRDB_TERMCOLORS_PATCH
static char termcol0[]  = "#000000"; /* black   */
static char termcol1[]  = "#ff0000"; /* red     */
static char termcol2[]  = "#33ff00"; /* green   */
static char termcol3[]  = "#ff0099"; /* yellow  */
static char termcol4[]  = "#0066ff"; /* blue    */
static char termcol5[]  = "#cc00ff"; /* magenta */
static char termcol6[]  = "#00ffff"; /* cyan    */
static char termcol7[]  = "#d0d0d0"; /* white   */
static char termcol8[]  = "#808080"; /* black   */
static char termcol9[]  = "#ff0000"; /* red     */
static char termcol10[] = "#33ff00"; /* green   */
static char termcol11[] = "#ff0099"; /* yellow  */
static char termcol12[] = "#0066ff"; /* blue    */
static char termcol13[] = "#cc00ff"; /* magenta */
static char termcol14[] = "#00ffff"; /* cyan    */
static char termcol15[] = "#ffffff"; /* white   */
static char *termcolor[] = {
	termcol0, termcol1, termcol2, termcol3, termcol4, termcol5, termcol6, termcol7,
	termcol8, termcol9, termcol10, termcol11, termcol12, termcol13, termcol14, termcol15,
};
#endif // BAR_STATUS2D_XRDB_TERMCOLORS_PATCH

int
width_status2d(Bar *bar, BarArg *a)
{
	int width;
	#if BAR_EXTRASTATUS_PATCH || BAR_STATUSCMD_PATCH
	width = status2dtextlength(rawstext);
	#else
	width = status2dtextlength(stext);
	#endif // #if BAR_EXTRASTATUS_PATCH | BAR_STATUSCMD_PATCH
	return width ? width + lrpad : 0;
}

#if BAR_EXTRASTATUS_PATCH
int
width_status2d_es(Bar *bar, BarArg *a)
{
	int width;
	#if BAR_STATUSCMD_PATCH
	width = status2dtextlength(rawestext);
	#else
	width = status2dtextlength(estext);
	#endif // BAR_STATUSCMD_PATCH
	return width ? width + lrpad : 0;
}
#endif // BAR_EXTRASTATUS_PATCH

int
draw_status2d(Bar *bar, BarArg *a)
{
	#if BAR_EXTRASTATUS_PATCH || BAR_STATUSCMD_PATCH
	return drawstatusbar(a, rawstext);
	#else
	return drawstatusbar(a, stext);
	#endif // #if BAR_EXTRASTATUS_PATCH | BAR_STATUSCMD_PATCH
}

#if BAR_EXTRASTATUS_PATCH
int
draw_status2d_es(Bar *bar, BarArg *a)
{
	#if BAR_STATUSCMD_PATCH
	return drawstatusbar(a, rawestext);
	#else
	return drawstatusbar(a, estext);
	#endif // BAR_STATUSCMD_PATCH
}
#endif // BAR_EXTRASTATUS_PATCH

#if !BAR_STATUSCMD_PATCH
int
click_status2d(Bar *bar, Arg *arg, BarArg *a)
{
	return ClkStatusText;
}
#endif // BAR_STATUSCMD_PATCH

int
drawstatusbar(BarArg *barg, char* stext)
{
	int i, w, len, s, col, tc;
	int rx, ry, rw, rh;
	unsigned int rgb, a;
	int x = barg->x;
	int y = barg->y;
	short isCode = 0;
	char buf[8];
	char *text;
	char *p;
	char c;
	Clr oldbg, oldfg, swp;
	len = strlen(stext);
	if (!(text = (char*) malloc(sizeof(char)*(len + 1))))
		die("malloc");
	p = text;
	#if BAR_STATUSCMD_PATCH
	copyvalidchars(text, stext);
	#else
	memcpy(text, stext, len);
	#endif // BAR_STATUSCMD_PATCH

	x += lrpad / 2;
	drw_setscheme(drw, scheme[LENGTH(colors)]);
	drw->scheme[ColFg] = scheme[SchemeNorm][ColFg];
	drw->scheme[ColBg] = scheme[SchemeNorm][ColBg];

	/* process status text */
	i = -1;
	while (text[++i]) {
		if (text[i] == '^' && !isCode) {
			isCode = 1;

			text[i] = '\0';
			w = TEXTWM(text) - lrpad;
			drw_text(drw, x, y, w, bh, 0, text, 0, True);

			x += w;

			/* process code */
			while (text[++i] != '^') {
				s = sscanf(text + i, "%c#%6x%2x", &c, &rgb, &a);
				switch (c) {
				case 'c':
				case 'b':
					col = c == 'c' ? ColFg : ColBg;
					switch (s) {
						case 3:
							sprintf(buf, "#%.6x", rgb);
							drw_clr_create(drw, &drw->scheme[col], buf, a);
							i += 9;
							break;
						case 2:
							sprintf(buf, "#%.6x", rgb);
							drw_clr_create(drw, &drw->scheme[col], buf, 0xff);
							i += 7;
							break;
						default:
							len = 0;
							break;
					}
					break;
				#if BAR_STATUS2D_XRDB_TERMCOLORS_PATCH
				case 'C':
				case 'B':
					col = c == 'C' ? ColFg : ColBg;
					tc = atoi(text + ++i) % 16;
					drw_clr_create(drw, &drw->scheme[col], termcolor[tc], 0xff);
					break;
				#endif // BAR_STATUS2D_XRDB_TERMCOLORS_PATCH
				case 'd':
					drw->scheme[ColFg] = scheme[SchemeNorm][ColFg];
					drw->scheme[ColBg] = scheme[SchemeNorm][ColBg];
					break;
				case 'w':
					swp = drw->scheme[ColFg];
					drw->scheme[ColFg] = drw->scheme[ColBg];
					drw->scheme[ColBg] = swp;
					break;
				case 'v':
					oldfg = drw->scheme[ColFg];
					oldbg = drw->scheme[ColBg];
					break;
				case 't':
					drw->scheme[ColFg] = oldfg;
					drw->scheme[ColBg] = oldbg;
					break;
				case 'r':
					rx = atoi(text + ++i);
					while (text[++i] != ',');
					ry = atoi(text + ++i);
					while (text[++i] != ',');
					rw = atoi(text + ++i);
					while (text[++i] != ',');
					rh = atoi(text + ++i);

					if (ry < 0)
						ry = 0;
					if (rx < 0)
						rx = 0;

					drw_rect(drw, rx + x, y + ry, rw, rh, 1, 0);
					break;
				case 'f':
					x += atoi(text + ++i);
					break;
				}
			}

			text = text + i + 1;
			len -= i + 1;
			i = -1;
			isCode = 0;
			if (len <= 0)
				break;
		}
	}
	if (!isCode && len > 0) {
		w = TEXTWM(text) - lrpad;
		drw_text(drw, x, y, w, bh, 0, text, 0, True);
		x += w;
	}
	free(p);

	drw_setscheme(drw, scheme[SchemeNorm]);
	return 1;
}

int
status2dtextlength(char* stext)
{
	int i, w, len;
	short isCode = 0;
	char *text;
	char *p;

	len = strlen(stext) + 1;
	if (!(text = (char*) malloc(sizeof(char)*len)))
		die("malloc");
	p = text;
	#if BAR_STATUSCMD_PATCH
	copyvalidchars(text, stext);
	#else
	memcpy(text, stext, len);
	#endif // BAR_STATUSCMD_PATCH

	/* compute width of the status text */
	w = 0;
	i = -1;
	while (text[++i]) {
		if (text[i] == '^') {
			if (!isCode) {
				isCode = 1;
				text[i] = '\0';
				w += TEXTWM(text) - lrpad;
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
		w += TEXTWM(text) - lrpad;
	free(p);
	return w;
}

