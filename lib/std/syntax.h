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

#define call(a, ...) REVERSE(__VA_ARGS__) a CONTEXT CALL
#define callist(...)          REVERSE(__VA_ARGS__) CALLIST
#define new(...)              REVERSE(__VA_ARGS__) NEW
#define free(...)             REVERSE(__VA_ARGS__) FREE
#define push(...)             REVERSE(__VA_ARGS__) PUSH
#define pop(...)              REVERSE(__VA_ARGS__) POP
#define unshift(...)          REVERSE(__VA_ARGS__) UNSHIFT
#define shift(...)            REVERSE(__VA_ARGS__) SHIFT
#define insert(...)           REVERSE(__VA_ARGS__) INSERT
#define remove(...)           REVERSE(__VA_ARGS__) REMOVE
#define swap(...)             REVERSE(__VA_ARGS__) SWAP
#define set(...)              REVERSE(__VA_ARGS__) SET
#define get(...)              REVERSE(__VA_ARGS__) GET
#define copy(...)             REVERSE(__VA_ARGS__) COPY
#define length(...)           REVERSE(__VA_ARGS__) LENGTH
#define double_(...)          REVERSE(__VA_ARGS__) DOUBLE
#define half(...)             REVERSE(__VA_ARGS__) HALF

#define dup(...)              REVERSE(__VA_ARGS__) DUP
#define drop(...)             REVERSE(__VA_ARGS__) DROP

#define add(...)              REVERSE(__VA_ARGS__) ADD
#define sub(...)              REVERSE(__VA_ARGS__) SUB

#define bit_and(...)          REVERSE(__VA_ARGS__) BIT_AND
#define bit_or(...)           REVERSE(__VA_ARGS__) BIT_OR
#define bit_xor(...)          REVERSE(__VA_ARGS__) BIT_XOR
#define bit_ls(...)           REVERSE(__VA_ARGS__) BIT_LS
#define bit_rs(...)           REVERSE(__VA_ARGS__) BIT_RS
#define bit_not(...)          REVERSE(__VA_ARGS__) BIT_NOT
#define equals(...)           REVERSE(__VA_ARGS__) EQUALS
#define not_equals(...)       REVERSE(__VA_ARGS__) NOT_EQUALS
#define greater(...)          REVERSE(__VA_ARGS__) GREATER
#define greater_or_equals(...) REVERSE(__VA_ARGS__) GREATER_OR_EQUALS
#define lesser(...)           REVERSE(__VA_ARGS__) LESSER
#define lesser_or_equals(...) REVERSE(__VA_ARGS__) LESSER_OR_EQUALS
#define and(...)              REVERSE(__VA_ARGS__) AND
#define or(...)               REVERSE(__VA_ARGS__) OR

#define write(...)            REVERSE(__VA_ARGS__) WRITE
#define read(...)             REVERSE(__VA_ARGS__) READ

#define context(...)          REVERSE(__VA_ARGS__) CONTEXT
#define stack(...)            REVERSE(__VA_ARGS__) STACK
#define code(...)             REVERSE(__VA_ARGS__) CODE
#define bypass(...)           BYPASS __VA_ARGS__
#define word_size(...)        REVERSE(__VA_ARGS__) WORD_SIZE


// concat
#define concat_(a, b) a##b
#define concat(a, b) concat_(a, b)

// tables
// 0 = DATA
// 2 = KEYS
// #define TABLE_NEW() NEW DUP DUP NEW PUSH NEW PUSH
#define TABLE_NEW() call(pack, 3, NEW, NEW)

#define TABLE_FREE(table) \
    PUSH(CONTEXT, table) \
    PUSH(GET(CONTEXT, -1), )

#define TABLE_PUSH(table, value) \
    PUSH(CONTEXT, table) \
    PUSH(GET(GET(CONTEXT, -1), 0), value) \
    PUSH(GET(GET(CONTEXT, -1), 1), 0) \
    PUSH(GET(GET(CONTEXT, -1), 2), 0) \
    POP(CONTEXT) \
    DROP()
