lua-buf
=======

Mutable buffer object.

TODO
----

* Export `lua_push*`/`lua_to*` style interface for other C modules to use.  This will allow I/O modules to work directly with LBuffers.

Installing
----------

### Install lua-buf:

	luarocks install https://github.com/Neopallium/lua-buf/raw/master/lua-buf-scm-0.rockspec


To re-generating the bindings
-----------------------------

You will need to install LuaNativeObjects and set the CMake variable `USE_PRE_GENERATED_BINDINGS` to FALSE.
By default CMake will use the pre-generated bindings that are include in the project.

Build Dependencies
------------------

Optional dependency for re-generating Lua bindings from `*.nobj.lua` files:

* [LuaNativeObjects](https://github.com/Neopallium/LuaNativeObjects), this is the bindings generator used to convert the `*.nobj.lua` files into a native Lua module.

