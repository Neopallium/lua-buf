#
# Lua bindings for buf
#
cmake_minimum_required(VERSION 2.8)

project(lua-buf C)

set(BUILD_SHARED_LIBS TRUE)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

set(INSTALL_CMOD ${CMAKE_INSTALL_PREFIX}/lib/lua/5.1 CACHE PATH
				"Directory to install Lua binary modules (configure lua via LUA_CPATH)")
set(LUA_NATIVE_OBJECTS_PATH ../LuaNativeObjects CACHE PATH
				"Directory to LuaNativeObjects bindings generator.")
set(USE_PRE_GENERATED_BINDINGS TRUE CACHE BOOL
				"Set this to FALSE to re-generate bindings using LuaNativeObjects")

set(COMMON_CFLAGS "${CFLAGS}")
set(COMMON_LDFLAGS)
set(COMMON_LIBS)

include(FindPkgConfig)
include(CustomMacros)

## Lua 5.1.x
include(FindLua51)
if(NOT ${LUA51_FOUND})
	message(FATAL_ERROR "The FindLua51 module could not find lua :-(")
endif()
set(COMMON_LIBS "${COMMON_LIBS};${LUA_LIBRARIES}")

## LuaNativeObjects
include(LuaNativeObjects)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${LUA_INCLUDE_DIR})

## LuaBuffer
set(LUA_BUFFER_NOBJ_SRC
	buf.nobj.lua
)
set(LUA_BUFFER_SRC
	src/lbuffer.c
	src/lbuffer.h
)

if(${USE_PRE_GENERATED_BINDINGS})
	set(LUA_BUFFER_NOBJ_SRC src/pre_generated-buf.nobj.c)
else()
	# Generate Lua bindings.
	GenLuaNativeObjects(LUA_BUFFER_NOBJ_SRC)
endif()
set(LUA_BUFFER_SRC ${LUA_BUFFER_SRC} ${LUA_BUFFER_NOBJ_SRC})

add_library(lua-buf MODULE ${LUA_BUFFER_SRC})
target_link_libraries(lua-buf ${COMMON_LIBS})
add_target_properties(lua-buf COMPILE_FLAGS "${COMMON_CFLAGS}")
add_target_properties(lua-buf LINK_FLAGS "${LD_FLAGS} ${COMMON_LDFLAGS}")
set_target_properties(lua-buf PROPERTIES PREFIX "")
set_target_properties(lua-buf PROPERTIES OUTPUT_NAME buf)

install(TARGETS lua-buf
        DESTINATION "${INSTALL_CMOD}")

