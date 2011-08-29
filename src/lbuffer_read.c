/***************************************************************************
 * Copyright (C) 2007-2010 by Robert G. Jakabosky <bobby@neoawareness.com> *
 *                                                                         *
 ***************************************************************************/

#include "lbuffer_read.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include "num_utils.h"

#if defined(CHECK_BUFFER_SPACE)
#undef CHECK_BUFFER_SPACE
#endif
#define CHECK_BUFFER_SPACE(data, buf, need, return_val) do { \
	(data) = l_buffer_get_tail((buf), (need)); \
	if L_UNLIKELY((data) == NULL) return (return_val); \
} while(0)

#define BUFFER_CONSUME(buf, len) do { \
	buflen_t tail = (buf)->tail + (len); \
	if(tail == (buf)->head) { /* check if buffer is empty. */ \
		(buf)->head = 0; \
		tail = 0; \
	} \
	(buf)->tail = tail; \
} while(0)

/*
 * Read integers as binary big-endian values.
 */
int l_buffer_read_uint8_t(LBuffer *buf, uint8_t *num) {
	uint8_t *data;
#define INT_LEN ((int)sizeof(uint8_t))
	/* make sure there is enough buffer space to read binary integer. */
	CHECK_BUFFER_SPACE(data, buf, INT_LEN, -INT_LEN);

	/* read integer from buffer. */
	*num = data[0];
	BUFFER_CONSUME(buf, INT_LEN);
	return INT_LEN;
#undef INT_LEN
}

int l_buffer_read_uint16_t(LBuffer *buf, uint16_t *num) {
	uint8_t *data;

#define INT_LEN ((int)sizeof(uint16_t))
	/* make sure there is enough buffer space to read binary integer. */
	CHECK_BUFFER_SPACE(data, buf, INT_LEN, -INT_LEN);

	/* read integer from buffer. */
	memcpy(num, data, INT_LEN);
	BUFFER_CONSUME(buf, INT_LEN);

	/* convert from big-endian to host endian. */
	*num = l_be16toh(*num);

	return INT_LEN;
#undef INT_LEN
}

int l_buffer_read_uint32_t(LBuffer *buf, uint32_t *num) {
	uint8_t *data;

#define INT_LEN ((int)sizeof(uint32_t))
	/* make sure there is enough buffer space to read binary integer. */
	CHECK_BUFFER_SPACE(data, buf, INT_LEN, -INT_LEN);

	/* read integer from buffer. */
	memcpy(num, data, INT_LEN);
	BUFFER_CONSUME(buf, INT_LEN);

	/* convert from big-endian to host endian. */
	*num = l_be32toh(*num);

	return INT_LEN;
#undef INT_LEN
}

int l_buffer_read_uint64_t(LBuffer *buf, uint64_t *num) {
	uint8_t *data;

#define INT_LEN ((int)sizeof(uint64_t))
	/* make sure there is enough buffer space to read binary integer. */
	CHECK_BUFFER_SPACE(data, buf, INT_LEN, -INT_LEN);

	/* read integer from buffer. */
	memcpy(num, data, INT_LEN);
	BUFFER_CONSUME(buf, INT_LEN);

	/* convert from big-endian to host endian. */
	*num = l_be64toh(*num);

	return INT_LEN;
#undef INT_LEN
}

/*
 * Read floats/doubles as binary values.
 */
int l_buffer_read_float(LBuffer *buf, float *num) {
	return l_buffer_read_uint32_t(buf, (uint32_t *)num);
}

int l_buffer_read_double(LBuffer *buf, double *num) {
	return l_buffer_read_uint64_t(buf, (uint64_t *)num);
}

/*
 * Read integers as variable length big-endian values.
 */
/* base 128 varints. */
int l_buffer_read_b128_uvar32(LBuffer *buf, uint32_t *num) {
	uint32_t consume = 0;
	uint32_t val = 0;
	uint32_t blen;
	uint32_t off = 0;
	uint8_t *data;
	uint8_t b;

#define VAR_INT_MAX_LEN L_B128_INT32_MAX_LEN
	blen = L_BUFFER_LENGTH(buf);
	data = buf->data + buf->tail;
	if L_LIKELY(blen > VAR_INT_MAX_LEN) blen = VAR_INT_MAX_LEN+1;

	/* read variable length integer from buffer. */
	do {
		/* check if we have run out of buffer space. */
		if L_UNLIKELY(consume >= blen) return -1;
		b = data[consume];
		val |= ((uint32_t)(b & ~0x80)) << off; /* strip msb of all bytes. */
		consume++;
		off += 7;
	} while((b & 0x80) != 0);

	if(consume > VAR_INT_MAX_LEN) {
		return L_BUFFER_NUM_PARSE_ERROR;
	}

	*num = val;

	BUFFER_CONSUME(buf, consume);
	return consume;
#undef VAR_INT_MAX_LEN
}

int l_buffer_read_b128_var32(LBuffer *buf, int32_t *num, bool zigzag) {
	int rc = l_buffer_read_b128_uvar32(buf, (uint32_t *)num);
	if(rc > 0) {
		if(zigzag) *num = l_unzigzag_int32_t(*num);
	}
	return rc;
}

int l_buffer_read_b128_uvar64(LBuffer *buf, uint64_t *num) {
	uint32_t consume = 0;
	uint64_t val = 0;
	uint32_t blen;
	uint32_t off = 0;
	uint8_t *data;
	uint8_t b;

#define VAR_INT_MAX_LEN L_B128_INT64_MAX_LEN
	blen = L_BUFFER_LENGTH(buf);
	data = buf->data + buf->tail;
	if L_LIKELY(blen > VAR_INT_MAX_LEN) blen = VAR_INT_MAX_LEN+1;

	/* read variable length integer from buffer. */
	do {
		/* check if we have run out of buffer space. */
		if L_UNLIKELY(consume >= blen) return -1;
		b = data[consume];
		val |= ((uint64_t)(b & ~0x80)) << off; /* strip msb of all bytes. */
		consume++;
		off += 7;
	} while((b & 0x80) != 0);

	if(consume > VAR_INT_MAX_LEN) {
		return L_BUFFER_NUM_PARSE_ERROR;
	}

	*num = val;

	BUFFER_CONSUME(buf, consume);
	return consume;
#undef VAR_INT_MAX_LEN
}

int l_buffer_read_b128_var64(LBuffer *buf, int64_t *num, bool zigzag) {
	int rc = l_buffer_read_b128_uvar64(buf, (uint64_t *)num);
	if(rc > 0) {
		if(zigzag) *num = l_unzigzag_int64_t(*num);
	}
	return rc;
}

