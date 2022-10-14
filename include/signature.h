#ifndef SIGNATURE_H_INCLUDED__
#define SIGNATURE_H_INCLUDED__


#include <stdbool.h>
#include <string.h>
#include "version.h"


static const char CPU_EXE_FORMAT[] = "MP";


typedef struct
{
    char format[2 + 1];
    char version[2 + 1];
} signature_t;


static bool signCheck(signature_t *sign)
{
    return strcmp(CPU_EXE_FORMAT, sign->format) && strcmp(CPU_EXE_VERSION, sign->version);
}


#endif /* SIGNATURE_H_INCLUDED__ */

