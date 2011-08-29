/***************************************************************************
 * Copyright (C) 2007-2010 by Robert G. Jakabosky <bobby@neoawareness.com> *
 *                                                                         *
 ***************************************************************************/
#ifndef __L_BUFFER_READ_H__
#define __L_BUFFER_READ_H__

#include "lbuffer.h"

#define L_BUFFER_NUM_PARSE_ERROR (-1000)

/**
 * Read an 8-bit binary big-endian integer.
 *
 * @param buf LBuffer instance.
 * @param num pointer to numeric variable where read number will be stored.
 * @return number of bytes read from buffer, or < 0 to indicate more data is needed.
 * @public @memberof LBuffer
 */
L_LIB_API int l_buffer_read_uint8_t(LBuffer *buf, uint8_t *num);
#define l_buffer_read_int8_t(buf, num) \
	l_buffer_read_uint8_t(buf, (uint8_t *) num)

/**
 * Read an 16-bit binary big-endian integer.
 *
 * @param buf LBuffer instance.
 * @param num pointer to numeric variable where read number will be stored.
 * @return number of bytes read from buffer, or < 0 to indicate more data is needed.
 * @public @memberof LBuffer
 */
L_LIB_API int l_buffer_read_uint16_t(LBuffer *buf, uint16_t *num);
#define l_buffer_read_int16_t(buf, num) \
	l_buffer_read_uint16_t(buf, (uint16_t *) num)

/**
 * Read an 32-bit binary big-endian integer.
 *
 * @param buf LBuffer instance.
 * @param num pointer to numeric variable where read number will be stored.
 * @return number of bytes read from buffer, or < 0 to indicate more data is needed.
 * @public @memberof LBuffer
 */
L_LIB_API int l_buffer_read_uint32_t(LBuffer *buf, uint32_t *num);
#define l_buffer_read_int32_t(buf, num) \
	l_buffer_read_uint32_t(buf, (uint32_t *) num)

/**
 * Read an 64-bit binary big-endian integer.
 *
 * @param buf LBuffer instance.
 * @param num pointer to numeric variable where read number will be stored.
 * @return number of bytes read from buffer, or < 0 to indicate more data is needed.
 * @public @memberof LBuffer
 */
L_LIB_API int l_buffer_read_uint64_t(LBuffer *buf, uint64_t *num);
#define l_buffer_read_int64_t(buf, num) \
	l_buffer_read_uint64_t(buf, (uint64_t *) num)

/**
 * Read a binary float.
 *
 * @param buf LBuffer instance.
 * @param num pointer to numeric variable where read number will be stored.
 * @return number of bytes read from buffer, or < 0 to indicate more data is needed.
 * @public @memberof LBuffer
 */
L_LIB_API int l_buffer_read_float(LBuffer *buf, float *num);

/**
 * Read a binary double.
 *
 * @param buf LBuffer instance.
 * @param num pointer to numeric variable where read number will be stored.
 * @return number of bytes read from buffer, or < 0 to indicate more data is needed.
 * @public @memberof LBuffer
 */
L_LIB_API int l_buffer_read_double(LBuffer *buf, double *num);

/**
 * Read a base 128 variable-length big-endian integer value.
 *
 * @param buf LBuffer instance.
 * @param num pointer to numeric variable where read number will be stored.
 * @return number of bytes read from buffer, or < 0 to indicate more data is needed.
 * @public @memberof LBuffer
 */
L_LIB_API int l_buffer_read_b128_uvar32(LBuffer *buf, uint32_t *num);

/**
 * Read a base 128 variable-length big-endian integer value.
 *
 * @param buf LBuffer instance.
 * @param num pointer to numeric variable where read number will be stored.
 * @param zigzag true, unzigzag number.  false, don't unzigzag number.
 * @return number of bytes read from buffer, or < 0 to indicate more data is needed.
 * @public @memberof LBuffer
 */
L_LIB_API int l_buffer_read_b128_var32(LBuffer *buf, int32_t *num, bool zigzag);

/**
 * Read a base 128 variable-length big-endian integer value.
 *
 * @param buf LBuffer instance.
 * @param num pointer to numeric variable where read number will be stored.
 * @return number of bytes read from buffer, or < 0 to indicate more data is needed.
 * @public @memberof LBuffer
 */
L_LIB_API int l_buffer_read_b128_uvar64(LBuffer *buf, uint64_t *num);

/**
 * Read a base 128 variable-length big-endian integer value.
 *
 * @param buf LBuffer instance.
 * @param num pointer to numeric variable where read number will be stored.
 * @param zigzag true, unzigzag number.  false, don't unzigzag number.
 * @return number of bytes read from buffer, or < 0 to indicate more data is needed.
 * @public @memberof LBuffer
 */
L_LIB_API int l_buffer_read_b128_var64(LBuffer *buf, int64_t *num, bool zigzag);

#endif /* __L_BUFFER_READ_H__ */
