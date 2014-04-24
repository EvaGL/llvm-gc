#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "config.h"
#if MARK_DEBUG == 1
    #include <stdio.h>
    #define MDEBUG(str, args...) printf(str, ## args)
#else
    #define MDEBUG(str, args...) (void) 0
#endif

#endif
