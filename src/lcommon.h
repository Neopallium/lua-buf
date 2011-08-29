/***************************************************************************
 * Copyright (C) 2007-2010 by Robert G. Jakabosky <bobby@neoawareness.com> *
 *                                                                         *
 ***************************************************************************/
#if !defined(__L_COMMON_H__)
#define __L_COMMON_H__

#define L_LIB_API extern
#define L_INLINE static inline

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/user.h>
#include <glib.h>

#ifdef NDEBUG
#define l_assert(expr) do { } while(0)
#define l_assert2(expr, str) do { } while(0)
#else
#define l_assert(expr) g_assert(expr)
#define l_assert2(expr, str) G_STMT_START { \
  if L_LIKELY(expr) {                     \
  } else {                                \
    g_assertion_message_expr (G_LOG_DOMAIN, __FILE__, __LINE__, G_STRFUNC, str ", " #expr); \
  }                                       \
} G_STMT_END
#endif

#define l_strlen(str) \
	(__builtin_constant_p(str) ? (sizeof(str) - 1) : strlen(str))

#define L_GNUC_PACKED __attribute__((__packed__))
#define L_UNUSED(var) ((void)var)
#define L_FUNC_UNUSED __attribute__((unused))

/* L_LIKELY/L_UNLIKELY */
#define L_LIKELY(exp) G_LIKELY(exp)
#define L_UNLIKELY(exp) G_UNLIKELY(exp)

/* L_PAGE_SIZE */
#ifdef PAGE_SIZE
#define L_PAGE_SIZE PAGE_SIZE
#else
#define L_PAGE_SIZE (4 * 1024)
#endif

#endif /* __L_COMMON_H__ */
