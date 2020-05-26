#if STATUS2D_PATCH
static char rawstext[1024];
#else
static char rawstext[256];
#endif // STATUS2D_PATCH
#if !DWMBLOCKS_PATCH
static const char statusexport[] = "export BUTTON=-;";
static int statuscmdn;
static int lastbutton;
#endif // DWMBLOCKS_PATCH

void
copyvalidchars(char *text, char *rawtext)
{
	int i = -1, j = 0;

	while (rawtext[++i]) {
		if ((unsigned char)rawtext[i] >= ' ') {
			text[j++] = rawtext[i];
		}
	}
	text[j] = '\0';
}
