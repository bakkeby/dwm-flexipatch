#if VANITYGAPS_PATCH
void
horizgrid(Monitor *m) {
	Client *c;
	unsigned int n, i;
	int w = 0, oh, ov, ih, iv;
	int ntop, nbottom = 0;

	/* Count windows */
	getgaps(m, &oh, &ov, &ih, &iv, &n);

	if (n == 0)
		return;

	if (n == 1) { /* Just fill the whole screen */
		c = nexttiled(m->clients);
		resize(c, m->wx + ov, m->wy + oh, m->ww - 2*ov - (2*c->bw), m->wh - 2*oh - (2*c->bw), False);
	} else if (n == 2) { /* Split vertically */
		w = (m->ww - 2*ov - iv) / 2;
		c = nexttiled(m->clients);
		resize(c, m->wx + ov, m->wy + oh, w - (2*c->bw), m->wh - 2*oh - (2*c->bw), False);
		c = nexttiled(c->next);
		resize(c, m->wx + ov + w + iv, m->wy + oh, w - (2*c->bw), m->wh - 2*oh - (2*c->bw), False);
	} else {
		ntop = n / 2;
		nbottom = n - ntop;
		for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++) {
			if (i < ntop)
				resize(
					c,
					m->wx + ov + i * ((m->ww - 2*ov - iv*(ntop - 1)) / ntop + iv),
					m->wy + oh,
					(m->ww - 2*ov - iv*(ntop - 1)) / ntop - (2*c->bw),
					(m->wh - 2*oh - ih) / 2 - (2*c->bw),
					False
				);
			else
				resize(
					c,
					m->wx + ov + (i - ntop) * ((m->ww - 2*ov - iv*(nbottom - 1)) / nbottom + iv),
					m->wy + oh + ih + (m->wh - 2*oh - ih) / 2,
					(m->ww - 2*ov - iv*(nbottom - 1)) / nbottom - (2*c->bw),
					(m->wh - 2*oh - ih) / 2 - (2*c->bw),
					False
				);
		}
	}
}
#else
void
horizgrid(Monitor *m) {
	Client *c;
	unsigned int n, i;
	int w = 0;
	int ntop, nbottom = 0;

	/* Count windows */
	for(n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);

	if(n == 0)
		return;
	else if(n == 1) { /* Just fill the whole screen */
		c = nexttiled(m->clients);
		resize(c, m->wx, m->wy, m->ww - (2*c->bw), m->wh - (2*c->bw), False);
	} else if(n == 2) { /* Split vertically */
		w = m->ww / 2;
		c = nexttiled(m->clients);
		resize(c, m->wx, m->wy, w - (2*c->bw), m->wh - (2*c->bw), False);
		c = nexttiled(c->next);
		resize(c, m->wx + w, m->wy, w - (2*c->bw), m->wh - (2*c->bw), False);
	} else {
		ntop = n / 2;
		nbottom = n - ntop;
		for(i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++) {
			if(i < ntop)
				resize(c, m->wx + i * m->ww / ntop, m->wy, m->ww / ntop - (2*c->bw), m->wh / 2 - (2*c->bw), False);
			else
				resize(c, m->wx + (i - ntop) * m->ww / nbottom, m->wy + m->wh / 2, m->ww / nbottom - (2*c->bw), m->wh / 2 - (2*c->bw), False);
		}
	}
}
#endif