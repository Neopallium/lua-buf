-- Copyright (c) 2011 by Robert G. Jakabosky <bobby@sharedrealm.com>
--
-- Permission is hereby granted, free of charge, to any person obtaining a copy
-- of this software and associated documentation files (the "Software"), to deal
-- in the Software without restriction, including without limitation the rights
-- to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
-- copies of the Software, and to permit persons to whom the Software is
-- furnished to do so, subject to the following conditions:
--
-- The above copyright notice and this permission notice shall be included in
-- all copies or substantial portions of the Software.
--
-- THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
-- IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
-- FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
-- AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
-- LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
-- OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
-- THE SOFTWARE.

object "LBuffer" {
	-- store the `LBuffer` structure in the Lua userdata object.
	userdata_type = "embed",
	implements "Buffer" {
		implement_method "const_data" {
			c_function = "l_buffer_data"
		},
		implement_method "get_size" {
			c_function = "l_buffer_length"
		},
	},
	implements "MutableBuffer" {
		implement_method "data" {
			c_function = "l_buffer_data"
		},
		implement_method "get_size" {
			c_function = "l_buffer_size"
		},
	},

	include "lbuffer.h",
	ffi_cdef[[
typedef struct LBuffer LBuffer;

typedef uint32_t buflen_t;

struct LBuffer {
	uint8_t   *_data;           /**< Buffer data. */
	buflen_t  _tail;            /**< start offset, tail must always be lower then head. */
	buflen_t  _head;            /**< end offset, head must always be lower then size. */
	buflen_t  _size;            /**< total memory size of data. */
	uint32_t  _free_struct: 1;  /**< Should the LBuffer struct be freed. */
};

]],
	destructor "free" {
		c_method_call "void" "l_buffer_free" {}
	},
	method "__tostring" {
		c_method_call { "char *", "data" } "l_buffer_data" {},
		c_method_call { "size_t", "#data" } "l_buffer_length" {}
	},
	method "tostring" {
		c_method_call { "char *", "data" } "l_buffer_data" {},
		c_method_call { "size_t", "#data" } "l_buffer_length" {}
	},

	method "reset" {
		c_method_call "void" "l_buffer_reset" {}
	},
	method "length" {
		c_method_call { "size_t", "size", ffi_wrap = "tonumber" } "l_buffer_length" {}
	},
	method "__len" {
		c_method_call { "size_t", "size", ffi_wrap = "tonumber" } "l_buffer_length" {}
	},
	method "set_length" {
		c_method_call "bool" "l_buffer_set_length" { "size_t", "len" }
	},
	method "data_ptr" {
		c_method_call "void *" "l_buffer_data" {}
	},
	method "sub" {
		c_method_call { "const char *", "data", length = "len" }
			"l_buffer_sub" { "size_t", "off?", "size_t" , "&len?" },
	},
	method "size" {
		c_method_call { "size_t", "size", ffi_wrap = "tonumber" } "l_buffer_size" {}
	},
	method "resize" {
		c_method_call "bool" "l_buffer_resize" { "size_t", "len" }
	},

	--[[ Read methods. ]]
	method "read_data" {
		c_method_call { "const char *", "data", length = "len" }
			"l_buffer_read_data_len" { "size_t" , "len" },
	},
	method "read_string" {
		c_method_call { "const char *", "str", has_length = true }
			"l_buffer_read_string_len" { "size_t" , "&#str" },
	},
	method "read_uint8" {
		c_method_call "int>2" "l_buffer_read_uint8_t" { "uint8_t", "&num>1" }
	},
	method "read_int8" {
		c_method_call "int>2" "l_buffer_read_uint8_t" { "int8_t", "&num>1" }
	},
	method "read_uint16" {
		c_method_call "int>2" "l_buffer_read_uint16_t" { "uint16_t", "&num>1" }
	},
	method "read_int16" {
		c_method_call "int>2" "l_buffer_read_uint16_t" { "int16_t", "&num>1" }
	},
	method "read_uint32" {
		c_method_call "int>2" "l_buffer_read_uint32_t" { "uint32_t", "&num>1" }
	},
	method "read_int32" {
		c_method_call "int>2" "l_buffer_read_uint32_t" { "int32_t", "&num>1" }
	},
	method "read_uint64" {
		c_method_call "int>2" "l_buffer_read_uint64_t" { "uint64_t", "&num>1" }
	},
	method "read_int64" {
		c_method_call "int>2" "l_buffer_read_uint64_t" { "int64_t", "&num>1" }
	},
	method "read_float" {
		c_method_call "int>2" "l_buffer_read_float" { "float", "&num>1" }
	},
	method "read_double" {
		c_method_call "int>2" "l_buffer_read_double" { "double", "&num>1" }
	},
	method "read_b128_uvar32" {
		c_method_call "int>2" "l_buffer_read_b128_uvar32" { "uint32_t", "&num>1" }
	},
	method "read_b128_var32" {
		c_method_call "int>2" "l_buffer_read_b128_var32" { "int32_t", "&num>1", "bool", "zigzag?" }
	},
	method "read_b128_uvar64" {
		c_method_call "int>2" "l_buffer_read_b128_uvar64" { "uint64_t", "&num>1" }
	},
	method "read_b128_var64" {
		c_method_call "int>2" "l_buffer_read_b128_var64" { "int64_t", "&num>1", "bool", "zigzag?" }
	},

	--[[ Append methods. ]]
	method "append_data" {
		c_method_call "int" "l_buffer_append_data_len" { "const char *", "data", "size_t", "#data" }
	},
	method "append_string" {
		c_method_call "int" "l_buffer_append_string_len" { "const char *", "str", "size_t", "#str" }
	},
	method "append_uint8" {
		c_method_call "int" "l_buffer_append_uint8_t" { "uint8_t", "num" }
	},
	method "append_int8" {
		c_method_call "int" "l_buffer_append_uint8_t" { "int8_t", "num" }
	},
	method "append_uint16" {
		c_method_call "int" "l_buffer_append_uint16_t" { "uint16_t", "num" }
	},
	method "append_int16" {
		c_method_call "int" "l_buffer_append_uint16_t" { "int16_t", "num" }
	},
	method "append_uint32" {
		c_method_call "int" "l_buffer_append_uint32_t" { "uint32_t", "num" }
	},
	method "append_int32" {
		c_method_call "int" "l_buffer_append_uint32_t" { "int32_t", "num" }
	},
	method "append_uint64" {
		c_method_call "int" "l_buffer_append_uint64_t" { "uint64_t", "num" }
	},
	method "append_int64" {
		c_method_call "int" "l_buffer_append_uint64_t" { "int64_t", "num" }
	},
	method "append_float" {
		c_method_call "int" "l_buffer_append_float" { "float", "num" }
	},
	method "append_double" {
		c_method_call "int" "l_buffer_append_double" { "double", "num" }
	},
	method "append_b128_uvar32" {
		c_method_call "int" "l_buffer_append_b128_uvar32" { "uint32_t", "num" }
	},
	method "append_b128_var32" {
		c_method_call "int" "l_buffer_append_b128_var32" { "int32_t", "num", "bool", "zigzag?" }
	},
	method "append_b128_uvar64" {
		c_method_call "int" "l_buffer_append_b128_uvar64" { "uint64_t", "num" }
	},
	method "append_b128_var64" {
		c_method_call "int" "l_buffer_append_b128_var64" { "int64_t", "num", "bool", "zigzag?" }
	},
}
