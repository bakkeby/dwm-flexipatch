void
cyclelayout(const Arg *arg)
{
	int i;
	int num_layouts = LENGTH(layouts);

	for (i = 0; i < num_layouts && &layouts[i] != selmon->lt[selmon->sellt]; i++);
	i += arg->i;
	setlayout(&((Arg) { .v = &layouts[(i % num_layouts + num_layouts) % num_layouts] })); // modulo
}
