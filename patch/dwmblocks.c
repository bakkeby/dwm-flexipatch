static int dwmblockssig;
pid_t dwmblockspid = 0;

int
getdwmblockspid()
{
	char buf[16];
	FILE *fp = popen("pidof -s dwmblocks", "r");
	if (fgets(buf, sizeof(buf), fp));
	pid_t pid = strtoul(buf, NULL, 10);
	pclose(fp);
	dwmblockspid = pid;
	return pid != 0 ? 0 : -1;
}

void
sigdwmblocks(const Arg *arg)
{
	union sigval sv;
	sv.sival_int = (dwmblockssig << 8) | arg->i;
	if (!dwmblockspid)
		if (getdwmblockspid() == -1)
			return;

	if (sigqueue(dwmblockspid, SIGUSR1, sv) == -1) {
		if (errno == ESRCH) {
			if (!getdwmblockspid())
				sigqueue(dwmblockspid, SIGUSR1, sv);
		}
	}
}