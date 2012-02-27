/***************************************************************************
 * Copyright (C) 2007-2010 by Robert G. Jakabosky <bobby@neoawareness.com> *
 *                                                                         *
 ***************************************************************************/
#if !defined(__L_COMMON_H__)
#define __L_COMMON_H__

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <assert.h>
#include <stdio.h>

#ifdef __WINDOWS__

/* for MinGW32 compiler need to include <stdint.h> */
#ifdef __GNUC__
#include <stdint.h>
#include <stdbool.h>
#else

/* define some standard types missing on Windows. */
#ifndef __INT32_MAX__
typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;
#endif
#ifndef __INT64_MAX__
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
#endif
typedef int bool;
#ifndef true
#define true 1
#endif
#ifndef false
#define false 0
#endif

#endif

#define L_LIB_API __declspec(dllexport)
#define L_INLINE static inline

#else

#define L_LIB_API extern
#define L_INLINE static inline

#include <stdint.h>
#include <stdbool.h>

#define L_FUNC_UNUSED __attribute__((unused))

#endif

#ifdef NDEBUG
#define l_assert(expr) do { } while(0)
#define l_assert2(expr, str) do { } while(0)
#else
#define l_assert(expr) do { \
  if L_LIKELY(expr) { \
  } else { \
    fprintf(stderr, #expr); \
		abort(); \
  } \
} while(0)
#define l_assert2(expr, str) do { \
  if L_LIKELY(expr) { \
  } else { \
    fprintf(stderr, str ", " #expr); \
		abort(); \
  } \
} while(0)
#endif

#define l_strlen(str) \
	(__builtin_constant_p(str) ? (sizeof(str) - 1) : strlen(str))

#define L_GNUC_PACKED __attribute__((__packed__))
#define L_UNUSED(var) ((void)var)

/* L_LIKELY/L_UNLIKELY */
#if defined(__GNUC__) && (__GNUC__ > 2) && defined(__OPTIMIZE__)
#define L_LIKELY(exp) (__builtin_expect(((exp) ? 1 : 0), 1))
#define L_UNLIKELY(exp) (__builtin_expect(((exp) ? 1 : 0), 0))
#else
#define L_LIKELY(exp) (exp)
#define L_UNLIKELY(exp) (exp)
#endif

/* L_PAGE_SIZE */
#ifdef PAGE_SIZE
#define L_PAGE_SIZE PAGE_SIZE
#else
#define L_PAGE_SIZE (4 * 1024)
#endif

#endif /* __L_COMMON_H__ */
