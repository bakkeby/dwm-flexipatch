int
loadxrdbcolor(XrmDatabase xrdb, char **dest, char *resource)
{
	XrmValue value;
	char *type;
	if (XrmGetResource(xrdb, resource, NULL, &type, &value) == True) {
		if (value.addr != NULL && strnlen(value.addr, 8) == 7 && value.addr[0] == '#') {
			int i = 1;
			for (; i <= 6; i++) {
				if (value.addr[i] < 48) break;
				if (value.addr[i] > 57 && value.addr[i] < 65) break;
				if (value.addr[i] > 70 && value.addr[i] < 97) break;
				if (value.addr[i] > 102) break;
			}
			if (i == 7) {
				strncpy(*dest, value.addr, 7);
				return 1;
			}
		}
	}
	return 0;
}

void
loadxrdbconfig(XrmDatabase xrdb, char *name, enum resource_type rtype, void *dst)
{
	char *sdst = NULL;
	int *idst = NULL;
	float *fdst = NULL;

	sdst = dst;
	idst = dst;
	fdst = dst;

	char fullname[256];
	char *type;
	XrmValue ret;

	snprintf(fullname, sizeof(fullname), "%s.%s", "dwm", name);
	fullname[sizeof(fullname) - 1] = '\0';

	XrmGetResource(xrdb, fullname, "*", &type, &ret);
	if (!(ret.addr == NULL || strncmp("String", type, 64)))
	{
		switch (rtype) {
		case STRING:
			strcpy(sdst, ret.addr);
			break;
		case INTEGER:
			*idst = strtoul(ret.addr, NULL, 10);
			break;
		case FLOAT:
			*fdst = strtof(ret.addr, NULL);
			break;
		}
	}
}

void
loadxrdb()
{
	Display *display;
	char * resm;
	XrmDatabase xrdb;
	const ResourcePref *p;

	int s;
	char resource[40];
	char *pattern = "dwm.%s%scolor";

	char fg[] = "#000000";
	char bg[] = "#000000";
	char bd[] = "#000000";
	char *clrnames[] = { fg, bg, bd };

	display = XOpenDisplay(NULL);

	if (display != NULL) {
		resm = XResourceManagerString(display);

		if (resm != NULL) {
			xrdb = XrmGetStringDatabase(resm);

			if (xrdb != NULL) {

				for (s = 0; s < SchemeLast; s++) {

					/* Skip schemes that do not specify a resource string */
					if (colors[s][ColCount][0] == '\0')
						continue;

					sprintf(resource, pattern, colors[s][ColCount], "fg");
					if (!loadxrdbcolor(xrdb, &clrnames[ColFg], resource))
						strcpy(clrnames[ColFg], colors[s][ColFg]);

					sprintf(resource, pattern, colors[s][ColCount], "bg");
					if (!loadxrdbcolor(xrdb, &clrnames[ColBg], resource))
						strcpy(clrnames[ColBg], colors[s][ColBg]);

					sprintf(resource, pattern, colors[s][ColCount], "border");
					if (!loadxrdbcolor(xrdb, &clrnames[ColBorder], resource))
						strcpy(clrnames[ColBorder], colors[s][ColBorder]);

					free(scheme[s]);
					#if BAR_ALPHA_PATCH
					scheme[s] = drw_scm_create(drw, clrnames, alphas[s], ColCount);
					#else
					scheme[s] = drw_scm_create(drw, clrnames, ColCount);
					#endif // BAR_ALPHA_PATCH
				}

				#if BAR_STATUS2D_XRDB_TERMCOLORS_PATCH && BAR_STATUS2D_PATCH
				/* status2d terminal colours */
				for (s = 0; s < 16; s++) {
					sprintf(resource, "color%d", s);
					loadxrdbcolor(xrdb, &termcolor[s], resource);
				}
				#endif // BAR_STATUS2D_XRDB_TERMCOLORS_PATCH

				/* other preferences */
				for (p = resources; p < resources + LENGTH(resources); p++)
					loadxrdbconfig(xrdb, p->name, p->type, p->dst);

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
	focus(NULL);
	arrange(NULL);
}
