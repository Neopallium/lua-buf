/***************************************************************************
 * Copyright (C) 2007-2010 by Robert G. Jakabosky <bobby@neoawareness.com> *
 *                                                                         *
 ***************************************************************************/

#include "lbuffer.h"
#include <stdlib.h>
#include <string.h>

#define MIN_BUFFER_SIZE 512

#define L_BUFFER_LENGTH(buf) ((buf)->head - (buf)->tail)

void l_buffer_init(LBuffer *buf, const uint8_t *data, size_t len) {
	size_t size = (len > MIN_BUFFER_SIZE) ? len : MIN_BUFFER_SIZE;

	/* initialize buffer object. */
	buf->tail = 0;
	buf->head = len;
	buf->size = size;
	buf->data = (uint8_t *)malloc(sizeof(uint8_t) * size);
	if(data) {
		/* copy data into buffer. */
		memcpy(buf->data, data, len);
	}
	buf->free_struct = 0;
}

LBuffer *l_buffer_new_full(const uint8_t *data, size_t len) {
	LBuffer *buf;

	/* allocate buffer object. */
	buf = (LBuffer *)malloc(sizeof(LBuffer));
	l_buffer_init(buf, data, len);
	buf->free_struct = 1;

	return buf;
}

static bool l_buffer_resize_internal(LBuffer *buf, size_t size) {
	uint8_t *data;

	/* try to realloc data. */
	data = (uint8_t *)realloc(buf->data, sizeof(uint8_t) * size);
	if(data == NULL && size > 0) {
		return false;
	}

	if(buf->head > size) {
		/* buffer is shrinking. */
		if(buf->tail >= size) {
			/* buffer is now empty. */
			buf->tail = 0;
			buf->head = 0;
		} else {
			/* truncate used data. */
			buf->head = size;
		}
	}
	buf->size = size;
	return true;
}

L_INLINE bool l_buffer_grow_head(LBuffer *buf, size_t len) {
	size_t head = buf->head + (len);
	if(head > buf->size) {
		l_buffer_resize_internal((buf), head);
	}
	buf->head = head;
	return true;
}

static void l_buffer_cleanup(LBuffer *buf) {
	if(buf == NULL) return;
	if(buf->data) free(buf->data);
	buf->data = NULL;
	buf->size = 0;
	buf->tail = 0;
	buf->head = 0;
}

void l_buffer_free(LBuffer *buf) {
	if(buf == NULL) return;
	l_buffer_cleanup(buf);
	if(buf->free_struct) {
		free(buf);
	}
}

void l_buffer_reset(LBuffer *buf) {
	buf->tail = 0;
	buf->head = 0;
}

size_t l_buffer_length(LBuffer *buf) {
	return L_BUFFER_LENGTH(buf);
}

bool l_buffer_set_length(LBuffer *buf, size_t len) {
	size_t head = (buf)->tail + len;
	if L_UNLIKELY(head > buf->size) {
		if(!l_buffer_resize_internal(buf, head)) { return false; }
	}
	buf->head = head;
	return true;
}

L_INLINE uint8_t *l_buffer_get_tail(LBuffer *buf, size_t need) {
	if L_UNLIKELY(need > L_BUFFER_LENGTH(buf)) {
		return NULL;
	}
	return buf->data + buf->tail;
}

L_INLINE uint8_t *l_buffer_get_head(LBuffer *buf, size_t need) {
	size_t new_head = buf->head + need;
	if L_UNLIKELY(new_head > buf->size) {
		if L_UNLIKELY(!l_buffer_resize_internal(buf, new_head)) { return NULL; }
	}
	return buf->data + buf->head;
}

uint8_t *l_buffer_sub(LBuffer *buf, size_t off, size_t *plen) {
	size_t end;
	size_t len;

	if(plen) {
		/* get requested length. */
		len = *plen;
		/* if len == 0, then return the whole buffer from `off` to end. */
		/* if (off + len) > head, then truncated the requested length. */
		if(len == 0 || (off + len) > buf->head) {
			*plen = buf->head - off;
		}
	}
	/* if `off` beyond end of the buffer. */
	if(off > buf->head) {
		return NULL;
	}
	return buf->data + off;
}

uint8_t *l_buffer_data(LBuffer *buf) {
	return buf->data;
}

size_t l_buffer_size(LBuffer *buf) {
	return buf->size;
}

bool l_buffer_resize(LBuffer *buf, size_t size) {
	return l_buffer_resize_internal(buf, size);
}

#include "lbuffer_read.c"
#include "lbuffer_append.c"
