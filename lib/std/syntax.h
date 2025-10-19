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

// concat
#define concat_(a, b) a##b
#define concat(a, b) concat_(a, b)

// call & control
// controle e chamada
#define CALL(...)        REVERSE(__VA_ARGS__) CALL
#define CALLIST(...)     REVERSE(__VA_ARGS__) CALLIST
#define JMPIF(...)       REVERSE(__VA_ARGS__) JMPIF

// stack manipulation
#define DUP(...)         REVERSE(__VA_ARGS__) DUP
#define DROP(...)        REVERSE(__VA_ARGS__) DROP
#define COPY(...)        REVERSE(__VA_ARGS__) COPY

// list / urb operators
#define NEW(...)         REVERSE(__VA_ARGS__) NEW
#define FREE(...)        REVERSE(__VA_ARGS__) FREE
#define PUSH(...)        REVERSE(__VA_ARGS__) PUSH
#define POP(...)         REVERSE(__VA_ARGS__) POP
#define UNSHIFT(...)     REVERSE(__VA_ARGS__) UNSHIFT
#define SHIFT(...)       REVERSE(__VA_ARGS__) SHIFT
#define INSERT(...)      REVERSE(__VA_ARGS__) INSERT
#define REMOVE(...)      REVERSE(__VA_ARGS__) REMOVE
#define SWAP(...)        REVERSE(__VA_ARGS__) SWAP
#define SET(...)         REVERSE(__VA_ARGS__) SET
#define GET(...)         REVERSE(__VA_ARGS__) GET
#define LENGTH(...)      REVERSE(__VA_ARGS__) LENGTH
#define DOUBLE(...)      REVERSE(__VA_ARGS__) DOUBLE
#define HALF(...)        REVERSE(__VA_ARGS__) HALF

// bitwise
#define ADD(...)         REVERSE(__VA_ARGS__) ADD
#define SUB(...)         REVERSE(__VA_ARGS__) SUB
#define BIT_AND(...)     REVERSE(__VA_ARGS__) BIT_AND
#define BIT_OR(...)      REVERSE(__VA_ARGS__) BIT_OR
#define BIT_XOR(...)     REVERSE(__VA_ARGS__) BIT_XOR
#define BIT_LS(...)      REVERSE(__VA_ARGS__) BIT_LS
#define BIT_RS(...)      REVERSE(__VA_ARGS__) BIT_RS
#define BIT_NOT(...)     REVERSE(__VA_ARGS__) BIT_NOT

// condições
#define EQUALS(...)      REVERSE(__VA_ARGS__) EQUALS
#define NOT_EQUALS(...)  REVERSE(__VA_ARGS__) NOT_EQUALS
#define GREATER(...)     REVERSE(__VA_ARGS__) GREATER
#define LESSER(...)      REVERSE(__VA_ARGS__) LESSER
#define AND(...)         REVERSE(__VA_ARGS__) AND
#define OR(...)          REVERSE(__VA_ARGS__) OR

// buffer ops
#define WRITE(...)       REVERSE(__VA_ARGS__) WRITE
#define READ(...)        REVERSE(__VA_ARGS__) READ

// tables
// 0 = DATA
// 1 = META
// 2 = KEYS
// #define TABLE_NEW() NEW DUP DUP DUP NEW PUSH NEW PUSH NEW PUSH
#define TABLE_NEW() \
    DUP(DUP(DUP(NEW()))) \
    PUSH(NEW) \
    PUSH(NEW) \
    PUSH(NEW)

#define TABLE_PUSH(value) \
    DUP() \
    GET(GET(STACK, -2), 0) \
    