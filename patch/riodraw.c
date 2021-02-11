/* drag out an area using slop and resize the selected window to it. */
void
riodraw(const Arg *arg)
{
	char str[100];
	int i;
	char strout[100];
	int dimensions[4];
	int width, height, x, y;
	char tmpstring[30] = {0};
	char slopcmd[100] = "slop -f x%xx%yx%wx%hx ";
	int firstchar = 0;
	int counter = 0;
	Monitor *m;
	Client *c;

	if (!selmon->sel)
		return;
	strcat(slopcmd, slopstyle);
	FILE *fp = popen(slopcmd, "r");

	while (fgets(str, 100, fp) != NULL)
		strcat(strout, str);

	pclose(fp);

	if (strlen(strout) < 6)
		return;

	for (i = 0; i < strlen(strout); i++){
		if (!firstchar) {
			if (strout[i] == 'x')
				firstchar = 1;
			continue;
		}

		if (strout[i] != 'x')
			tmpstring[strlen(tmpstring)] = strout[i];
		else {
			dimensions[counter] = atoi(tmpstring);
			counter++;
			memset(tmpstring,0,strlen(tmpstring));
		}
	}

	x = dimensions[0];
	y = dimensions[1];
	width = dimensions[2];
	height = dimensions[3];

	if (!selmon->sel)
		return;

	c = selmon->sel;

	if (width > 50 && height > 50 && x > -40 && y > -40 && width < selmon->mw + 40 && height < selmon->mh + 40 &&
			(abs(c->w - width) > 20 || abs(c->h - height) > 20 || abs(c->x - x) > 20 || abs(c->y - y) > 20)) {
		if ((m = recttomon(x, y, width, height)) != selmon) {
			sendmon(c, m);
			unfocus(selmon->sel, 0, NULL);
			selmon = m;
			focus(NULL);
		}

		if (!c->isfloating)
			togglefloating(NULL);
		resizeclient(c, x, y, width - (c->bw * 2), height - (c->bw * 2));
		arrange(selmon);
	} else
		fprintf(stderr, "error %s", strout);
	memset(tmpstring,0,strlen(tmpstring));
}