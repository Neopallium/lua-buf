/***************************************************************************
 * Copyright (C) 2007-2010 by Robert G. Jakabosky <bobby@neoawareness.com> *
 *                                                                         *
 ***************************************************************************/
#if !defined(__L_BUFFER_H__)
#define __L_BUFFER_H__

#include "lcommon.h"

typedef struct LBuffer LBuffer;

#if 0
typedef size_t buflen_t;
#else
typedef uint32_t buflen_t;
#endif

struct LBuffer {
	uint8_t   *data;           /**< Buffer data. */
	buflen_t  tail;            /**< start offset, tail must always be lower then head. */
	buflen_t  head;            /**< end offset, head must always be lower then size. */
	buflen_t  size;            /**< total memory size of data. */
	uint32_t  free_struct: 1;  /**< Should the LBuffer struct be freed. */
};

L_LIB_API void l_buffer_init(LBuffer *buf, const uint8_t *data, size_t len);

L_LIB_API LBuffer *l_buffer_new(const uint8_t *data, size_t len);

L_LIB_API void l_buffer_free(LBuffer *buf);

L_LIB_API void l_buffer_reset(LBuffer *buf);

L_LIB_API size_t l_buffer_length(LBuffer *buf);

L_LIB_API bool l_buffer_set_length(LBuffer *buf, size_t len);

L_LIB_API uint8_t *l_buffer_sub(LBuffer *buf, size_t off, size_t *plen);

L_LIB_API uint8_t *l_buffer_data(LBuffer *buf);

L_LIB_API size_t l_buffer_size(LBuffer *buf);

L_LIB_API bool l_buffer_resize(LBuffer *buf, size_t size);

/**
 * Read functions read from the buffer's tail.
 * Append functions append to the buffer's head.
 */
#include "lbuffer_read.h"
#include "lbuffer_append.h"

#endif /* __L_BUFFER_H__ */
