/***************************************************************************
 * Copyright (C) 2007-2010 by Robert G. Jakabosky <bobby@neoawareness.com> *
 *                                                                         *
 ***************************************************************************/
#if !defined(__BIT_UTILS_H__)
#define __BIT_UTILS_H__

#include "lcommon.h"

#define BITS_PER_LONG (8 * GLIB_SIZEOF_LONG)

/**
 * bit utility functions.
 */
#define l_bit_rot(x,k) (((x)<<(k)) | ((x)>>(32-(k))))

/**
 * return number of set(1) bits in x.
 * @todo Test SSE4 support of __builtin_popcount(x);
 */
static inline uint32_t l_bit_count(uint32_t x) {
	x = x - ((x >> 1) & 0x55555555);
	x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
	x = (x + (x >> 4)) & 0x0F0F0F0F;
	x = x + (x >> 8);
	x = x + (x >> 16);
	return x & 0x0000003F;
}
/**
 * return number of set(1) bits in x.
 * @todo Test SSE4 support of __builtin_popcountl(x);
 */
static inline uint32_t l_bit_countl(uint64_t x) {
	x = x - ((x >> 1) & 0x5555555555555555);
	x = (x & 0x3333333333333333) + ((x >> 2) & 0x3333333333333333);
	x = (x + (x >> 4)) & 0x0F0F0F0F0F0F0F0F;
	x = x + (x >> 8);
	x = x + (x >> 16);
	x = x + (x >> 32);
	return x & 0x0000007F;
}

/**
 * Returns the number of trailing 0-bits in x, starting at the least significant bit position.
 * If x is 0, return 32 or 64 if parameter is a long
 */
static inline uint32_t l_count_trailing_zeros(uint32_t x) {
#if defined(__GNUC__) && (__GNUC__ >= 4) && defined(__OPTIMIZE__)
	return L_LIKELY(x) ? __builtin_ctz(x) : 32;
#else
	return l_bit_count(~x & (x - 1));
#endif
}
static inline uint32_t l_count_trailing_zerosl(uint64_t x) {
#if defined(__GNUC__) && (__GNUC__ >= 4) && defined(__OPTIMIZE__)
	return L_LIKELY(x) ? __builtin_ctzl(x) : L_BITS_PER_LONG;
#else
	return l_bit_countl(~x & (x - 1));
#endif
}

/**
 * Returns the number of leading 0-bits in x, starting at the most significant bit position.
 * If x is 0, return 32 or 64 if parameter is a long
 */
static inline uint32_t l_count_leading_zeros(uint32_t x) {
#if defined(__GNUC__) && (__GNUC__ >= 4) && defined(__OPTIMIZE__)
	return L_LIKELY(x) ? __builtin_clz(x) : 32;
#else
	x = x | (x >> 1);
	x = x | (x >> 2);
	x = x | (x >> 4);
	x = x | (x >> 8);
	x = x | (x >>16);
	return l_bit_count(~x);
#endif
}
static inline uint32_t l_count_leading_zerosl(uint64_t x) {
#if defined(__GNUC__) && (__GNUC__ >= 4) && defined(__OPTIMIZE__)
	return L_LIKELY(x) ? __builtin_clzl(x) : L_BITS_PER_LONG;
#else
	x = x | (x >> 1);
	x = x | (x >> 2);
	x = x | (x >> 4);
	x = x | (x >> 8);
	x = x | (x >>16);
	x = x | (x >>32);
	return l_bit_countl(~x);
#endif
}

/**
 * bit storage.
 */
static inline uint32_t l_bit_storage(uint32_t x) {
	if L_LIKELY(x) {
		return (32 - l_count_leading_zeros(x));
	}
	return 1;
}

static inline uint64_t l_bit_storagel(uint64_t x) {
	if L_LIKELY(x) {
		return (64 - l_count_leading_zerosl(x));
	}
	return 1;
}

/**
 * round up to next power of 2.
 */
static inline uint32_t l_ceiling_pow2(uint32_t x) {
#if defined(__GNUC__) && (__GNUC__ >= 4) && defined(__OPTIMIZE__)
	x -= 1;
	if L_LIKELY(!(x & 0x80000000)) {
		x = l_count_leading_zeros(x);
		x -= 1;
		return 0x80000000 >> x;
	}
	return 0;
#else
	x = x - 1;
	x = x | (x >> 1);
	x = x | (x >> 2);
	x = x | (x >> 4);
	x = x | (x >> 8);
	x = x | (x >>16);
	return x + 1;
#endif
}

static inline uint64_t l_ceiling_pow2l(uint64_t x) {
#if defined(__GNUC__) && (__GNUC__ >= 4) && defined(__OPTIMIZE__)
	x -= 1;
	if L_LIKELY(!(x & 0x8000000000000000UL)) {
		x = l_count_leading_zerosl(x);
		x -= 1;
		return 0x8000000000000000UL >> x;
	}
	return 0;
#else
	x = x - 1;
	x = x | (x >> 1);
	x = x | (x >> 2);
	x = x | (x >> 4);
	x = x | (x >> 8);
	x = x | (x >>16);
	x = x | (x >>32);
	return x + 1;
#endif
}

/**
 * round down to next power of 2.
 */
static inline uint32_t l_floor_pow2(uint32_t x) {
#if defined(__GNUC__) && (__GNUC__ >= 4) && defined(__OPTIMIZE__)
	if L_LIKELY(x) {
		x = l_count_leading_zeros(x);
		return 0x80000000 >> x;
	}
	return 0;
#else
	x = x | (x >> 1);
	x = x | (x >> 2);
	x = x | (x >> 4);
	x = x | (x >> 8);
	x = x | (x >>16);
	return x - (x >> 1);
#endif
}

static inline uint64_t l_floor_pow2l(uint64_t x) {
#if defined(__GNUC__) && (__GNUC__ >= 4) && defined(__OPTIMIZE__)
	if L_LIKELY(x) {
		x = l_count_leading_zerosl(x);
		return 0x8000000000000000UL >> x;
	}
	return 0;
#else
	x = x | (x >> 1);
	x = x | (x >> 2);
	x = x | (x >> 4);
	x = x | (x >> 8);
	x = x | (x >>16);
	x = x | (x >>32);
	return x - (x >> 1);
#endif
}

#endif /* __L_BIT_UTILS_H__ */
