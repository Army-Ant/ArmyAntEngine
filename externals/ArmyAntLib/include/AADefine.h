#ifndef A_A_DEFINE_H_2015_11_11
#define A_A_DEFINE_H_2015_11_11

#if defined DEBUG
#include <assert.h>
#define AAAssert(x) assert(x)
#elif defined _cplusplus
#include <exception>
#define AAAssert(x) if(!(x)) throw std::exception();
#else
#define AAAssert(x)
#endif

#define AA_SAFE_DEL(x) if(x != nullptr){delete x; x = nullptr;}
#define AA_SAFE_DELALL(x) if(x != nullptr){delete[] x; x = nullptr;}

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

#ifndef min
#define min(x,y) ((x<=y)?(x):(y))
#endif
#ifndef max
#define max(x,y) ((x>=y)?(x):(y))
#endif

#define AA_FORBID_EQUAL_OPR(typen) private: virtual typen operator=(typen Value){return Value;}
#define AA_FORBID_COPY_CTOR(typen) private: virtual typen(typen&Value){}

#endif // A_A_DEFINE_H_2015_11_11
