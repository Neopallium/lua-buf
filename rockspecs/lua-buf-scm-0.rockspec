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
	type = "builtin",
	modules = {
		buf = {
			sources = {
				"src/lbuffer.c",
				"src/pre_generated-buf.nobj.c",
			},
		}
	}
}
