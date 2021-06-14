enum { EX_NW, EX_N, EX_NE, EX_W, EX_C, EX_E, EX_SW, EX_S, EX_SE };

void expand(unsigned char mask);
void togglemaximize(const Arg *arg);
void toggleverticalexpand(const Arg *arg);
void togglehorizontalexpand(const Arg *arg);
void exresize(const Arg *arg);
void explace(const Arg *arg);

