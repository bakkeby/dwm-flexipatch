void
nametag(const Arg *arg)
{
	char *p, name[MAX_TAGLEN];
	FILE *f;
	int i, group;
	int tagindex;
	Monitor *m = selmon;
	#if BAR_ALTTAGSDECORATION_PATCH
	Client *c;
	int occ = 0;

	for (c = m->clients; c; c = c->next)
		occ |= c->tags == 255 ? 0 : c->tags;
	#endif // BAR_ALTTAGSDECORATION_PATCH

	errno = 0; // popen(3p) says on failure it "may" set errno
	if (!(f = popen(NAMETAG_COMMAND, "r"))) {
		fprintf(stderr, "dwm: popen command failed%s%s\n", errno ? ": " : "", errno ? strerror(errno) : "");
		return;
	}

	if (!(p = fgets(name, MAX_TAGLEN, f)) && (i = errno) && ferror(f))
		fprintf(stderr, "dwm: fgets failed: %s\n", strerror(i));

	pclose(f);

	if (!p)
		return;
	if ((p = strchr(name, '\n')))
		*p = '\0';

	for (i = 0; i < NUMTAGS; i++) {
		if (m->tagset[m->seltags] & (1 << i)) {

		 	tagindex = i + NUMTAGS * m->num;
			if (tagindex >= LENGTH(tagicons[DEFAULT_TAGS]))
				tagindex = tagindex % LENGTH(tagicons[DEFAULT_TAGS]);

			#if BAR_ALTTAGSDECORATION_PATCH
			if (occ & 1 << i)
				group = ALT_TAGS_DECORATION;
			else
			#endif // BAR_ALTTAGSDECORATION_PATCH
			#if BAR_ALTERNATIVE_TAGS_PATCH
			if (m->alttag)
				group = ALTERNATIVE_TAGS;
			else
			#endif // BAR_ALTERNATIVE_TAGS_PATCH
			group = DEFAULT_TAGS;

			#if NAMETAG_PREPEND_PATCH
			if (snprintf(tagicons[group][i], MAX_TAGLEN, NAMETAG_FORMAT, i+1, name) < 0)
				fprintf(stderr, "nametag: if statement to avoid -Wformat-truncation= warnings\n");
			#else
			snprintf(tagicons[group][i], MAX_TAGLEN, NAMETAG_FORMAT, name);
			#endif // NAMETAG_PREPEND_PATCH
		}
	}
	drawbars();
}
