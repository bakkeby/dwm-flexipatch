void
get_vt_colors(void)
{
	char *cfs[3] = {
		"/sys/module/vt/parameters/default_red",
		"/sys/module/vt/parameters/default_grn",
		"/sys/module/vt/parameters/default_blu",
	};
	char vtcs[16][8];
	char tk[] = ",";
	char cl[64];
	char *tp = NULL;
	FILE *fp;
	size_t r;
	int i, c, n, len;
	for (i = 0; i < 16; i++)
		strcpy(vtcs[i], "#000000");

	for (i = 0, r = 0; i < 3; i++) {
		if ((fp = fopen(cfs[i], "r")) == NULL)
			continue;
		while ((cl[r] = fgetc(fp)) != EOF && cl[r] != '\n')
			r++;
		cl[r] = '\0';
		for (c = 0, tp = cl, n = 0; c < 16; c++, tp++) {
			if ((r = strcspn(tp, tk)) == -1)
				break;
			for (n = 0; r && *tp >= 48 && *tp < 58; r--, tp++)
				n = n * 10 - 48 + *tp;
			vtcs[c][i * 2 + 1] = n / 16 < 10 ? n / 16 + 48 : n / 16 + 87;
			vtcs[c][i * 2 + 2] = n % 16 < 10 ? n % 16 + 48 : n % 16 + 87;
		}
		fclose(fp);
	}

	len = LENGTH(colors);
	if (len > LENGTH(color_ptrs))
		len = LENGTH(color_ptrs);
	for (i = 0; i < len; i++) {
		for (c = 0; c < ColCount; c++) {
			n = color_ptrs[i][c];
			if (n > -1 && strlen(colors[i][c]) >= strlen(vtcs[n]))
				memcpy(colors[i][c], vtcs[n], 7);
		}
	}
}

int get_luminance(char *r)
{
	char *c = r;
	int n[3] = {0};
	int i = 0;

	while (*c) {
		if (*c >= 48 && *c < 58)
			n[i / 2] = n[i / 2] * 16 - 48 + *c;
		else if (*c >= 65 && *c < 71)
			n[i / 2] = n[i / 2] * 16 - 55 + *c;
		else if (*c >= 97 && *c < 103)
			n[i / 2] = n[i / 2] * 16 - 87 + *c;
		else
			i--;
		i++;
		c++;
	}

	return (0.299 * n[0] + 0.587 * n[1] + 0.114 * n[2]) / 2.55;
}

