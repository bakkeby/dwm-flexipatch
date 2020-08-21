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
				XRDB_LOAD_COLOR("dwm.normfloatcolor", normfloatcolor);
				XRDB_LOAD_COLOR("dwm.selfgcolor", selfgcolor);
				XRDB_LOAD_COLOR("dwm.selbgcolor", selbgcolor);
				XRDB_LOAD_COLOR("dwm.selbordercolor", selbordercolor);
				XRDB_LOAD_COLOR("dwm.selfloatcolor", selfloatcolor);
				XRDB_LOAD_COLOR("dwm.titlenormfgcolor", titlenormfgcolor);
				XRDB_LOAD_COLOR("dwm.titlenormbgcolor", titlenormbgcolor);
				XRDB_LOAD_COLOR("dwm.titlenormbordercolor", titlenormbordercolor);
				XRDB_LOAD_COLOR("dwm.titlenormfloatcolor", titlenormfloatcolor);
				XRDB_LOAD_COLOR("dwm.titleselfgcolor", titleselfgcolor);
				XRDB_LOAD_COLOR("dwm.titleselbgcolor", titleselbgcolor);
				XRDB_LOAD_COLOR("dwm.titleselbordercolor", titleselbordercolor);
				XRDB_LOAD_COLOR("dwm.titleselfloatcolor", titleselfloatcolor);
				XRDB_LOAD_COLOR("dwm.tagsnormfgcolor", tagsnormfgcolor);
				XRDB_LOAD_COLOR("dwm.tagsnormbgcolor", tagsnormbgcolor);
				XRDB_LOAD_COLOR("dwm.tagsnormbordercolor", tagsnormbordercolor);
				XRDB_LOAD_COLOR("dwm.tagsnormfloatcolor", tagsnormfloatcolor);
				XRDB_LOAD_COLOR("dwm.tagsselfgcolor", tagsselfgcolor);
				XRDB_LOAD_COLOR("dwm.tagsselbgcolor", tagsselbgcolor);
				XRDB_LOAD_COLOR("dwm.tagsselbordercolor", tagsselbordercolor);
				XRDB_LOAD_COLOR("dwm.tagsselfloatcolor", tagsselfloatcolor);
				XRDB_LOAD_COLOR("dwm.hidfgcolor", hidfgcolor);
				XRDB_LOAD_COLOR("dwm.hidbgcolor", hidbgcolor);
				XRDB_LOAD_COLOR("dwm.hidbordercolor", hidbordercolor);
				XRDB_LOAD_COLOR("dwm.hidfloatcolor", hidfloatcolor);
				XRDB_LOAD_COLOR("dwm.urgfgcolor", urgfgcolor);
				XRDB_LOAD_COLOR("dwm.urgbgcolor", urgbgcolor);
				XRDB_LOAD_COLOR("dwm.urgbordercolor", urgbordercolor);
				XRDB_LOAD_COLOR("dwm.urgfloatcolor", urgfloatcolor);
				#if BAR_FLEXWINTITLE_PATCH
				XRDB_LOAD_COLOR("dwm.normTTBbgcolor", normTTBbgcolor);
				XRDB_LOAD_COLOR("dwm.normLTRbgcolor", normLTRbgcolor);
				XRDB_LOAD_COLOR("dwm.normMONObgcolor", normMONObgcolor);
				XRDB_LOAD_COLOR("dwm.normGRIDbgcolor", normGRIDbgcolor);
				XRDB_LOAD_COLOR("dwm.normGRD1bgcolor", normGRD1bgcolor);
				XRDB_LOAD_COLOR("dwm.normGRD2bgcolor", normGRD2bgcolor);
				XRDB_LOAD_COLOR("dwm.normGRDMbgcolor", normGRDMbgcolor);
				XRDB_LOAD_COLOR("dwm.normHGRDbgcolor", normHGRDbgcolor);
				XRDB_LOAD_COLOR("dwm.normDWDLbgcolor", normDWDLbgcolor);
				XRDB_LOAD_COLOR("dwm.normSPRLbgcolor", normSPRLbgcolor);
				XRDB_LOAD_COLOR("dwm.normfloatbgcolor", normfloatbgcolor);
				XRDB_LOAD_COLOR("dwm.actTTBbgcolor", actTTBbgcolor);
				XRDB_LOAD_COLOR("dwm.actLTRbgcolor", actLTRbgcolor);
				XRDB_LOAD_COLOR("dwm.actMONObgcolor", actMONObgcolor);
				XRDB_LOAD_COLOR("dwm.actGRIDbgcolor", actGRIDbgcolor);
				XRDB_LOAD_COLOR("dwm.actGRD1bgcolor", actGRD1bgcolor);
				XRDB_LOAD_COLOR("dwm.actGRD2bgcolor", actGRD2bgcolor);
				XRDB_LOAD_COLOR("dwm.actGRDMbgcolor", actGRDMbgcolor);
				XRDB_LOAD_COLOR("dwm.actHGRDbgcolor", actHGRDbgcolor);
				XRDB_LOAD_COLOR("dwm.actDWDLbgcolor", actDWDLbgcolor);
				XRDB_LOAD_COLOR("dwm.actSPRLbgcolor", actSPRLbgcolor);
				XRDB_LOAD_COLOR("dwm.actfloatbgcolor", actfloatbgcolor);
				XRDB_LOAD_COLOR("dwm.selTTBbgcolor", selTTBbgcolor);
				XRDB_LOAD_COLOR("dwm.selLTRbgcolor", selLTRbgcolor);
				XRDB_LOAD_COLOR("dwm.selMONObgcolor", selMONObgcolor);
				XRDB_LOAD_COLOR("dwm.selGRIDbgcolor", selGRIDbgcolor);
				XRDB_LOAD_COLOR("dwm.selGRD1bgcolor", selGRD1bgcolor);
				XRDB_LOAD_COLOR("dwm.selGRD2bgcolor", selGRD2bgcolor);
				XRDB_LOAD_COLOR("dwm.selGRDMbgcolor", selGRDMbgcolor);
				XRDB_LOAD_COLOR("dwm.selHGRDbgcolor", selHGRDbgcolor);
				XRDB_LOAD_COLOR("dwm.selDWDLbgcolor", selDWDLbgcolor);
				XRDB_LOAD_COLOR("dwm.selSPRLbgcolor", selSPRLbgcolor);
				XRDB_LOAD_COLOR("dwm.selfloatbgcolor", selfloatbgcolor);
				#endif // BAR_FLEXWINTITLE_PATCH
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
		#if BAR_ALPHA_PATCH
		alphas[i],
		#endif // BAR_ALPHA_PATCH
		ColCount
		);
	focus(NULL);
	arrange(NULL);
}