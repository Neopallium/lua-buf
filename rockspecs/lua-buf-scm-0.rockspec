#!/usr/bin/env lua

package	= 'lua-buf'
version	= 'scm-0'
source	= {
	url	= 'git://github.com/Neopallium/lua-buf.git'
}
description	= {
	summary	= "Mutable buffer object.",
	detailed	= '',
	homepage	= 'https://github.com/Neopallium/lua-buf',
	license	= 'MIT',
	maintainer = "Robert G. Jakabosky",
}
dependencies = {
	'lua >= 5.1',
}
build	= {
	type = "cmake",
	variables = {
	CMAKE_INSTALL_PREFIX = "$(LUAROCKS_PREFIX)",
	INSTALL_CMOD = "$(LUAROCKS_PREFIX)/lib/lua/5.1",
},
}
