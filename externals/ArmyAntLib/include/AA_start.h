#ifndef AA_START_H_2015_11_11
#define AA_START_H_2015_11_11

#ifdef _WIN32

#ifdef ARMYANTLIB_EXPORTS
#define ARMYANTLIB_API __declspec(dllexport)
#else
#define ARMYANTLIB_API __declspec(dllimport)
#endif

#endif // _WIN32

#endif // AA_START_H_2015_11_11
