/***************************************************************************
 * Copyright (C) 2007-2010 by Robert G. Jakabosky <bobby@neoawareness.com> *
 *                                                                         *
 ***************************************************************************/
#ifndef __L_BUFFER_APPEND_H__
#define __L_BUFFER_APPEND_H__

/**
 * append data/bytes functions.
 * @public @memberof LBuffer
 */
L_LIB_API int l_buffer_append_data_len(LBuffer *buf, const uint8_t *data, size_t len);

/**
 * Append integers as binary big-endian values.
 * @public @memberof LBuffer
 */
L_LIB_API int l_buffer_append_uint8_t(LBuffer *buf, uint8_t num);
#define l_buffer_append_int8_t(buf, num) \
	l_buffer_append_uint8_t(buf, (uint8_t) num)
L_LIB_API int l_buffer_append_uint16_t(LBuffer *buf, uint16_t num);
#define l_buffer_append_int16_t(buf, num) \
	l_buffer_append_uint16_t(buf, (uint16_t) num)

L_LIB_API int l_buffer_append_uint32_t(LBuffer *buf, uint32_t num);
#define l_buffer_append_int32_t(buf, num) \
	l_buffer_append_uint32_t(buf, (uint32_t) num)

L_LIB_API int l_buffer_append_uint64_t(LBuffer *buf, uint64_t num);
#define l_buffer_append_int64_t(buf, num) \
	l_buffer_append_uint64_t(buf, (uint64_t) num)

/**
 * Append floats/doubles as binary values.
 * @public @memberof LBuffer
 */
L_LIB_API int l_buffer_append_float(LBuffer *buf, float num);
L_LIB_API int l_buffer_append_double(LBuffer *buf, double num);

/**
 * Append integers as base 128 variable-length big-endian values.
 * @public @memberof LBuffer
 */
L_LIB_API int l_buffer_append_b128_uvar32(LBuffer *buf, uint32_t num);
L_LIB_API int l_buffer_append_b128_var32(LBuffer *buf, int32_t num, bool zigzag);
L_LIB_API int l_buffer_append_b128_uvar64(LBuffer *buf, uint64_t num);
L_LIB_API int l_buffer_append_b128_var64(LBuffer *buf, int64_t num, bool zigzag);

#endif /* __L_BUFFER_APPEND_H__ */
