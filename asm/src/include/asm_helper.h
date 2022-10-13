#ifndef __ASM_HELPER_H_INCLUDED__
#define __ASM_HELPER_H_INCLUDED__



#define ASM_CHECK(cond, ret)                                                    \
do                                                                              \
{                                                                               \
    bool tmpcond_ = (cond);                                                     \
    assert(tmpcond_);                                                           \
    if (!(tmpcond_))                                                            \
    {                                                                           \
        return ret;                                                             \
    }                                                                           \
}                                                                               \
while (0)



#endif /* __ASM_HELPER_H_INCLUDED__ */
