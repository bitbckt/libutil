#pragma once

/*
 * __attribute__ is available in gcc 2.5 and later, but only with gcc 2.7
 * could you use the __format__ form of the attributes, which is what we use
 * (to avoid confusion with other macros).
 */
#ifndef __attribute__
#    if __GNUC__ < 2 || (__GNUC__ == 2 && __GNUC_MINOR__ < 7)
#        define __attribute__(spec) /* empty */
#    endif
#endif

/*
 * BEGIN_DECLS is used at the beginning of declarations so that C++
 * compilers don't mangle their names.  END_DECLS is used at the end.
 */
#undef BEGIN_DECLS
#undef END_DECLS
#ifdef __cplusplus
#    define BEGIN_DECLS extern "C" {
#    define END_DECLS   }
#else
#    define BEGIN_DECLS /* empty */
#    define END_DECLS   /* empty */
#endif
