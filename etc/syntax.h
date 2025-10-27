// we will limit args to 16
#define REVERSE_1(a) a
#define REVERSE_2(a, b) b a
#define REVERSE_3(a, b, c) c b a
#define REVERSE_4(a, b, c, d) d c b a
#define REVERSE_5(a, b, c, d, e) e d c b a
#define REVERSE_6(a, b, c, d, e, f) f e d c b a
#define REVERSE_7(a, b, c, d, e, f, g) g f e d c b a
#define REVERSE_8(a, b, c, d, e, f, g, h) h g f e d c b a
#define REVERSE_9(a, b, c, d, e, f, g, h, i) i h g f e d c b a
#define REVERSE_10(a, b, c, d, e, f, g, h, i, j) j i h g f e d c b a
#define REVERSE_11(a, b, c, d, e, f, g, h, i, j, k) k j i h g f e d c b a
#define REVERSE_12(a, b, c, d, e, f, g, h, i, j, k, l) l k j i h g f e d c b a
#define REVERSE_13(a, b, c, d, e, f, g, h, i, j, k, l, m) m l k j i h g f e d c b a
#define REVERSE_14(a, b, c, d, e, f, g, h, i, j, k, l, m, n) n m l k j i h g f e d c b a
#define REVERSE_15(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o) o n m l k j i h g f e d c b a
#define REVERSE_16(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p) p o n m l k j i h g f e d c b a
#define GET_REVERSE_MACRO(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, NAME, ...) NAME
#define REVERSE(...) GET_REVERSE_MACRO(__VA_ARGS__, REVERSE_16, REVERSE_15, REVERSE_14, REVERSE_13, REVERSE_12, REVERSE_11, REVERSE_10, REVERSE_9, REVERSE_8, REVERSE_7, REVERSE_6, REVERSE_5, REVERSE_4, REVERSE_3, REVERSE_2, REVERSE_1)(__VA_ARGS__)

#define call(a, ...) REVERSE(__VA_ARGS__) a context call

#define jif(condition, position) position condition jif
#define goto(position) position 1 jif