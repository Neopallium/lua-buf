#
# Lua bindings for buf
#
cmake_minimum_required(VERSION 2.8)

project(lua-buf C)

set(BUILD_SHARED_LIBS TRUE)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

set(INSTALL_CMOD share/lua/cmod CACHE PATH "Directory to install Lua binary modules (configure lua via LUA_CPATH)")
set(LUA_NATIVE_OBJECTS_PATH ../LuaNativeObjects CACHE PATH
				"Directory to LuaNativeObjects bindings generator.")
set(USE_PRE_GENERATED_BINDINGS TRUE CACHE BOOL
				"Set this to FALSE to re-generate bindings using LuaNativeObjects")

set(MODULE_CFLAGS "${CFLAGS}")
set(MODULE_LDFLAGS)
set(MODULE_LIBS)

set(LIB_CFLAGS "${CFLAGS}")
set(LIB_LDFLAGS)
set(LIB_LIBS)

include(FindPkgConfig)
include(CustomMacros)

## Lua 5.1.x
include(FindLua51)
if(NOT ${LUA51_FOUND})
	message(FATAL_ERROR "The FindLua51 module could not find lua :-(")
endif()
set(MODULE_LIBS "${MODULE_LIBS};${LUA_LIBRARIES}")

## LuaNativeObjects
include(LuaNativeObjects)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${LUA_INCLUDE_DIR})

## LuaBuffer
set(LBUFFER_SRC
	src/lbuffer.c
	src/lbuffer.h
)
set(LUA_BUFFER_SRC
	buf.nobj.lua
)

if(${USE_PRE_GENERATED_BINDINGS})
	set(LUA_BUFFER_SRC src/pre_generated-buf.nobj.c)
else()
	# Generate Lua bindings.
	GenLuaNativeObjects(LUA_BUFFER_SRC)
endif()

add_library(lbuffer SHARED ${LBUFFER_SRC})
target_link_libraries(lbuffer ${LIB_LIBS})
add_target_properties(lbuffer COMPILE_FLAGS "${LIB_CFLAGS}")
add_target_properties(lbuffer LINK_FLAGS "${LD_FLAGS} ${LIB_LDFLAGS}")

add_library(lua-buf MODULE ${LUA_BUFFER_SRC})
target_link_libraries(lua-buf lbuffer ${MODULE_LIBS})
add_target_properties(lua-buf COMPILE_FLAGS "${MODULE_CFLAGS}")
add_target_properties(lua-buf LINK_FLAGS "${LD_FLAGS} ${MODULE_LDFLAGS}")
set_target_properties(lua-buf PROPERTIES PREFIX "")
set_target_properties(lua-buf PROPERTIES OUTPUT_NAME buf)

install(TARGETS lua-buf
        DESTINATION "${INSTALL_CMOD}")

