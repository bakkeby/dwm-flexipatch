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
				#if BAR_ALPHA_PATCH
				XRDB_LOAD_FLOAT("dwm.alphas.norm.fg", alphas[SchemeNorm][ColFg]);
				XRDB_LOAD_FLOAT("dwm.alphas.norm.bg", alphas[SchemeNorm][ColBg]);
				XRDB_LOAD_FLOAT("dwm.alphas.norm.border", alphas[SchemeNorm][ColBorder]);
				XRDB_LOAD_FLOAT("dwm.alphas.sel.fg", alphas[SchemeSel][ColFg]);
				XRDB_LOAD_FLOAT("dwm.alphas.sel.bg", alphas[SchemeSel][ColBg]);
				XRDB_LOAD_FLOAT("dwm.alphas.sel.border", alphas[SchemeSel][ColBorder]);
				XRDB_LOAD_FLOAT("dwm.alphas.titlenorm.fg", alphas[SchemeTitleNorm][ColFg]);
				XRDB_LOAD_FLOAT("dwm.alphas.titlenorm.bg", alphas[SchemeTitleNorm][ColBg]);
				XRDB_LOAD_FLOAT("dwm.alphas.titlenorm.border", alphas[SchemeTitleNorm][ColBorder]);
				XRDB_LOAD_FLOAT("dwm.alphas.titlesel.fg", alphas[SchemeTitleSel][ColFg]);
				XRDB_LOAD_FLOAT("dwm.alphas.titlesel.bg", alphas[SchemeTitleSel][ColBg]);
				XRDB_LOAD_FLOAT("dwm.alphas.titlesel.border", alphas[SchemeTitleSel][ColBorder]);
				XRDB_LOAD_FLOAT("dwm.alphas.tagsnorm.fg", alphas[SchemeTagsNorm][ColFg]);
				XRDB_LOAD_FLOAT("dwm.alphas.tagsnorm.bg", alphas[SchemeTagsNorm][ColBg]);
				XRDB_LOAD_FLOAT("dwm.alphas.tagsnorm.border", alphas[SchemeTagsNorm][ColBorder]);
				XRDB_LOAD_FLOAT("dwm.alphas.tagssel.fg", alphas[SchemeTagsSel][ColFg]);
				XRDB_LOAD_FLOAT("dwm.alphas.tagssel.bg", alphas[SchemeTagsSel][ColBg]);
				XRDB_LOAD_FLOAT("dwm.alphas.tagssel.border", alphas[SchemeTagsSel][ColBorder]);
				XRDB_LOAD_FLOAT("dwm.alphas.hidnorm.fg", alphas[SchemeHidNorm][ColFg]);
				XRDB_LOAD_FLOAT("dwm.alphas.hidnorm.bg", alphas[SchemeHidNorm][ColBg]);
				XRDB_LOAD_FLOAT("dwm.alphas.hidnorm.border", alphas[SchemeHidNorm][ColBorder]);
				XRDB_LOAD_FLOAT("dwm.alphas.hidsel.fg", alphas[SchemeHidSel][ColFg]);
				XRDB_LOAD_FLOAT("dwm.alphas.hidsel.bg", alphas[SchemeHidSel][ColBg]);
				XRDB_LOAD_FLOAT("dwm.alphas.hidsel.border", alphas[SchemeHidSel][ColBorder]);
				XRDB_LOAD_FLOAT("dwm.alphas.urg.fg", alphas[SchemeUrg][ColFg]);
				XRDB_LOAD_FLOAT("dwm.alphas.urg.bg", alphas[SchemeUrg][ColBg]);
				XRDB_LOAD_FLOAT("dwm.alphas.urg.border", alphas[SchemeUrg][ColBorder]);
				#if RENAMED_SCRATCHPADS_PATCH
				XRDB_LOAD_FLOAT("dwm.alphas.scratchsel.fg", alphas[SchemeScratchSel][ColFg]);
				XRDB_LOAD_FLOAT("dwm.alphas.scratchsel.bg", alphas[SchemeScratchSel][ColBg]);
				XRDB_LOAD_FLOAT("dwm.alphas.scratchsel.border", alphas[SchemeScratchSel][ColBorder]);
				XRDB_LOAD_FLOAT("dwm.alphas.scratchnorm.fg", alphas[SchemeScratchNorm][ColFg]);
				XRDB_LOAD_FLOAT("dwm.alphas.scratchnorm.bg", alphas[SchemeScratchNorm][ColBg]);
				XRDB_LOAD_FLOAT("dwm.alphas.scratchnorm.border", alphas[SchemeScratchNorm][ColBorder]);
				#endif // RENAMED_SCRATCHPADS_PATCH
				#if BAR_FLEXWINTITLE_PATCH
				XRDB_LOAD_FLOAT("dwm.alphas.flexactttb.fg", alphas[SchemeFlexActTTB][ColFg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexactttb.bg", alphas[SchemeFlexActTTB][ColBg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexactttb.border", alphas[SchemeFlexActTTB][ColBorder]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexactltr.fg", alphas[SchemeFlexActLTR][ColFg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexactltr.bg", alphas[SchemeFlexActLTR][ColBg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexactltr.border", alphas[SchemeFlexActLTR][ColBorder]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexactmono.fg", alphas[SchemeFlexActMONO][ColFg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexactmono.bg", alphas[SchemeFlexActMONO][ColBg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexactmono.border", alphas[SchemeFlexActMONO][ColBorder]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexactgrid.fg", alphas[SchemeFlexActGRID][ColFg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexactgrid.bg", alphas[SchemeFlexActGRID][ColBg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexactgrid.border", alphas[SchemeFlexActGRID][ColBorder]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexactgrd1.fg", alphas[SchemeFlexActGRD1][ColFg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexactgrd1.bg", alphas[SchemeFlexActGRD1][ColBg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexactgrd1.border", alphas[SchemeFlexActGRD1][ColBorder]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexactgrd2.fg", alphas[SchemeFlexActGRD2][ColFg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexactgrd2.bg", alphas[SchemeFlexActGRD2][ColBg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexactgrd2.border", alphas[SchemeFlexActGRD2][ColBorder]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexactgrdm.fg", alphas[SchemeFlexActGRDM][ColFg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexactgrdm.bg", alphas[SchemeFlexActGRDM][ColBg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexactgrdm.border", alphas[SchemeFlexActGRDM][ColBorder]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexacthgrd.fg", alphas[SchemeFlexActHGRD][ColFg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexacthgrd.bg", alphas[SchemeFlexActHGRD][ColBg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexacthgrd.border", alphas[SchemeFlexActHGRD][ColBorder]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexactdwdl.fg", alphas[SchemeFlexActDWDL][ColFg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexactdwdl.bg", alphas[SchemeFlexActDWDL][ColBg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexactdwdl.border", alphas[SchemeFlexActDWDL][ColBorder]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexactsprl.fg", alphas[SchemeFlexActSPRL][ColFg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexactsprl.bg", alphas[SchemeFlexActSPRL][ColBg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexactsprl.border", alphas[SchemeFlexActSPRL][ColBorder]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexactfloat.fg", alphas[SchemeFlexActFloat][ColFg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexactfloat.bg", alphas[SchemeFlexActFloat][ColBg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexactfloat.border", alphas[SchemeFlexActFloat][ColBorder]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexinattb.fg", alphas[SchemeFlexInaTTB][ColFg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexinattb.bg", alphas[SchemeFlexInaTTB][ColBg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexinattb.border", alphas[SchemeFlexInaTTB][ColBorder]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexinaltr.fg", alphas[SchemeFlexInaLTR][ColFg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexinaltr.bg", alphas[SchemeFlexInaLTR][ColBg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexinaltr.border", alphas[SchemeFlexInaLTR][ColBorder]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexinamono.fg", alphas[SchemeFlexInaMONO][ColFg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexinamono.bg", alphas[SchemeFlexInaMONO][ColBg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexinamono.border", alphas[SchemeFlexInaMONO][ColBorder]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexinagrid.fg", alphas[SchemeFlexInaGRID][ColFg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexinagrid.bg", alphas[SchemeFlexInaGRID][ColBg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexinagrid.border", alphas[SchemeFlexInaGRID][ColBorder]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexinagrd1.fg", alphas[SchemeFlexInaGRD1][ColFg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexinagrd1.bg", alphas[SchemeFlexInaGRD1][ColBg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexinagrd1.border", alphas[SchemeFlexInaGRD1][ColBorder]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexinagrd2.fg", alphas[SchemeFlexInaGRD2][ColFg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexinagrd2.bg", alphas[SchemeFlexInaGRD2][ColBg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexinagrd2.border", alphas[SchemeFlexInaGRD2][ColBorder]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexinagrdm.fg", alphas[SchemeFlexInaGRDM][ColFg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexinagrdm.bg", alphas[SchemeFlexInaGRDM][ColBg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexinagrdm.border", alphas[SchemeFlexInaGRDM][ColBorder]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexinahgrd.fg", alphas[SchemeFlexInaHGRD][ColFg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexinahgrd.bg", alphas[SchemeFlexInaHGRD][ColBg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexinahgrd.border", alphas[SchemeFlexInaHGRD][ColBorder]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexinadwdl.fg", alphas[SchemeFlexInaDWDL][ColFg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexinadwdl.bg", alphas[SchemeFlexInaDWDL][ColBg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexinadwdl.border", alphas[SchemeFlexInaDWDL][ColBorder]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexinasprl.fg", alphas[SchemeFlexInaSPRL][ColFg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexinasprl.bg", alphas[SchemeFlexInaSPRL][ColBg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexinasprl.border", alphas[SchemeFlexInaSPRL][ColBorder]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexinafloat.fg", alphas[SchemeFlexInaFloat][ColFg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexinafloat.bg", alphas[SchemeFlexInaFloat][ColBg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexinafloat.border", alphas[SchemeFlexInaFloat][ColBorder]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexselttb.fg", alphas[SchemeFlexSelTTB][ColFg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexselttb.bg", alphas[SchemeFlexSelTTB][ColBg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexselttb.border", alphas[SchemeFlexSelTTB][ColBorder]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexselltr.fg", alphas[SchemeFlexSelLTR][ColFg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexselltr.bg", alphas[SchemeFlexSelLTR][ColBg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexselltr.border", alphas[SchemeFlexSelLTR][ColBorder]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexselmono.fg", alphas[SchemeFlexSelMONO][ColFg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexselmono.bg", alphas[SchemeFlexSelMONO][ColBg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexselmono.border", alphas[SchemeFlexSelMONO][ColBorder]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexselgrid.fg", alphas[SchemeFlexSelGRID][ColFg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexselgrid.bg", alphas[SchemeFlexSelGRID][ColBg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexselgrid.border", alphas[SchemeFlexSelGRID][ColBorder]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexselgrd1.fg", alphas[SchemeFlexSelGRD1][ColFg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexselgrd1.bg", alphas[SchemeFlexSelGRD1][ColBg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexselgrd1.border", alphas[SchemeFlexSelGRD1][ColBorder]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexselgrd2.fg", alphas[SchemeFlexSelGRD2][ColFg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexselgrd2.bg", alphas[SchemeFlexSelGRD2][ColBg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexselgrd2.border", alphas[SchemeFlexSelGRD2][ColBorder]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexselgrdm.fg", alphas[SchemeFlexSelGRDM][ColFg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexselgrdm.bg", alphas[SchemeFlexSelGRDM][ColBg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexselgrdm.border", alphas[SchemeFlexSelGRDM][ColBorder]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexselhgrd.fg", alphas[SchemeFlexSelHGRD][ColFg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexselhgrd.bg", alphas[SchemeFlexSelHGRD][ColBg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexselhgrd.border", alphas[SchemeFlexSelHGRD][ColBorder]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexseldwdl.fg", alphas[SchemeFlexSelDWDL][ColFg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexseldwdl.bg", alphas[SchemeFlexSelDWDL][ColBg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexseldwdl.border", alphas[SchemeFlexSelDWDL][ColBorder]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexselsprl.fg", alphas[SchemeFlexSelSPRL][ColFg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexselsprl.bg", alphas[SchemeFlexSelSPRL][ColBg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexselsprl.border", alphas[SchemeFlexSelSPRL][ColBorder]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexselfloat.fg", alphas[SchemeFlexSelFloat][ColFg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexselfloat.bg", alphas[SchemeFlexSelFloat][ColBg]);
				XRDB_LOAD_FLOAT("dwm.alphas.flexselfloat.border", alphas[SchemeFlexSelFloat][ColBorder]);
				#endif // BAR_FLEXWINTITLE_PATCH
				#endif // BAR_ALPHA_PATCH
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
				XRDB_LOAD_COLOR("dwm.hidnormfgcolor", hidnormfgcolor);
				XRDB_LOAD_COLOR("dwm.hidnormbgcolor", hidnormbgcolor);
				XRDB_LOAD_COLOR("dwm.hidselfgcolor", hidselfgcolor);
				XRDB_LOAD_COLOR("dwm.hidselbgcolor", hidselbgcolor);
				XRDB_LOAD_COLOR("dwm.urgfgcolor", urgfgcolor);
				XRDB_LOAD_COLOR("dwm.urgbgcolor", urgbgcolor);
				XRDB_LOAD_COLOR("dwm.urgbordercolor", urgbordercolor);
				XRDB_LOAD_COLOR("dwm.urgfloatcolor", urgfloatcolor);
				#if RENAMED_SCRATCHPADS_PATCH
				XRDB_LOAD_COLOR("dwm.scratchselfgcolor", scratchselfgcolor);
				XRDB_LOAD_COLOR("dwm.scratchselbgcolor", scratchselbgcolor);
				XRDB_LOAD_COLOR("dwm.scratchselbordercolor", scratchselbordercolor);
				XRDB_LOAD_COLOR("dwm.scratchselfloatcolor", scratchselfloatcolor);
				XRDB_LOAD_COLOR("dwm.scratchnormfgcolor", scratchnormfgcolor);
				XRDB_LOAD_COLOR("dwm.scratchnormbgcolor", scratchnormbgcolor);
				XRDB_LOAD_COLOR("dwm.scratchnormbordercolor", scratchnormbordercolor);
				XRDB_LOAD_COLOR("dwm.scratchnormfloatcolor", scratchnormfloatcolor);
				#endif // RENAMED_SCRATCHPADS_PATCH
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
				#if BAR_STATUS2D_XRDB_TERMCOLORS_PATCH && BAR_STATUS2D_PATCH
				XRDB_LOAD_COLOR("color0", termcol0);
				XRDB_LOAD_COLOR("color1", termcol1);
				XRDB_LOAD_COLOR("color2", termcol2);
				XRDB_LOAD_COLOR("color3", termcol3);
				XRDB_LOAD_COLOR("color4", termcol4);
				XRDB_LOAD_COLOR("color5", termcol5);
				XRDB_LOAD_COLOR("color6", termcol6);
				XRDB_LOAD_COLOR("color7", termcol7);
				XRDB_LOAD_COLOR("color8", termcol8);
				XRDB_LOAD_COLOR("color9", termcol9);
				XRDB_LOAD_COLOR("color10", termcol10);
				XRDB_LOAD_COLOR("color11", termcol11);
				XRDB_LOAD_COLOR("color12", termcol12);
				XRDB_LOAD_COLOR("color13", termcol13);
				XRDB_LOAD_COLOR("color14", termcol14);
				XRDB_LOAD_COLOR("color15", termcol15);
				#endif // BAR_STATUS2D_XRDB_TERMCOLORS_PATCH

				XrmDestroyDatabase(xrdb);
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
	#if BAR_SYSTRAY_PATCH && !BAR_ALPHA_PATCH
	if (systray) {
		XMoveWindow(dpy, systray->win, -32000, -32000);
	}
	#endif // BAR_SYSTRAY_PATCH
	arrange(NULL);
	focus(NULL);
}
