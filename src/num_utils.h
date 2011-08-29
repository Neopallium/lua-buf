/***************************************************************************
 * Copyright (C) 2007-2010 by Robert G. Jakabosky <bobby@neoawareness.com> *
 *                                                                         *
 ***************************************************************************/
#ifndef __L_NUM_UTILS_H__
#define __L_NUM_UTILS_H__

#include "bit_utils.h"

/*
 * endianness convertion.
 */
#include <endian.h>
#include <byteswap.h>

#ifndef BYTE_ORDER
#define BYTE_ORDER __BYTE_ORDER
#endif
#ifndef LITTLE_ENDIAN
#define LITTLE_ENDIAN __LITTLE_ENDIAN
#endif
#ifndef BIG_ENDIAN
#define BIG_ENDIAN __BIG_ENDIAN
#endif

/* check for standard byteswap macros. */
#ifndef htobe16
#if BYTE_ORDER == LITTLE_ENDIAN
#define htobe16(x) bswap_16(x)
#define htole16(x) (x)
#define be16toh(x) bswap_16(x)
#define le16toh(x) (x)

#define htobe32(x) bswap_32(x)
#define htole32(x) (x)
#define be32toh(x) bswap_32(x)
#define le32toh(x) (x)

#define htobe64(x) bswap_64(x)
#define htole64(x) (x)
#define be64toh(x) bswap_64(x)
#define le64toh(x) (x)
#else
#define htobe16(x) (x)
#define htole16(x) bswap_16(x)
#define be16toh(x) (x)
#define le16toh(x) bswap_16(x)

#define htobe32(x) (x)
#define htole32(x) bswap_32(x)
#define be32toh(x) (x)
#define le32toh(x) bswap_32(x)

#define htobe64(x) (x)
#define htole64(x) bswap_64(x)
#define be64toh(x) (x)
#define le64toh(x) bswap_64(x)
#endif
#endif

/* 16 bit endian convertion. */
#define l_htobe16(host_16bits) \
	htobe16(host_16bits)
#define l_htole16(host_16bits) \
	htole16(host_16bits)
#define l_be16toh(big_endian_16bits) \
	be16toh(big_endian_16bits)
#define l_le16toh(little_endian_16bits) \
	le16toh(little_endian_16bits)

/* 32 bit endian convertion. */
#define l_htobe32(host_32bits) \
	htobe32(host_32bits)
#define l_htole32(host_32bits) \
	htole32(host_32bits)
#define l_be32toh(big_endian_32bits) \
	be32toh(big_endian_32bits)
#define l_le32toh(little_endian_32bits) \
	le32toh(little_endian_32bits)

/* 64 bit endian convertion. */
#define l_htobe64(host_64bits) \
	htobe64(host_64bits)
#define l_htole64(host_64bits) \
	htole64(host_64bits)
#define l_be64toh(big_endian_64bits) \
	be64toh(big_endian_64bits)
#define l_le64toh(little_endian_64bits) \
	le64toh(little_endian_64bits)

/*
 * ZigZag encoding.
 */
#define l_zigzag_int32_t(num) \
	(int32_t)((((int32_t)num) << 1) ^ (((int32_t)num) >> 31))

#define l_unzigzag_int32_t(num) \
	(int32_t)((((uint32_t)num) >> 1) ^ -(((int32_t)num) & 1))

#define l_zigzag_int64_t(num) \
	(int64_t)((((int64_t)num) << 1) ^ (((int64_t)num) >> 63))

#define l_unzigzag_int64_t(num) \
	(int64_t)((((uint64_t)num) >> 1) ^ -(((int64_t)num) & 1))

/*
 * Base 128 Varint encoding.
 */
#define L_B128_INT32_MAX_LEN 5
#define l_b128_varint32_len(num) \
	((l_bit_storage(num) + 6) / 7)

#define L_B128_INT64_MAX_LEN 10
#define l_b128_varint64_len(num) \
	((l_bit_storagel(num) + 6) / 7)

#endif /* __L_NUM_UTILS_H__ */
