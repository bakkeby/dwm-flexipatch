void
runAutostart(void) {

	int ret;

	ret = system("cd ~/.config/dwm; ./autostart_blocking.sh");
	ret = system("cd ~/.config/dwm; ./autostart.sh &");

	if (ret); // ignore, hide compilation warnings
}