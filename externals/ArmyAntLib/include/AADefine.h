#ifndef A_A_DEFINE_H_2015_11_11
#define A_A_DEFINE_H_2015_11_11

#if defined DEBUG
#include <assert.h>
#define AAAssert(x) assert(x)
#elif defined _cplusplus
#include <exception>
#define AAAssert(x) if(!(x)) throw std::exception("assert failed !");else;
#else
#define AAAssert(x) x
#endif

#define AA_SAFE_DEL(x) if(x != nullptr){delete x; x = nullptr;}else;
#define AA_SAFE_DELALL(x) if(x != nullptr){delete[] x; x = nullptr;}else;

#ifndef BYTE
typedef unsigned char BYTE;
#endif
#ifndef WORD
typedef unsigned short WORD;
#endif
#ifndef DWORD
typedef unsigned long DWORD;
#endif
#ifndef LWORD
#ifdef _cplusplus
typedef unsigned _int64 LWORD;
#endif
#endif

#ifndef BOOL
typedef int BOOL;
#ifndef TRUE
const int TRUE = 1;
const int FALSE = 0;
#endif
#endif

#ifndef min
#define min(x,y) ((x<=y)?(x):(y))
#endif
#ifndef max
#define max(x,y) ((x>=y)?(x):(y))
#endif

#define AA_FORBID_ASSGN_OPR(typen) public: typen& operator=(typen&)=delete
#define AA_FORBID_COPY_CTOR(typen) public: typen(typen&)=delete

#endif // A_A_DEFINE_H_2015_11_11
