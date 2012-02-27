
-- make generated variable nicer.
set_variable_format "%s"

c_module "buf" {

-- enable FFI bindings support.
luajit_ffi = true,
luajit_ffi_load_cmodule = true,

subfiles {
"src/lbuffer.nobj.lua",
},

c_function "new" {
	var_in{ "<any>", "size_or_data"},
	var_out{ "!LBuffer *", "this" },
	c_source[[
	LBuffer buf;
	const uint8_t *data = NULL;
	size_t len = 0;
	int ltype = lua_type(L, ${size_or_data::idx});

	if(ltype == LUA_TSTRING) {
		data = lua_tolstring(L, ${size_or_data::idx}, &len);
	} else if(ltype == LUA_TNUMBER) {
		len = lua_tointeger(L, ${size_or_data::idx});
	}

	${this} = &buf;
	l_buffer_init(${this}, data, len);
]],
},
}

