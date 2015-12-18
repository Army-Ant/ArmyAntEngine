#ifndef C_A_A_DEFINE_H_2015_11_11
#define C_A_A_DEFINE_H_2015_11_11

#include "AADefine.h"

#ifdef _cplusplus

#include <yvals.h>

#else

typedef _Bool bool;
const int true = TRUE;
const int false = FALSE;

#undef AA_SAFE_DEL
#undef AA_SAFE_DELALL
#define AA_SAFE_DEL(x) {if(x != NULL){free(x); x = NULL;}}
#define AA_SAFE_FREE AA_SAFE_DEL
#define AA_SAFE_DELALL AA_SAFE_DEL

#endif

#define ARMYANT_CLIB_API ARMYANTLIB_API

#endif // C_A_A_DEFINE_H_2015_11_11