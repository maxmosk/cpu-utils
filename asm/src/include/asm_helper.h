#ifndef ASM_HELPER_H_INCLUDED__
#define ASM_HELPER_H_INCLUDED__



#define ASM_CHECK(cond, ret)\
do                           \
{                             \
    bool tmpcond_ = (cond);    \
    assert(tmpcond_);           \
    if (!(tmpcond_))             \
    {                             \
        return ret;                \
    }                               \
}                                    \
while (0)


#define COMPILE_ERROR(code)                                   \
do                                                             \
{                                                               \
    printf("Compile error on line %zu on pass %d\n", line, pass);\
    printf("> %s\n", curLine);                                    \
    printf("Error code: %d\n", code);                              \
}                                                                   \
while (0)



#endif /* ASM_HELPER_H_INCLUDED__ */
