void
loadxrdb()
{
	Display *display;
	char * resm;
	XrmDatabase xrdb;
	char *type;
	XrmValue value;

	display = XOpenDisplay(NULL);

	if (display != NULL) {
		resm = XResourceManagerString(display);

		if (resm != NULL) {
			xrdb = XrmGetStringDatabase(resm);

			if (xrdb != NULL) {
				XRDB_LOAD_COLOR("dwm.normfgcolor", normfgcolor);
				XRDB_LOAD_COLOR("dwm.normbgcolor", normbgcolor);
				XRDB_LOAD_COLOR("dwm.normbordercolor", normbordercolor);
				#if FLOAT_BORDER_COLOR_PATCH
				XRDB_LOAD_COLOR("dwm.normfloatcolor", normfloatcolor);
				#endif // FLOAT_BORDER_COLOR_PATCH

				XRDB_LOAD_COLOR("dwm.selfgcolor", selfgcolor);
				XRDB_LOAD_COLOR("dwm.selbgcolor", selbgcolor);
				XRDB_LOAD_COLOR("dwm.selbordercolor", selbordercolor);
				#if FLOAT_BORDER_COLOR_PATCH
				XRDB_LOAD_COLOR("dwm.selfloatcolor", selfloatcolor);
				#endif // FLOAT_BORDER_COLOR_PATCH

				#if AWESOMEBAR_PATCH
				XRDB_LOAD_COLOR("dwm.hidfgcolor", hidfgcolor);
				XRDB_LOAD_COLOR("dwm.hidbgcolor", hidbgcolor);
				XRDB_LOAD_COLOR("dwm.hidbordercolor", hidbordercolor);
				#if FLOAT_BORDER_COLOR_PATCH
				XRDB_LOAD_COLOR("dwm.hidfloatcolor", hidfloatcolor);
				#endif // FLOAT_BORDER_COLOR_PATCH
				#endif // AWESOMEBAR_PATCH

				#if TITLECOLOR_PATCH
				XRDB_LOAD_COLOR("dwm.titlefgcolor", titlefgcolor);
				XRDB_LOAD_COLOR("dwm.titlebgcolor", titlebgcolor);
				XRDB_LOAD_COLOR("dwm.titlebordercolor", titlebordercolor);
				#if FLOAT_BORDER_COLOR_PATCH
				XRDB_LOAD_COLOR("dwm.titlefloatcolor", titlefloatcolor);
				#endif // FLOAT_BORDER_COLOR_PATCH
				#endif // TITLECOLOR_PATCH
			}
		}
	}

	XCloseDisplay(display);
}

void
xrdb(const Arg *arg)
{
	loadxrdb();
	int i;
	for (i = 0; i < LENGTH(colors); i++)
		scheme[i] = drw_scm_create(drw, colors[i],
		#if ALPHA_PATCH
		alphas[i],
		#endif // ALPHA_PATCH
		#if FLOAT_BORDER_COLOR_PATCH
		4
		#else
		3
		#endif // FLOAT_BORDER_COLOR_PATCH
		);
	focus(NULL);
	arrange(NULL);
}