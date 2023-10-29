char *
tagicon(Monitor *m, int tag)
{
	#if BAR_ALTTAGSDECORATION_PATCH
	Client *c;
	#endif // BAR_ALTTAGSDECORATION_PATCH
	int tagindex = tag + NUMTAGS * m->num;
	if (tagindex >= LENGTH(tagicons[DEFAULT_TAGS]))
		tagindex = tagindex % LENGTH(tagicons[DEFAULT_TAGS]);
	#if BAR_ALTTAGSDECORATION_PATCH
	for (c = m->clients; c && (!(c->tags & 1 << tag) || HIDDEN(c)); c = c->next);
	if (c)
		return tagicons[ALT_TAGS_DECORATION][tagindex];
	#endif // BAR_ALTTAGSDECORATION_PATCH
	#if BAR_ALTERNATIVE_TAGS_PATCH
	if (m->alttag)
		return tagicons[ALTERNATIVE_TAGS][tagindex];
	#endif // BAR_ALTERNATIVE_TAGS_PATCH
	return tagicons[DEFAULT_TAGS][tagindex];
}

