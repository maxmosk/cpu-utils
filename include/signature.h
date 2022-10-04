#ifndef __SIGNATURE_H_INCLUDED__
#define __SIGNATURE_H_INCLUDED__


#include <stdint.h>
#include "version.h"


static const uint16_t CPU_EXE_FORMAT = 'PM';


typedef struct
{
    uint16_t format;
    uint16_t version;
} signature_t;


#endif /* __SIGNATURE_H_INCLUDED__ */

