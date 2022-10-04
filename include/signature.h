#ifndef __SIGNATURE_H_INCLUDED__
#define __SIGNATURE_H_INCLUDED__


#include <stdbool.h>
#include <stdint.h>
#include "version.h"


static const uint16_t CPU_EXE_FORMAT = 'PM';


typedef struct
{
    uint16_t format;
    uint16_t version;
} signature_t;


static bool signCheck(signature_t *sign)
{
    return (CPU_EXE_FORMAT == sign->format) && (CPU_EXE_VERSION == sign->version);
}


#endif /* __SIGNATURE_H_INCLUDED__ */

