#if __UINTPTR_MAX__ == 0xffffffff
#define SYSTEM_32BIT 1
#elif __UINTPTR_MAX__ == 0xffffffffffffffff
#define SYSTEM_64BIT 1
#endif

#ifdef SYSTEM_64BIT 
    #define CALL       9223372036854775807
    #define GOIF       9223372036854775806
    #define DROP       9223372036854775805

    // LIST OPCODES
    #define NEW        9223372036854775804
    #define FREE       9223372036854775803
    #define PUSH       9223372036854775802
    #define POP        9223372036854775801
    #define UNSHIFT    9223372036854775800
    #define SHIFT      9223372036854775799
    #define INSERT     9223372036854775798
    #define REMOVE     9223372036854775797
    #define SWAP       9223372036854775796
    #define SET        9223372036854775795
    #define GET        9223372036854775794

    // BASIC MATH OPCODES
    #define ADD        9223372036854775793
    #define SUB        9223372036854775792
    #define BIT_AND    9223372036854775791
    #define BIT_OR     9223372036854775790
    #define BIT_XOR    9223372036854775789
    #define BIT_LS     9223372036854775788
    #define BIT_RS     9223372036854775787
    #define BIT_NOT    9223372036854775786

    // ALIASES
    #define CONTEXT    9223372036854775785
    #define STACK      9223372036854775784
    #define CODE       9223372036854775783
    #define UINT       9223372036854775782
    #define STRING     9223372036854775781
#elif SYSTEM_32BIT
    // SPECIAL OPCODES
    #define CALL       2147483647
    #define GOIF       2147483646
    #define DROP       2147483645

    // LIST OPCODES
    #define NEW        2147483644
    #define FREE       2147483643
    #define PUSH       2147483642
    #define POP        2147483641
    #define UNSHIFT    2147483640
    #define SHIFT      2147483639
    #define INSERT     2147483638
    #define REMOVE     2147483637
    #define SWAP       2147483636
    #define SET        2147483635
    #define GET        2147483634

    // BASIC MATH OPCODES
    #define ADD        2147483633
    #define SUB        2147483632
    #define BIT_AND    2147483631
    #define BIT_OR     2147483630
    #define BIT_XOR    2147483629
    #define BIT_LS     2147483628
    #define BIT_RS     2147483627
    #define BIT_NOT    2147483626

    // ALIASES
    #define CONTEXT    2147483625
    #define STACK      2147483624
    #define CODE       2147483623
    #define UINT       2147483622
    #define STRING     2147483621
#endif 