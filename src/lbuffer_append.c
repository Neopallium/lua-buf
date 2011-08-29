/***************************************************************************
 * Copyright (C) 2007-2010 by Robert G. Jakabosky <bobby@neoawareness.com> *
 *                                                                         *
 ***************************************************************************/

#include "lbuffer_append.h"
#include <stdio.h>
#include <stdlib.h>
#include "num_utils.h"

#if defined(CHECK_BUFFER_SPACE)
#undef CHECK_BUFFER_SPACE
#endif
#define CHECK_BUFFER_SPACE(data, buf, need, return_val) do { \
	(data) = l_buffer_get_head((buf), (need)); \
	if L_UNLIKELY((data) == NULL) return (return_val); \
} while(0)

/*
 * append data/bytes functions.
 */
int l_buffer_append_data_len(LBuffer *buf, const uint8_t *src, size_t len) {
	uint8_t *data;

	/* make sure there is enough buffer space to append array of bytes. */
	CHECK_BUFFER_SPACE(data, buf, len, 0 - len);

	/* append data to buffer. */
	memcpy(data, src, len);
	buf->head += len;
	return len;
}

int l_buffer_append_string_len(LBuffer *buf, const char *str, size_t len) {
	/* append null-terminated string. */
	return l_buffer_append_data_len(buf, str, len+1);
}

/*
 * Append integers as binary big-endian values.
 */
int l_buffer_append_uint8_t(LBuffer *buf, uint8_t num) {
	uint8_t *data;

#define INT_LEN ((int)sizeof(uint8_t))
	/* make sure there is enough buffer space to append binary integer. */
	CHECK_BUFFER_SPACE(data, buf, INT_LEN, -INT_LEN);

	/* append integer to buffer. */
	*data = num;
	buf->head += INT_LEN;
	return INT_LEN;
#undef INT_LEN
}

int l_buffer_append_uint16_t(LBuffer *buf, uint16_t num) {
	uint8_t *data;

#define INT_LEN ((int)sizeof(uint16_t))
	/* make sure there is enough buffer space to append binary integer. */
	CHECK_BUFFER_SPACE(data, buf, INT_LEN, -INT_LEN);

	/* convert from host endian to big-endian. */
	num = l_htobe16(num);

	/* append integer to buffer. */
	memcpy(data, &(num), INT_LEN);
	buf->head += INT_LEN;
	return INT_LEN;
#undef INT_LEN
}

int l_buffer_append_uint32_t(LBuffer *buf, uint32_t num) {
	uint8_t *data;

#define INT_LEN ((int)sizeof(uint32_t))
	/* make sure there is enough buffer space to append binary integer. */
	CHECK_BUFFER_SPACE(data, buf, INT_LEN, -INT_LEN);

	/* convert from host endian to big-endian. */
	num = l_htobe32(num);

	/* append integer to buffer. */
	memcpy(data, &(num), INT_LEN);
	buf->head += INT_LEN;
	return INT_LEN;
#undef INT_LEN
}

int l_buffer_append_uint64_t(LBuffer *buf, uint64_t num) {
	uint8_t *data;

#define INT_LEN ((int)sizeof(uint64_t))
	/* make sure there is enough buffer space to append binary integer. */
	CHECK_BUFFER_SPACE(data, buf, INT_LEN, -INT_LEN);

	/* convert from host endian to big-endian. */
	num = l_htobe64(num);

	/* append integer to buffer. */
	memcpy(data, &(num), INT_LEN);
	buf->head += INT_LEN;
	return INT_LEN;
#undef INT_LEN
}

/*
 * Append floats/doubles as binary values.
 */
int l_buffer_append_float(LBuffer *buf, float num) {
	union { float fnum; uint32_t inum; } data = { .fnum = num };
	return l_buffer_append_uint32_t(buf, data.inum);
}

int l_buffer_append_double(LBuffer *buf, double num) {
	union { double fnum; uint64_t inum; } data = { .fnum = num };
	return l_buffer_append_uint64_t(buf, data.inum);
}

/*
 * Append integers as variable length big-endian values.
 */
/* base 128 varints. */
int l_buffer_append_b128_uvar32(LBuffer *buf, uint32_t num) {
	int len = 0;
	uint8_t *data;
	uint8_t b;

#define VAR_INT_MAX_LEN L_B128_INT32_MAX_LEN
	/* make sure there is enough buffer space to append variable length integer. */
	CHECK_BUFFER_SPACE(data, buf, VAR_INT_MAX_LEN, -VAR_INT_MAX_LEN);

	/* append variable length integer to buffer. */
	goto first_byte; /* this is to skip the set msb & len++ */
	do {
		b |= 0x80; /* set msb of previous byte. */
		data[len] = b;
		len++;
first_byte:
		b = (num & 0x7F); /* copy 7bits into current byte. */
		num >>= 7; /* remove 7bits from number. */
	} while(num > 0);
	data[len] = b;
	len++;

	l_assert2(len <= VAR_INT_MAX_LEN, "The impossible happened, b128_uvar encoding is broken.");
	buf->head += len;
	return len;
#undef VAR_INT_MAX_LEN
}

int l_buffer_append_b128_var32(LBuffer *buf, int32_t num, bool zigzag) {
	if(zigzag) num = l_zigzag_int32_t(num);
	return l_buffer_append_b128_uvar32(buf, num);
}

int l_buffer_append_b128_uvar64(LBuffer *buf, uint64_t num) {
	int len = 0;
	uint8_t *data;
	uint8_t b;

#define VAR_INT_MAX_LEN L_B128_INT64_MAX_LEN
	/* make sure there is enough buffer space to append variable length integer. */
	CHECK_BUFFER_SPACE(data, buf, VAR_INT_MAX_LEN, -VAR_INT_MAX_LEN);

	/* append variable length integer to buffer. */
	goto first_byte; /* this is to skip the set msb & len++ */
	do {
		b |= 0x80; /* set msb of previous byte. */
		data[len] = b;
		len++;
first_byte:
		b = (num & 0x7F); /* copy 7bits into current byte. */
		num >>= 7; /* remove 7bits from number. */
	} while(num > 0);
	data[len] = b;
	len++;

	l_assert2(len <= VAR_INT_MAX_LEN, "The impossible happened, b128_uvar encoding is broken.");
	buf->head += len;
	return len;
#undef VAR_INT_MAX_LEN
}

int l_buffer_append_b128_var64(LBuffer *buf, int64_t num, bool zigzag) {
	if(zigzag) num = l_zigzag_int64_t(num);
	return l_buffer_append_b128_uvar64(buf, num);
}

