
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
	c_source[[
	int ltype = lua_type(L, ${size_or_data::idx});

	if(ltype == LUA_TSTRING) {
		data = lua_tolstring(L, ${size_or_data::idx}, &len);
	} else if(ltype == LUA_TNUMBER) {
		len = lua_tointeger(L, ${size_or_data::idx});
	}
]],
	ffi_source[[
	local ltype = type(${size_or_data})

	if ltype == 'string' then
		data = ${size_or_data}
		len = #data
	elseif ltype == 'number' then
		len = ${size_or_data}
	end
]],
	c_call "void" "l_buffer_init" { "!LBuffer *", "buf>1", "const uint8_t *", "(data)", "size_t", "(len)" }
},
}

