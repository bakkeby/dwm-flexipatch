void
placedir(const Arg *arg)
{
	Client *s = selmon->sel, *f = NULL, *c, *next, *fprior, *sprior;

	if (!s || s->isfloating)
		return;

	unsigned int score = -1;
	unsigned int client_score;
	int dist;
	int dirweight = 20;

	next = s->next;
	if (!next)
		next = s->mon->clients;
	for (c = next; c != s; c = next) {

		next = c->next;
		if (!next)
			next = s->mon->clients;

		if (!ISVISIBLE(c)) // || HIDDEN(c)
			continue;

		switch (arg->i) {
		case 0: // left
			dist = s->x - c->x - c->w;
			client_score =
				dirweight * MIN(abs(dist), abs(dist + s->mon->ww)) +
				abs(s->y - c->y);
			break;
		case 1: // right
			dist = c->x - s->x - s->w;
			client_score =
				dirweight * MIN(abs(dist), abs(dist + s->mon->ww)) +
				abs(c->y - s->y);
			break;
		case 2: // up
			dist = s->y - c->y - c->h;
			client_score =
				dirweight * MIN(abs(dist), abs(dist + s->mon->wh)) +
				abs(s->x - c->x);
			break;
		default:
		case 3: // down
			dist = c->y - s->y - s->h;
			client_score =
				dirweight * MIN(abs(dist), abs(dist + s->mon->wh)) +
				abs(c->x - s->x);
			break;
		}

		if (((arg->i == 0 || arg->i == 2) && client_score <= score) || client_score < score) {
			score = client_score;
			f = c;
		}
	}

	if (f && f != s) {
		for (fprior = f->mon->clients; fprior && fprior->next != f; fprior = fprior->next);
		for (sprior = s->mon->clients; sprior && sprior->next != s; sprior = sprior->next);

		if (s == fprior) {
			next = f->next;
			if (sprior)
				sprior->next = f;
			else
				f->mon->clients = f;
			f->next = s;
			s->next = next;
		} else if (f == sprior) {
			next = s->next;
			if (fprior)
				fprior->next = s;
			else
				s->mon->clients = s;
			s->next = f;
			f->next = next;
		} else { // clients are not adjacent to each other
			next = f->next;
			f->next = s->next;
			s->next = next;
			if (fprior)
				fprior->next = s;
			else
				s->mon->clients = s;
			if (sprior)
				sprior->next = f;
			else
				f->mon->clients = f;
		}

		arrange(f->mon);
	}
}
