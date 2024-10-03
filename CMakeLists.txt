#
# Lua bindings for buf
#
cmake_minimum_required(VERSION 3.18)

project(lua-buf C)

set(BUILD_SHARED_LIBS TRUE)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

set(INSTALL_LMOD ${CMAKE_INSTALL_PREFIX}/share/lua/ CACHE PATH
				"Directory to install Lua source modules (configure lua via LUA_PATH)")
set(INSTALL_CMOD ${CMAKE_INSTALL_PREFIX}/lib/lua/ CACHE PATH
				"Directory to install Lua binary modules (configure lua via LUA_CPATH)")

set(COMMON_CFLAGS "${CFLAGS}")
set(COMMON_LDFLAGS)
set(COMMON_LIBS)

include(FindPkgConfig)
include(CustomMacros)

## Lua 5.x
include(FindLua)
if(NOT ${LUA_FOUND})
    message(FATAL_ERROR "The FindLua module could not find lua :-(")
endif()
set(COMMON_LIBS "${COMMON_LIBS};${LUA_LIBRARIES}")
if(WIN32)
	set(COMMON_CFLAGS "${COMMON_CFLAGS} -I${LUA_INCLUDE_DIR}")
	set(COMMON_LDFLAGS "${COMMON_LDFLAGS} ${LUA_LIBRARY}")
endif()
## MAC OSX needs extra linker flags
if(APPLE)
	set(COMMON_LDFLAGS "${COMMON_LDFLAGS} -undefined dynamic_lookup")
endif()

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

