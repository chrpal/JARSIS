
#ifndef DEFINES_H
#define DEFINES_H

#ifdef KRC2_GC_NATIVE_EXPORTS
#define DLL_SYMBOL __declspec(dllexport)
#else
#define DLL_SYMBOL __declspec(dllimport)
#endif

#endif