#include <X11/extensions/XRes.h>

typedef struct SwallowDef {
	pid_t pid;
	Client *swallower;
	struct SwallowDef *next;
} SwallowDef;

static Atom swallow_atom;
static SwallowDef *swallowlist;

void
checkswallowed(Client *c) {
	pid_t pid = wintopid(c->win);

	if (pid < 0)
		return;

	SwallowDef **curr = &swallowlist;
	while (*curr) {
		if (pid == (*curr)->pid) {
			SwallowDef *matched = *curr;

			c->swallower = matched->swallower;
			copyclientpos(c, matched->swallower);

			c->next_swallowed = c->swallower->swallowed;
			c->swallower->swallowed = c;

			c->next = c->swallower->next;
			c->swallower->next = c;

			c->snext = c->swallower->snext;
			c->swallower->snext = c;

			*curr = matched->next;
			free(matched);

			return;
		}
		curr = &(*curr)->next;
	}
}

void
deleteswallower(Client *c) {
	SwallowDef **prevnext = &swallowlist;
	for(SwallowDef *sd = swallowlist; sd != NULL;) {
		if(sd->swallower == c) {
			SwallowDef *next = sd->next;
			*prevnext = next;
			free(sd);
			sd = next;
		} else {
			prevnext = &sd->next;
			sd = sd->next;
		}
	}

	Client *sw = c->swallowed;
	while(sw) {
		sw->swallower = NULL;
		Client *next = sw->next_swallowed;
 		sw->next_swallowed = NULL;
		sw = next;
	}
}

void
copyclientpos(Client *dst, Client *src) {
	dst->bw = src->bw;
	resizeclient(dst, src->x, src->y, src->w, src->h);
	dst->oldx = src->oldx;
	dst->oldy = src->oldy;
	dst->oldw = src->oldw;
	dst->oldh = src->oldh;
	dst->oldbw = src->oldbw;
	dst->oldstate = src->oldstate;
	dst->isfullscreen = src->isfullscreen;
	dst->isfloating = src->isfloating;
	dst->tags = src->tags;
	dst->mon = src->mon;
}

pid_t
wintopid(Window window) {
  XResClientIdSpec spec;
  spec.client = window;
  spec.mask = XRES_CLIENT_ID_XID;

  long count;
  XResClientIdValue *output;
  XResQueryClientIds(dpy, 1, &spec, &count, &output);

  pid_t pid = -1;

  for (int i = 0; i < count; ++i)
    if (output[i].spec.mask == XRES_CLIENT_ID_PID_MASK) {
      pid = *(pid_t *)output[i].value;
      break;
    }

  XResClientIdsDestroy(count, output);

  return pid;
}

