/***********************************************************************************************
************************************************************************************************
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!!!!! Warning this file was generated from a set of *.nobj.lua definition files !!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
************************************************************************************************
***********************************************************************************************/

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

/* some Lua 5.0 compatibility support. */
#if !defined(lua_pushliteral)
#define lua_pushliteral(L, s) lua_pushstring(L, "" s, (sizeof(s)/sizeof(char))-1)
#endif

#if !defined(LUA_VERSION_NUM)
#define lua_pushinteger(L, n) lua_pushnumber(L, (lua_Number)n)
#define luaL_Reg luaL_reg
#endif

/* some Lua 5.1 compatibility support. */
#if !defined(LUA_VERSION_NUM) || (LUA_VERSION_NUM == 501)
/*
** Adapted from Lua 5.2.0
*/
static void luaL_setfuncs (lua_State *L, const luaL_Reg *l, int nup) {
  luaL_checkstack(L, nup, "too many upvalues");
  for (; l->name != NULL; l++) {  /* fill the table with given functions */
    int i;
    for (i = 0; i < nup; i++)  /* copy upvalues to the top */
      lua_pushvalue(L, -nup);
    lua_pushstring(L, l->name);
    lua_pushcclosure(L, l->func, nup);  /* closure with those upvalues */
    lua_settable(L, -(nup + 3));
  }
  lua_pop(L, nup);  /* remove upvalues */
}

#define lua_load_no_mode(L, reader, data, source) \
	lua_load(L, reader, data, source)

#define lua_rawlen(L, idx) lua_objlen(L, idx)

#endif

#if LUA_VERSION_NUM == 502

#define lua_load_no_mode(L, reader, data, source) \
	lua_load(L, reader, data, source, NULL)

static int luaL_typerror (lua_State *L, int narg, const char *tname) {
  const char *msg = lua_pushfstring(L, "%s expected, got %s",
                                    tname, luaL_typename(L, narg));
  return luaL_argerror(L, narg, msg);
}

#endif

#define REG_PACKAGE_IS_CONSTRUCTOR 0
#define REG_MODULES_AS_GLOBALS 0
#define REG_OBJECTS_AS_GLOBALS 0
#define OBJ_DATA_HIDDEN_METATABLE 1
#define USE_FIELD_GET_SET_METHODS 0
#define LUAJIT_FFI 1


#include "lbuffer.h"



#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#ifdef _MSC_VER
#define __WINDOWS__
#else
#if defined(_WIN32)
#define __WINDOWS__
#endif
#endif

#ifdef __WINDOWS__

/* for MinGW32 compiler need to include <stdint.h> */
#ifdef __GNUC__
#include <stdint.h>
#include <stdbool.h>
#else

/* define some standard types missing on Windows. */
#ifndef __INT32_MAX__
typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;
#endif
#ifndef __INT64_MAX__
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
#endif
typedef int bool;
#ifndef true
#define true 1
#endif
#ifndef false
#define false 0
#endif

#endif

/* wrap strerror_s(). */
#ifdef __GNUC__
#ifndef strerror_r
#define strerror_r(errno, buf, buflen) do { \
	strncpy((buf), strerror(errno), (buflen)-1); \
	(buf)[(buflen)-1] = '\0'; \
} while(0)
#endif
#else
#ifndef strerror_r
#define strerror_r(errno, buf, buflen) strerror_s((buf), (buflen), (errno))
#endif
#endif

#define FUNC_UNUSED

#define LUA_NOBJ_API __declspec(dllexport)

#else

#define LUA_NOBJ_API LUALIB_API

#include <stdint.h>
#include <stdbool.h>

#define FUNC_UNUSED __attribute__((unused))

#endif

#if defined(__GNUC__) && (__GNUC__ >= 4)
#define assert_obj_type(type, obj) \
	assert(__builtin_types_compatible_p(typeof(obj), type *))
#else
#define assert_obj_type(type, obj)
#endif

void *nobj_realloc(void *ptr, size_t osize, size_t nsize);

void *nobj_realloc(void *ptr, size_t osize, size_t nsize) {
	(void)osize;
	if(0 == nsize) {
		free(ptr);
		return NULL;
	}
	return realloc(ptr, nsize);
}

#define obj_type_free(type, obj) do { \
	assert_obj_type(type, obj); \
	nobj_realloc((obj), sizeof(type), 0); \
} while(0)

#define obj_type_new(type, obj) do { \
	assert_obj_type(type, obj); \
	(obj) = nobj_realloc(NULL, 0, sizeof(type)); \
} while(0)

typedef struct obj_type obj_type;

typedef void (*base_caster_t)(void **obj);

typedef void (*dyn_caster_t)(void **obj, obj_type **type);

#define OBJ_TYPE_FLAG_WEAK_REF (1<<0)
#define OBJ_TYPE_SIMPLE (1<<1)
#define OBJ_TYPE_IMPORT (1<<2)
struct obj_type {
	dyn_caster_t    dcaster;  /**< caster to support casting to sub-objects. */
	int32_t         id;       /**< type's id. */
	uint32_t        flags;    /**< type's flags (weak refs) */
	const char      *name;    /**< type's object name. */
};

typedef struct obj_base {
	int32_t        id;
	base_caster_t  bcaster;
} obj_base;

typedef enum obj_const_type {
	CONST_UNKOWN    = 0,
	CONST_BOOLEAN   = 1,
	CONST_NUMBER    = 2,
	CONST_STRING    = 3
} obj_const_type;

typedef struct obj_const {
	const char      *name;    /**< constant's name. */
	const char      *str;
	double          num;
	obj_const_type  type;
} obj_const;

typedef enum obj_field_type {
	TYPE_UNKOWN    = 0,
	TYPE_UINT8     = 1,
	TYPE_UINT16    = 2,
	TYPE_UINT32    = 3,
	TYPE_UINT64    = 4,
	TYPE_INT8      = 5,
	TYPE_INT16     = 6,
	TYPE_INT32     = 7,
	TYPE_INT64     = 8,
	TYPE_DOUBLE    = 9,
	TYPE_FLOAT     = 10,
	TYPE_STRING    = 11
} obj_field_type;

typedef struct obj_field {
	const char      *name;  /**< field's name. */
	uint32_t        offset; /**< offset to field's data. */
	obj_field_type  type;   /**< field's data type. */
	uint32_t        flags;  /**< is_writable:1bit */
} obj_field;

typedef enum {
	REG_OBJECT,
	REG_PACKAGE,
	REG_META,
} module_reg_type;

typedef struct reg_impl {
	const char *if_name;
	const void *impl;
} reg_impl;

typedef struct reg_sub_module {
	obj_type        *type;
	module_reg_type req_type;
	const luaL_Reg  *pub_funcs;
	const luaL_Reg  *methods;
	const luaL_Reg  *metas;
	const obj_base  *bases;
	const obj_field *fields;
	const obj_const *constants;
	const reg_impl  *implements;
	int             bidirectional_consts;
} reg_sub_module;

#define OBJ_UDATA_FLAG_OWN (1<<0)
#define OBJ_UDATA_FLAG_LOOKUP (1<<1)
#define OBJ_UDATA_LAST_FLAG (OBJ_UDATA_FLAG_LOOKUP)
typedef struct obj_udata {
	void     *obj;
	uint32_t flags;  /**< lua_own:1bit */
} obj_udata;

/* use static pointer as key to weak userdata table. */
static char obj_udata_weak_ref_key[] = "obj_udata_weak_ref_key";

/* use static pointer as key to module's private table. */
static char obj_udata_private_key[] = "obj_udata_private_key";

#if LUAJIT_FFI
typedef int (*ffi_export_func_t)(void);
typedef struct ffi_export_symbol {
	const char *name;
	union {
	void               *data;
	ffi_export_func_t  func;
	} sym;
} ffi_export_symbol;
#endif





static obj_type obj_types[] = {
#define obj_type_id_LBuffer 0
#define obj_type_LBuffer (obj_types[obj_type_id_LBuffer])
  { NULL, 0, OBJ_TYPE_SIMPLE, "LBuffer" },
  {NULL, -1, 0, NULL},
};


#if LUAJIT_FFI

/* nobj_ffi_support_enabled_hint should be set to 1 when FFI support is enabled in at-least one
 * instance of a LuaJIT state.  It should never be set back to 0. */
static int nobj_ffi_support_enabled_hint = 0;
static const char nobj_ffi_support_key[] = "LuaNativeObject_FFI_SUPPORT";
static const char nobj_check_ffi_support_code[] =
"local stat, ffi=pcall(require,\"ffi\")\n" /* try loading LuaJIT`s FFI module. */
"if not stat then return false end\n"
"return true\n";

static int nobj_check_ffi_support(lua_State *L) {
	int rc;
	int err;

	/* check if ffi test has already been done. */
	lua_pushstring(L, nobj_ffi_support_key);
	lua_rawget(L, LUA_REGISTRYINDEX);
	if(!lua_isnil(L, -1)) {
		rc = lua_toboolean(L, -1);
		lua_pop(L, 1);
		/* use results of previous check. */
		goto finished;
	}
	lua_pop(L, 1); /* pop nil. */

	err = luaL_loadbuffer(L, nobj_check_ffi_support_code,
		sizeof(nobj_check_ffi_support_code) - 1, nobj_ffi_support_key);
	if(0 == err) {
		err = lua_pcall(L, 0, 1, 0);
	}
	if(err) {
		const char *msg = "<err not a string>";
		if(lua_isstring(L, -1)) {
			msg = lua_tostring(L, -1);
		}
		printf("Error when checking for FFI-support: %s\n", msg);
		lua_pop(L, 1); /* pop error message. */
		return 0;
	}
	/* check results of test. */
	rc = lua_toboolean(L, -1);
	lua_pop(L, 1); /* pop results. */
		/* cache results. */
	lua_pushstring(L, nobj_ffi_support_key);
	lua_pushboolean(L, rc);
	lua_rawset(L, LUA_REGISTRYINDEX);

finished:
	/* turn-on hint that there is FFI code enabled. */
	if(rc) {
		nobj_ffi_support_enabled_hint = 1;
	}

	return rc;
}

typedef struct {
	const char **ffi_init_code;
	int offset;
} nobj_reader_state;

static const char *nobj_lua_Reader(lua_State *L, void *data, size_t *size) {
	nobj_reader_state *state = (nobj_reader_state *)data;
	const char *ptr;

	(void)L;
	ptr = state->ffi_init_code[state->offset];
	if(ptr != NULL) {
		*size = strlen(ptr);
		state->offset++;
	} else {
		*size = 0;
	}
	return ptr;
}

static int nobj_try_loading_ffi(lua_State *L, const char *ffi_mod_name,
		const char *ffi_init_code[], const ffi_export_symbol *ffi_exports, int priv_table)
{
	nobj_reader_state state = { ffi_init_code, 0 };
	int err;

	/* export symbols to priv_table. */
	while(ffi_exports->name != NULL) {
		lua_pushstring(L, ffi_exports->name);
		lua_pushlightuserdata(L, ffi_exports->sym.data);
		lua_settable(L, priv_table);
		ffi_exports++;
	}
	err = lua_load_no_mode(L, nobj_lua_Reader, &state, ffi_mod_name);
	if(0 == err) {
		lua_pushvalue(L, -2); /* dup C module's table. */
		lua_pushvalue(L, priv_table); /* move priv_table to top of stack. */
		lua_remove(L, priv_table);
		lua_pushvalue(L, LUA_REGISTRYINDEX);
		err = lua_pcall(L, 3, 0, 0);
	}
	if(err) {
		const char *msg = "<err not a string>";
		if(lua_isstring(L, -1)) {
			msg = lua_tostring(L, -1);
		}
		printf("Failed to install FFI-based bindings: %s\n", msg);
		lua_pop(L, 1); /* pop error message. */
	}
	return err;
}
#endif


typedef struct {
	void *impl;
	void *obj;
} obj_implement;

static FUNC_UNUSED void *obj_implement_luaoptional(lua_State *L, int _index, void **impl, char *if_name) {
	void *ud;
	if(lua_isnoneornil(L, _index)) {
		return NULL;
	}
	/* get the implements table for this interface. */
	lua_pushlightuserdata(L, if_name);
	lua_rawget(L, LUA_REGISTRYINDEX);

	/* get pointer to userdata value & check if it is a userdata value. */
	ud = (obj_implement *)lua_touserdata(L, _index);
	if(ud != NULL) {
		/* get the userdata's metatable */
		if(lua_getmetatable(L, _index)) {
			/* lookup metatable in interface table for this object's implementation of the interface. */
			lua_gettable(L, -2);
		} else {
			/* no metatable. */
			goto no_interface;
		}
#if LUAJIT_FFI
	} else if(nobj_ffi_support_enabled_hint) { /* handle cdata. */
		/* get cdata interface check function from interface table. */
		lua_getfield(L, -1, "cdata");
		if(lua_isfunction(L, -1)) {
			/* pass cdata to function, return value should be an implmentation. */
			lua_pushvalue(L, _index);
			lua_call(L, 1, 1);
			/* get pointer to cdata. */
			ud = (void *)lua_topointer(L, _index);
		} else {
			lua_pop(L, 1); /* pop non-function. */
			goto no_interface;
		}
#endif
	} else {
		goto no_interface;
	}

	if(!lua_isnil(L, -1)) {
		*impl = lua_touserdata(L, -1);
		lua_pop(L, 2); /* pop interface table & implementation. */
		/* object implements interface. */
		return ud;
	} else {
		lua_pop(L, 1); /* pop nil. */
	}
no_interface:
	lua_pop(L, 1); /* pop interface table. */
	return NULL;
}

static FUNC_UNUSED void *obj_implement_luacheck(lua_State *L, int _index, void **impl, char *type) {
	void *ud = obj_implement_luaoptional(L, _index, impl, type);
	if(ud == NULL) {
#define ERROR_BUFFER_SIZE 256
		char buf[ERROR_BUFFER_SIZE];
		snprintf(buf, ERROR_BUFFER_SIZE-1,"Expected object with %s interface", type);
		/* value doesn't implement this interface. */
		luaL_argerror(L, _index, buf);
	}
	return ud;
}

/* use static pointer as key to interfaces table. (version 1.0) */
static char obj_interfaces_table_key[] = "obj_interfaces<1.0>_table_key";

static void obj_get_global_interfaces_table(lua_State *L) {
	/* get global interfaces table. */
	lua_getfield(L, LUA_REGISTRYINDEX, obj_interfaces_table_key);
	if(lua_isnil(L, -1)) {
		/* Need to create global interfaces table. */
		lua_pop(L, 1); /* pop nil */
		lua_createtable(L, 0, 4); /* 0 size array part, small hash part. */
		lua_pushvalue(L, -1); /* dup table. */
		/* store interfaces table in Lua registery. */
		lua_setfield(L, LUA_REGISTRYINDEX, obj_interfaces_table_key);
	}
}

static void obj_get_interface(lua_State *L, const char *name, int global_if_tab) {
	/* get a interface's implementation table */
	lua_getfield(L, global_if_tab, name);
	if(lua_isnil(L, -1)) {
		lua_pop(L, 1); /* pop nil */
		/* new interface. (i.e. no object implement it yet.)
		 *
		 * create an empty table for this interface that will be used when an
		 * implementation is registered for this interface.
		 */
		lua_createtable(L, 0, 2); /* 0 size array part, small hash part. */
		lua_pushvalue(L, -1); /* dup table. */
		lua_setfield(L, global_if_tab, name); /* store interface in global interfaces table. */
	}
}

static int obj_get_userdata_interface(lua_State *L) {
	/* get the userdata's metatable */
	if(lua_getmetatable(L, 2)) {
		/* lookup metatable in interface table for the userdata's implementation of the interface. */
		lua_gettable(L, 1);
		if(!lua_isnil(L, -1)) {
			/* return the implementation. */
			return 1;
		}
	}
	/* no metatable or no implementation. */
	return 0;
}

static void obj_interface_register(lua_State *L, char *name, int global_if_tab) {
	/* get the table of implementations for this interface. */
	obj_get_interface(L, name, global_if_tab);

	/* check for 'userdata' function. */
	lua_getfield(L, -1, "userdata");
	if(lua_isnil(L, -1)) {
		lua_pop(L, 1); /* pop nil. */
		/* add C function for getting a userdata's implementation. */
		lua_pushcfunction(L, obj_get_userdata_interface);
		lua_setfield(L, -2, "userdata");
	} else {
		/* already have function. */
		lua_pop(L, 1); /* pop C function. */
	}
	/* we are going to use a lightuserdata pointer for fast lookup of the interface's impl. table. */
	lua_pushlightuserdata(L, name);
	lua_insert(L, -2);
	lua_settable(L, LUA_REGISTRYINDEX);
}

static void obj_register_interfaces(lua_State *L, char *interfaces[]) {
	int i;
	int if_tab;
	/* get global interfaces table. */
	obj_get_global_interfaces_table(L);
	if_tab = lua_gettop(L);

	for(i = 0; interfaces[i] != NULL ; i++) {
		obj_interface_register(L, interfaces[i], if_tab);
	}
	lua_pop(L, 1); /* pop global interfaces table. */
}

static void obj_type_register_implement(lua_State *L, const reg_impl *impl, int global_if_tab, int mt_tab) {
	/* get the table of implementations for this interface. */
	obj_get_interface(L, impl->if_name, global_if_tab);

	/* register object's implement in the interface table. */
	lua_pushvalue(L, mt_tab);
	lua_pushlightuserdata(L, (void *)impl->impl);
	lua_settable(L, -3);

	lua_pop(L, 1); /* pop inteface table. */
}

static void obj_type_register_implements(lua_State *L, const reg_impl *impls) {
	int if_tab;
	int mt_tab;
	/* get absolute position of object's metatable. */
	mt_tab = lua_gettop(L);
	/* get global interfaces table. */
	obj_get_global_interfaces_table(L);
	if_tab = lua_gettop(L);

	for(; impls->if_name != NULL ; impls++) {
		obj_type_register_implement(L, impls, if_tab, mt_tab);
	}
	lua_pop(L, 1); /* pop global interfaces table. */
}

#ifndef REG_PACKAGE_IS_CONSTRUCTOR
#define REG_PACKAGE_IS_CONSTRUCTOR 1
#endif

#ifndef REG_MODULES_AS_GLOBALS
#define REG_MODULES_AS_GLOBALS 0
#endif

/* For Lua 5.2 don't register modules as globals. */
#if LUA_VERSION_NUM == 502
#undef REG_MODULES_AS_GLOBALS
#define REG_MODULES_AS_GLOBALS 0
#endif

#ifndef REG_OBJECTS_AS_GLOBALS
#define REG_OBJECTS_AS_GLOBALS 0
#endif

#ifndef OBJ_DATA_HIDDEN_METATABLE
#define OBJ_DATA_HIDDEN_METATABLE 1
#endif

static FUNC_UNUSED int obj_import_external_type(lua_State *L, obj_type *type) {
	/* find the external type's metatable using it's name. */
	lua_pushstring(L, type->name);
	lua_rawget(L, LUA_REGISTRYINDEX); /* external type's metatable. */
	if(!lua_isnil(L, -1)) {
		/* found it.  Now we will map our 'type' pointer to the metatable. */
		/* REGISTERY[lightuserdata<type>] = REGISTERY[type->name] */
		lua_pushlightuserdata(L, type); /* use our 'type' pointer as lookup key. */
		lua_pushvalue(L, -2); /* dup. type's metatable. */
		lua_rawset(L, LUA_REGISTRYINDEX); /* save external type's metatable. */
		/* NOTE: top of Lua stack still has the type's metatable. */
		return 1;
	} else {
		lua_pop(L, 1); /* pop nil. */
	}
	return 0;
}

static FUNC_UNUSED int obj_import_external_ffi_type(lua_State *L, obj_type *type) {
	/* find the external type's metatable using it's name. */
	lua_pushstring(L, type->name);
	lua_rawget(L, LUA_REGISTRYINDEX); /* external type's metatable. */
	if(!lua_isnil(L, -1)) {
		/* found it.  Now we will map our 'type' pointer to the C check function. */
		/* _priv_table[lightuserdata<type>] = REGISTERY[type->name].c_check */
		lua_getfield(L, -1, "c_check");
		lua_remove(L, -2); /* remove metatable. */
		if(lua_isfunction(L, -1)) {
			lua_pushlightuserdata(L, type); /* use our 'type' pointer as lookup key. */
			lua_pushvalue(L, -2); /* dup. check function */
			lua_rawset(L, -4); /* save check function to module's private table. */
			/* NOTE: top of Lua stack still has the type's C check function. */
			return 1;
		} else {
			lua_pop(L, 1); /* pop non function value. */
		}
	} else {
		lua_pop(L, 1); /* pop nil. */
	}
	return 0;
}

static FUNC_UNUSED obj_udata *obj_udata_toobj(lua_State *L, int _index) {
	obj_udata *ud;
	size_t len;

	/* make sure it's a userdata value. */
	ud = (obj_udata *)lua_touserdata(L, _index);
	if(ud == NULL) {
		luaL_typerror(L, _index, "userdata"); /* is not a userdata value. */
	}
	/* verify userdata size. */
	len = lua_rawlen(L, _index);
	if(len != sizeof(obj_udata)) {
		/* This shouldn't be possible */
		luaL_error(L, "invalid userdata size: size=%d, expected=%d", len, sizeof(obj_udata));
	}
	return ud;
}

static FUNC_UNUSED int obj_udata_is_compatible(lua_State *L, obj_udata *ud, void **obj, base_caster_t *caster, obj_type *type) {
	obj_base *base;
	obj_type *ud_type;
	lua_pushlightuserdata(L, type);
	lua_rawget(L, LUA_REGISTRYINDEX); /* type's metatable. */
recheck_metatable:
	if(lua_rawequal(L, -1, -2)) {
		*obj = ud->obj;
		/* same type no casting needed. */
		return 1;
	} else if(lua_isnil(L, -1)) {
		lua_pop(L, 1); /* pop nil. */
		if((type->flags & OBJ_TYPE_IMPORT) == 0) {
			/* can't resolve internal type. */
			luaL_error(L, "Unknown object type(id=%d, name=%s)", type->id, type->name);
		}
		/* try to import external type. */
		if(obj_import_external_type(L, type)) {
			/* imported type, re-try metatable check. */
			goto recheck_metatable;
		}
		/* External type not yet available, so the object can't be compatible. */
	} else {
		/* Different types see if we can cast to the required type. */
		lua_rawgeti(L, -2, type->id);
		base = lua_touserdata(L, -1);
		lua_pop(L, 1); /* pop obj_base or nil */
		if(base != NULL) {
			*caster = base->bcaster;
			/* get the obj_type for this userdata. */
			lua_pushliteral(L, ".type");
			lua_rawget(L, -3); /* type's metatable. */
			ud_type = lua_touserdata(L, -1);
			lua_pop(L, 1); /* pop obj_type or nil */
			if(base == NULL) {
				luaL_error(L, "bad userdata, missing type info.");
				return 0;
			}
			/* check if userdata is a simple object. */
			if(ud_type->flags & OBJ_TYPE_SIMPLE) {
				*obj = ud;
			} else {
				*obj = ud->obj;
			}
			return 1;
		}
	}
	return 0;
}

static FUNC_UNUSED obj_udata *obj_udata_luacheck_internal(lua_State *L, int _index, void **obj, obj_type *type, int not_delete) {
	obj_udata *ud;
	base_caster_t caster = NULL;
	/* make sure it's a userdata value. */
	ud = (obj_udata *)lua_touserdata(L, _index);
	if(ud != NULL) {
		/* check object type by comparing metatables. */
		if(lua_getmetatable(L, _index)) {
			if(obj_udata_is_compatible(L, ud, obj, &(caster), type)) {
				lua_pop(L, 2); /* pop both metatables. */
				/* apply caster function if needed. */
				if(caster != NULL && *obj != NULL) {
					caster(obj);
				}
				/* check object pointer. */
				if(*obj == NULL) {
					if(not_delete) {
						luaL_error(L, "null %s", type->name); /* object was garbage collected? */
					}
					return NULL;
				}
				return ud;
			}
		}
	} else if(!lua_isnoneornil(L, _index)) {
		/* handle cdata. */
		/* get private table. */
		lua_pushlightuserdata(L, obj_udata_private_key);
		lua_rawget(L, LUA_REGISTRYINDEX); /* private table. */
		/* get cdata type check function from private table. */
		lua_pushlightuserdata(L, type);
		lua_rawget(L, -2);

		/* check for function. */
		if(!lua_isnil(L, -1)) {
got_check_func:
			/* pass cdata value to type checking function. */
			lua_pushvalue(L, _index);
			lua_call(L, 1, 1);
			if(!lua_isnil(L, -1)) {
				/* valid type get pointer from cdata. */
				lua_pop(L, 2);
				*obj = *(void **)lua_topointer(L, _index);
				return ud;
			}
			lua_pop(L, 2);
		} else {
			lua_pop(L, 1); /* pop nil. */
			if(type->flags & OBJ_TYPE_IMPORT) {
				/* try to import external ffi type. */
				if(obj_import_external_ffi_type(L, type)) {
					/* imported type. */
					goto got_check_func;
				}
				/* External type not yet available, so the object can't be compatible. */
			}
		}
	}
	if(not_delete) {
		luaL_typerror(L, _index, type->name); /* is not a userdata value. */
	}
	return NULL;
}

static FUNC_UNUSED void *obj_udata_luacheck(lua_State *L, int _index, obj_type *type) {
	void *obj = NULL;
	obj_udata_luacheck_internal(L, _index, &(obj), type, 1);
	return obj;
}

static FUNC_UNUSED void *obj_udata_luaoptional(lua_State *L, int _index, obj_type *type) {
	void *obj = NULL;
	if(lua_isnoneornil(L, _index)) {
		return obj;
	}
	obj_udata_luacheck_internal(L, _index, &(obj), type, 1);
	return obj;
}

static FUNC_UNUSED void *obj_udata_luadelete(lua_State *L, int _index, obj_type *type, int *flags) {
	void *obj;
	obj_udata *ud = obj_udata_luacheck_internal(L, _index, &(obj), type, 0);
	if(ud == NULL) return NULL;
	*flags = ud->flags;
	/* null userdata. */
	ud->obj = NULL;
	ud->flags = 0;
	/* clear the metatable in invalidate userdata. */
	lua_pushnil(L);
	lua_setmetatable(L, _index);
	return obj;
}

static FUNC_UNUSED void obj_udata_luapush(lua_State *L, void *obj, obj_type *type, int flags) {
	obj_udata *ud;
	/* convert NULL's into Lua nil's. */
	if(obj == NULL) {
		lua_pushnil(L);
		return;
	}
#if LUAJIT_FFI
	lua_pushlightuserdata(L, type);
	lua_rawget(L, LUA_REGISTRYINDEX); /* type's metatable. */
	if(nobj_ffi_support_enabled_hint && lua_isfunction(L, -1)) {
		/* call special FFI "void *" to FFI object convertion function. */
		lua_pushlightuserdata(L, obj);
		lua_pushinteger(L, flags);
		lua_call(L, 2, 1);
		return;
	}
#endif
	/* check for type caster. */
	if(type->dcaster) {
		(type->dcaster)(&obj, &type);
	}
	/* create new userdata. */
	ud = (obj_udata *)lua_newuserdata(L, sizeof(obj_udata));
	ud->obj = obj;
	ud->flags = flags;
	/* get obj_type metatable. */
#if LUAJIT_FFI
	lua_insert(L, -2); /* move userdata below metatable. */
#else
	lua_pushlightuserdata(L, type);
	lua_rawget(L, LUA_REGISTRYINDEX); /* type's metatable. */
#endif
	lua_setmetatable(L, -2);
}

static FUNC_UNUSED void *obj_udata_luadelete_weak(lua_State *L, int _index, obj_type *type, int *flags) {
	void *obj;
	obj_udata *ud = obj_udata_luacheck_internal(L, _index, &(obj), type, 0);
	if(ud == NULL) return NULL;
	*flags = ud->flags;
	/* null userdata. */
	ud->obj = NULL;
	ud->flags = 0;
	/* clear the metatable in invalidate userdata. */
	lua_pushnil(L);
	lua_setmetatable(L, _index);
	/* get objects weak table. */
	lua_pushlightuserdata(L, obj_udata_weak_ref_key);
	lua_rawget(L, LUA_REGISTRYINDEX); /* weak ref table. */
	/* remove object from weak table. */
	lua_pushlightuserdata(L, obj);
	lua_pushnil(L);
	lua_rawset(L, -3);
	return obj;
}

static FUNC_UNUSED void obj_udata_luapush_weak(lua_State *L, void *obj, obj_type *type, int flags) {
	obj_udata *ud;

	/* convert NULL's into Lua nil's. */
	if(obj == NULL) {
		lua_pushnil(L);
		return;
	}
	/* check for type caster. */
	if(type->dcaster) {
		(type->dcaster)(&obj, &type);
	}
	/* get objects weak table. */
	lua_pushlightuserdata(L, obj_udata_weak_ref_key);
	lua_rawget(L, LUA_REGISTRYINDEX); /* weak ref table. */
	/* lookup userdata instance from pointer. */
	lua_pushlightuserdata(L, obj);
	lua_rawget(L, -2);
	if(!lua_isnil(L, -1)) {
		lua_remove(L, -2);     /* remove objects table. */
		return;
	}
	lua_pop(L, 1);  /* pop nil. */

#if LUAJIT_FFI
	lua_pushlightuserdata(L, type);
	lua_rawget(L, LUA_REGISTRYINDEX); /* type's metatable. */
	if(nobj_ffi_support_enabled_hint && lua_isfunction(L, -1)) {
		lua_remove(L, -2);
		/* call special FFI "void *" to FFI object convertion function. */
		lua_pushlightuserdata(L, obj);
		lua_pushinteger(L, flags);
		lua_call(L, 2, 1);
		return;
	}
#endif
	/* create new userdata. */
	ud = (obj_udata *)lua_newuserdata(L, sizeof(obj_udata));

	/* init. obj_udata. */
	ud->obj = obj;
	ud->flags = flags;
	/* get obj_type metatable. */
#if LUAJIT_FFI
	lua_insert(L, -2); /* move userdata below metatable. */
#else
	lua_pushlightuserdata(L, type);
	lua_rawget(L, LUA_REGISTRYINDEX); /* type's metatable. */
#endif
	lua_setmetatable(L, -2);

	/* add weak reference to object. */
	lua_pushlightuserdata(L, obj); /* push object pointer as the 'key' */
	lua_pushvalue(L, -2);          /* push object's udata */
	lua_rawset(L, -4);             /* add weak reference to object. */
	lua_remove(L, -2);     /* remove objects table. */
}

/* default object equal method. */
static FUNC_UNUSED int obj_udata_default_equal(lua_State *L) {
	obj_udata *ud1 = obj_udata_toobj(L, 1);
	obj_udata *ud2 = obj_udata_toobj(L, 2);

	lua_pushboolean(L, (ud1->obj == ud2->obj));
	return 1;
}

/* default object tostring method. */
static FUNC_UNUSED int obj_udata_default_tostring(lua_State *L) {
	obj_udata *ud = obj_udata_toobj(L, 1);

	/* get object's metatable. */
	lua_getmetatable(L, 1);
	lua_remove(L, 1); /* remove userdata. */
	/* get the object's name from the metatable */
	lua_getfield(L, 1, ".name");
	lua_remove(L, 1); /* remove metatable */
	/* push object's pointer */
	lua_pushfstring(L, ": %p, flags=%d", ud->obj, ud->flags);
	lua_concat(L, 2);

	return 1;
}

/*
 * Simple userdata objects.
 */
static FUNC_UNUSED void *obj_simple_udata_toobj(lua_State *L, int _index) {
	void *ud;

	/* make sure it's a userdata value. */
	ud = lua_touserdata(L, _index);
	if(ud == NULL) {
		luaL_typerror(L, _index, "userdata"); /* is not a userdata value. */
	}
	return ud;
}

static FUNC_UNUSED void * obj_simple_udata_luacheck(lua_State *L, int _index, obj_type *type) {
	void *ud;
	/* make sure it's a userdata value. */
	ud = lua_touserdata(L, _index);
	if(ud != NULL) {
		/* check object type by comparing metatables. */
		if(lua_getmetatable(L, _index)) {
			lua_pushlightuserdata(L, type);
			lua_rawget(L, LUA_REGISTRYINDEX); /* type's metatable. */
recheck_metatable:
			if(lua_rawequal(L, -1, -2)) {
				lua_pop(L, 2); /* pop both metatables. */
				return ud;
			} else if(lua_isnil(L, -1)) {
				lua_pop(L, 1); /* pop nil. */
				if((type->flags & OBJ_TYPE_IMPORT) == 0) {
					/* can't resolve internal type. */
					luaL_error(L, "Unknown object type(id=%d, name=%s)", type->id, type->name);
				}
				/* try to import external type. */
				if(obj_import_external_type(L, type)) {
					/* imported type, re-try metatable check. */
					goto recheck_metatable;
				}
				/* External type not yet available, so the object can't be compatible. */
				return 0;
			}
		}
	} else if(!lua_isnoneornil(L, _index)) {
		/* handle cdata. */
		/* get private table. */
		lua_pushlightuserdata(L, obj_udata_private_key);
		lua_rawget(L, LUA_REGISTRYINDEX); /* private table. */
		/* get cdata type check function from private table. */
		lua_pushlightuserdata(L, type);
		lua_rawget(L, -2);

		/* check for function. */
		if(!lua_isnil(L, -1)) {
got_check_func:
			/* pass cdata value to type checking function. */
			lua_pushvalue(L, _index);
			lua_call(L, 1, 1);
			if(!lua_isnil(L, -1)) {
				/* valid type get pointer from cdata. */
				lua_pop(L, 2);
				return (void *)lua_topointer(L, _index);
			}
		} else {
			if(type->flags & OBJ_TYPE_IMPORT) {
				/* try to import external ffi type. */
				if(obj_import_external_ffi_type(L, type)) {
					/* imported type. */
					goto got_check_func;
				}
				/* External type not yet available, so the object can't be compatible. */
			}
		}
	}
	luaL_typerror(L, _index, type->name); /* is not a userdata value. */
	return NULL;
}

static FUNC_UNUSED void * obj_simple_udata_luaoptional(lua_State *L, int _index, obj_type *type) {
	if(lua_isnoneornil(L, _index)) {
		return NULL;
	}
	return obj_simple_udata_luacheck(L, _index, type);
}

static FUNC_UNUSED void * obj_simple_udata_luadelete(lua_State *L, int _index, obj_type *type) {
	void *obj;
	obj = obj_simple_udata_luacheck(L, _index, type);
	/* clear the metatable to invalidate userdata. */
	lua_pushnil(L);
	lua_setmetatable(L, _index);
	return obj;
}

static FUNC_UNUSED void *obj_simple_udata_luapush(lua_State *L, void *obj, int size, obj_type *type)
{
	void *ud;
#if LUAJIT_FFI
	lua_pushlightuserdata(L, type);
	lua_rawget(L, LUA_REGISTRYINDEX); /* type's metatable. */
	if(nobj_ffi_support_enabled_hint && lua_isfunction(L, -1)) {
		/* call special FFI "void *" to FFI object convertion function. */
		lua_pushlightuserdata(L, obj);
		lua_call(L, 1, 1);
		return obj;
	}
#endif
	/* create new userdata. */
	ud = lua_newuserdata(L, size);
	memcpy(ud, obj, size);
	/* get obj_type metatable. */
#if LUAJIT_FFI
	lua_insert(L, -2); /* move userdata below metatable. */
#else
	lua_pushlightuserdata(L, type);
	lua_rawget(L, LUA_REGISTRYINDEX); /* type's metatable. */
#endif
	lua_setmetatable(L, -2);

	return ud;
}

/* default simple object equal method. */
static FUNC_UNUSED int obj_simple_udata_default_equal(lua_State *L) {
	void *ud1 = obj_simple_udata_toobj(L, 1);
	size_t len1 = lua_rawlen(L, 1);
	void *ud2 = obj_simple_udata_toobj(L, 2);
	size_t len2 = lua_rawlen(L, 2);

	if(len1 == len2) {
		lua_pushboolean(L, (memcmp(ud1, ud2, len1) == 0));
	} else {
		lua_pushboolean(L, 0);
	}
	return 1;
}

/* default simple object tostring method. */
static FUNC_UNUSED int obj_simple_udata_default_tostring(lua_State *L) {
	void *ud = obj_simple_udata_toobj(L, 1);

	/* get object's metatable. */
	lua_getmetatable(L, 1);
	lua_remove(L, 1); /* remove userdata. */
	/* get the object's name from the metatable */
	lua_getfield(L, 1, ".name");
	lua_remove(L, 1); /* remove metatable */
	/* push object's pointer */
	lua_pushfstring(L, ": %p", ud);
	lua_concat(L, 2);

	return 1;
}

static int obj_constructor_call_wrapper(lua_State *L) {
	/* replace '__call' table with constructor function. */
	lua_pushvalue(L, lua_upvalueindex(1));
	lua_replace(L, 1);

	/* call constructor function with all parameters after the '__call' table. */
	lua_call(L, lua_gettop(L) - 1, LUA_MULTRET);
	/* return all results from constructor. */
	return lua_gettop(L);
}

static void obj_type_register_constants(lua_State *L, const obj_const *constants, int tab_idx,
		int bidirectional) {
	/* register constants. */
	while(constants->name != NULL) {
		lua_pushstring(L, constants->name);
		switch(constants->type) {
		case CONST_BOOLEAN:
			lua_pushboolean(L, constants->num != 0.0);
			break;
		case CONST_NUMBER:
			lua_pushnumber(L, constants->num);
			break;
		case CONST_STRING:
			lua_pushstring(L, constants->str);
			break;
		default:
			lua_pushnil(L);
			break;
		}
		/* map values back to keys. */
		if(bidirectional) {
			/* check if value already exists. */
			lua_pushvalue(L, -1);
			lua_rawget(L, tab_idx - 3);
			if(lua_isnil(L, -1)) {
				lua_pop(L, 1);
				/* add value->key mapping. */
				lua_pushvalue(L, -1);
				lua_pushvalue(L, -3);
				lua_rawset(L, tab_idx - 4);
			} else {
				/* value already exists. */
				lua_pop(L, 1);
			}
		}
		lua_rawset(L, tab_idx - 2);
		constants++;
	}
}

static void obj_type_register_package(lua_State *L, const reg_sub_module *type_reg) {
	const luaL_Reg *reg_list = type_reg->pub_funcs;

	/* create public functions table. */
	if(reg_list != NULL && reg_list[0].name != NULL) {
		/* register functions */
		luaL_setfuncs(L, reg_list, 0);
	}

	obj_type_register_constants(L, type_reg->constants, -1, type_reg->bidirectional_consts);

	lua_pop(L, 1);  /* drop package table */
}

static void obj_type_register_meta(lua_State *L, const reg_sub_module *type_reg) {
	const luaL_Reg *reg_list;

	/* create public functions table. */
	reg_list = type_reg->pub_funcs;
	if(reg_list != NULL && reg_list[0].name != NULL) {
		/* register functions */
		luaL_setfuncs(L, reg_list, 0);
	}

	obj_type_register_constants(L, type_reg->constants, -1, type_reg->bidirectional_consts);

	/* register methods. */
	luaL_setfuncs(L, type_reg->methods, 0);

	/* create metatable table. */
	lua_newtable(L);
	luaL_setfuncs(L, type_reg->metas, 0); /* fill metatable */
	/* setmetatable on meta-object. */
	lua_setmetatable(L, -2);

	lua_pop(L, 1);  /* drop meta-object */
}

static void obj_type_register(lua_State *L, const reg_sub_module *type_reg, int priv_table) {
	const luaL_Reg *reg_list;
	obj_type *type = type_reg->type;
	const obj_base *base = type_reg->bases;

	if(type_reg->req_type == REG_PACKAGE) {
		obj_type_register_package(L, type_reg);
		return;
	}
	if(type_reg->req_type == REG_META) {
		obj_type_register_meta(L, type_reg);
		return;
	}

	/* create public functions table. */
	reg_list = type_reg->pub_funcs;
	if(reg_list != NULL && reg_list[0].name != NULL) {
		/* register "constructors" as to object's public API */
		luaL_setfuncs(L, reg_list, 0); /* fill public API table. */

		/* make public API table callable as the default constructor. */
		lua_newtable(L); /* create metatable */
		lua_pushliteral(L, "__call");
		lua_pushcfunction(L, reg_list[0].func); /* push first constructor function. */
		lua_pushcclosure(L, obj_constructor_call_wrapper, 1); /* make __call wrapper. */
		lua_rawset(L, -3);         /* metatable.__call = <default constructor> */

#if OBJ_DATA_HIDDEN_METATABLE
		lua_pushliteral(L, "__metatable");
		lua_pushboolean(L, 0);
		lua_rawset(L, -3);         /* metatable.__metatable = false */
#endif

		/* setmetatable on public API table. */
		lua_setmetatable(L, -2);

		lua_pop(L, 1); /* pop public API table, don't need it any more. */
		/* create methods table. */
		lua_newtable(L);
	} else {
		/* register all methods as public functions. */
#if OBJ_DATA_HIDDEN_METATABLE
		lua_pop(L, 1); /* pop public API table, don't need it any more. */
		/* create methods table. */
		lua_newtable(L);
#endif
	}

	luaL_setfuncs(L, type_reg->methods, 0); /* fill methods table. */

	luaL_newmetatable(L, type->name); /* create metatable */
	lua_pushliteral(L, ".name");
	lua_pushstring(L, type->name);
	lua_rawset(L, -3);    /* metatable['.name'] = "<object_name>" */
	lua_pushliteral(L, ".type");
	lua_pushlightuserdata(L, type);
	lua_rawset(L, -3);    /* metatable['.type'] = lightuserdata -> obj_type */
	lua_pushlightuserdata(L, type);
	lua_pushvalue(L, -2); /* dup metatable. */
	lua_rawset(L, LUA_REGISTRYINDEX);    /* REGISTRY[type] = metatable */

	/* add metatable to 'priv_table' */
	lua_pushstring(L, type->name);
	lua_pushvalue(L, -2); /* dup metatable. */
	lua_rawset(L, priv_table);    /* priv_table["<object_name>"] = metatable */

	luaL_setfuncs(L, type_reg->metas, 0); /* fill metatable */

	/* add obj_bases to metatable. */
	while(base->id >= 0) {
		lua_pushlightuserdata(L, (void *)base);
		lua_rawseti(L, -2, base->id);
		base++;
	}

	obj_type_register_constants(L, type_reg->constants, -2, type_reg->bidirectional_consts);

	obj_type_register_implements(L, type_reg->implements);

	lua_pushliteral(L, "__index");
	lua_pushvalue(L, -3);               /* dup methods table */
	lua_rawset(L, -3);                  /* metatable.__index = methods */
#if OBJ_DATA_HIDDEN_METATABLE
	lua_pushliteral(L, "__metatable");
	lua_pushboolean(L, 0);
	lua_rawset(L, -3);                  /* hide metatable:
	                                       metatable.__metatable = false */
#endif
	lua_pop(L, 2);                      /* drop metatable & methods */
}

static FUNC_UNUSED int lua_checktype_ref(lua_State *L, int _index, int _type) {
	luaL_checktype(L,_index,_type);
	lua_pushvalue(L,_index);
	return luaL_ref(L, LUA_REGISTRYINDEX);
}

/* use static pointer as key to weak callback_state table. */
static char obj_callback_state_weak_ref_key[] = "obj_callback_state_weak_ref_key";

static FUNC_UNUSED void *nobj_get_callback_state(lua_State *L, int owner_idx, int size) {
	void *cb_state;

	lua_pushlightuserdata(L, obj_callback_state_weak_ref_key); /* key for weak table. */
	lua_rawget(L, LUA_REGISTRYINDEX);  /* check if weak table exists already. */
	if(lua_isnil(L, -1)) {
		lua_pop(L, 1); /* pop nil. */
		/* create weak table for callback_state */
		lua_newtable(L);               /* weak table. */
		lua_newtable(L);               /* metatable for weak table. */
		lua_pushliteral(L, "__mode");
		lua_pushliteral(L, "k");
		lua_rawset(L, -3);             /* metatable.__mode = 'k'  weak keys. */
		lua_setmetatable(L, -2);       /* add metatable to weak table. */
		lua_pushlightuserdata(L, obj_callback_state_weak_ref_key); /* key for weak table. */
		lua_pushvalue(L, -2);          /* dup weak table. */
		lua_rawset(L, LUA_REGISTRYINDEX);  /* add weak table to registry. */
	}

	/* check weak table for callback_state. */
	lua_pushvalue(L, owner_idx); /* dup. owner as lookup key. */
	lua_rawget(L, -2);
	if(lua_isnil(L, -1)) {
		lua_pop(L, 1); /* pop nil. */
		lua_pushvalue(L, owner_idx); /* dup. owner as lookup key. */
		/* create new callback state. */
		cb_state = lua_newuserdata(L, size);
		lua_rawset(L, -3);
		lua_pop(L, 1); /* pop <weak table> */
	} else {
		/* got existing callback state. */
		cb_state = lua_touserdata(L, -1);
		lua_pop(L, 2); /* pop <weak table>, <callback_state> */
	}

	return cb_state;
}

static FUNC_UNUSED void *nobj_delete_callback_state(lua_State *L, int owner_idx) {
	void *cb_state = NULL;

	lua_pushlightuserdata(L, obj_callback_state_weak_ref_key); /* key for weak table. */
	lua_rawget(L, LUA_REGISTRYINDEX);  /* check if weak table exists already. */
	if(lua_isnil(L, -1)) {
		lua_pop(L, 1); /* pop nil.  no weak table, so there is no callback state. */
		return NULL;
	}
	/* get callback state. */
	lua_pushvalue(L, owner_idx); /* dup. owner */
	lua_rawget(L, -2);
	if(lua_isnil(L, -1)) {
		lua_pop(L, 2); /* pop <weak table>, nil.  No callback state for the owner. */
	} else {
		cb_state = lua_touserdata(L, -1);
		lua_pop(L, 1); /* pop <state> */
		/* remove callback state. */
		lua_pushvalue(L, owner_idx); /* dup. owner */
		lua_pushnil(L);
		lua_rawset(L, -3);
		lua_pop(L, 1); /* pop <weak table> */
	}

	return cb_state;
}





typedef struct Buffer_if {
  const uint8_t * (* const const_data)(void *this_v);
  size_t (* const get_size)(void *this_v);
} BufferIF;

/* a per-module unique pointer for fast lookup of an interface's implementation table. */
static char obj_interface_BufferIF[] = "BufferIF";

#define BufferIF_VAR(var_name) \
	BufferIF *var_name ## _if; \
	void *var_name;

#define BufferIF_LUA_OPTIONAL(L, _index, var_name) \
	var_name = obj_implement_luaoptional(L, _index, (void **)&(var_name ## _if), \
		obj_interface_BufferIF)

#define BufferIF_LUA_CHECK(L, _index, var_name) \
	var_name = obj_implement_luacheck(L, _index, (void **)&(var_name ## _if), \
		obj_interface_BufferIF)






typedef struct MutableBuffer_if {
  uint8_t * (* const data)(void *this_v);
  size_t (* const get_size)(void *this_v);
} MutableBufferIF;

/* a per-module unique pointer for fast lookup of an interface's implementation table. */
static char obj_interface_MutableBufferIF[] = "MutableBufferIF";

#define MutableBufferIF_VAR(var_name) \
	MutableBufferIF *var_name ## _if; \
	void *var_name;

#define MutableBufferIF_LUA_OPTIONAL(L, _index, var_name) \
	var_name = obj_implement_luaoptional(L, _index, (void **)&(var_name ## _if), \
		obj_interface_MutableBufferIF)

#define MutableBufferIF_LUA_CHECK(L, _index, var_name) \
	var_name = obj_implement_luacheck(L, _index, (void **)&(var_name ## _if), \
		obj_interface_MutableBufferIF)




static char *obj_interfaces[] = {
  obj_interface_BufferIF,
  obj_interface_MutableBufferIF,
  NULL,
};

/**
 * LBuffer implements Buffer interface
 */
/** 
 * Buffer interface method const_data
 */
static const uint8_t * LBuffer_Buffer_const_data(void *this_v) {
  LBuffer * this_p = this_v;
  return l_buffer_data(this_p);
}
/** 
 * Buffer interface method get_size
 */
static size_t LBuffer_Buffer_get_size(void *this_v) {
  LBuffer * this_p = this_v;
  return l_buffer_length(this_p);
}

static const BufferIF LBuffer_Buffer = {
  LBuffer_Buffer_const_data,
  LBuffer_Buffer_get_size
};

/**
 * LBuffer implements MutableBuffer interface
 */
/** 
 * MutableBuffer interface method data
 */
static uint8_t * LBuffer_MutableBuffer_data(void *this_v) {
  LBuffer * this_p = this_v;
  return l_buffer_data(this_p);
}
/** 
 * MutableBuffer interface method get_size
 */
static size_t LBuffer_MutableBuffer_get_size(void *this_v) {
  LBuffer * this_p = this_v;
  return l_buffer_size(this_p);
}

static const MutableBufferIF LBuffer_MutableBuffer = {
  LBuffer_MutableBuffer_data,
  LBuffer_MutableBuffer_get_size
};



#define obj_type_LBuffer_check(L, _index) \
	(LBuffer *)obj_simple_udata_luacheck(L, _index, &(obj_type_LBuffer))
#define obj_type_LBuffer_optional(L, _index) \
	(LBuffer *)obj_simple_udata_luaoptional(L, _index, &(obj_type_LBuffer))
#define obj_type_LBuffer_delete(L, _index) \
	(LBuffer *)obj_simple_udata_luadelete(L, _index, &(obj_type_LBuffer))
#define obj_type_LBuffer_push(L, obj) \
	obj_simple_udata_luapush(L, obj, sizeof(LBuffer), &(obj_type_LBuffer))




static const char *buf_ffi_lua_code[] = { "local ffi=require\"ffi\"\n"
"local function ffi_safe_load(name, global)\n"
"	local stat, C = pcall(ffi.load, name, global)\n"
"	if not stat then return nil, C end\n"
"	if global then return ffi.C end\n"
"	return C\n"
"end\n"
"local function ffi_load(name, global)\n"
"	return assert(ffi_safe_load(name, global))\n"
"end\n"
"\n"
"local function ffi_string(ptr)\n"
"	if ptr ~= nil then\n"
"		return ffi.string(ptr)\n"
"	end\n"
"	return nil\n"
"end\n"
"\n"
"local function ffi_string_len(ptr, len)\n"
"	if ptr ~= nil then\n"
"		return ffi.string(ptr, len)\n"
"	end\n"
"	return nil\n"
"end\n"
"\n"
"local f_cast = ffi.cast\n"
"local pcall = pcall\n"
"local error = error\n"
"local type = type\n"
"local tonumber = tonumber\n"
"local tostring = tostring\n"
"local sformat = require\"string\".format\n"
"local rawset = rawset\n"
"local setmetatable = setmetatable\n"
"local package = (require\"package\") or {}\n"
"local p_config = package.config\n"
"local p_cpath = package.cpath\n"
"\n"
"\n"
"local ffi_load_cmodule\n"
"\n"
"-- try to detect luvit.\n"
"if p_config == nil and p_cpath == nil then\n"
"	ffi_load_cmodule = function(name, global)\n"
"		for path,module in pairs(package.loaded) do\n"
"			if module == name then\n"
"				local C, err = ffi_safe_load(path, global)\n"
"				-- return opened library\n"
"				if C then return C end\n"
"			end\n"
"		end\n"
"		error(\"Failed to find: \" .. name)\n"
"	end\n"
"else\n"
"	ffi_load_cmodule = function(name, global)\n"
"		local dir_sep = p_config:sub(1,1)\n"
"		local path_sep = p_config:sub(3,3)\n"
"		local path_mark = p_config:sub(5,5)\n"
"		local path_match = \"([^\" .. path_sep .. \"]*)\" .. path_sep\n"
"		-- convert dotted name to directory path.\n"
"		name = name:gsub('%.', dir_sep)\n"
"		-- try each path in search path.\n"
"		for path in p_cpath:gmatch(path_match) do\n"
"			local fname = path:gsub(path_mark, name)\n"
"			local C, err = ffi_safe_load(fname, global)\n"
"			-- return opened library\n"
"			if C then return C end\n"
"		end\n"
"		error(\"Failed to find: \" .. name)\n"
"	end\n"
"end\n"
"\n"
"local _M, _priv, reg_table = ...\n"
"local REG_OBJECTS_AS_GLOBALS = false\n"
"local C = ffi.C\n"
"\n"
"local OBJ_UDATA_FLAG_OWN		= 1\n"
"\n"
"local function ffi_safe_cdef(block_name, cdefs)\n"
"	local fake_type = \"struct sentinel_\" .. block_name .. \"_ty\"\n"
"	local stat, size = pcall(ffi.sizeof, fake_type)\n"
"	if stat and size > 0 then\n"
"		-- already loaded this cdef block\n"
"		return\n"
"	end\n"
"	cdefs = fake_type .. \"{ int a; int b; int c; };\" .. cdefs\n"
"	return ffi.cdef(cdefs)\n"
"end\n"
"\n"
"ffi_safe_cdef(\"LuaNativeObjects\", [[\n"
"\n"
"typedef struct obj_type obj_type;\n"
"\n"
"typedef void (*base_caster_t)(void **obj);\n"
"\n"
"typedef void (*dyn_caster_t)(void **obj, obj_type **type);\n"
"\n"
"struct obj_type {\n"
"	dyn_caster_t    dcaster;  /**< caster to support casting to sub-objects. */\n"
"	int32_t         id;       /**< type's id. */\n"
"	uint32_t        flags;    /**< type's flags (weak refs) */\n"
"	const char      *name;    /**< type's object name. */\n"
"};\n"
"\n"
"typedef struct obj_base {\n"
"	int32_t        id;\n"
"	base_caster_t  bcaster;\n"
"} obj_base;\n"
"\n"
"typedef struct obj_udata {\n"
"	void     *obj;\n"
"	uint32_t flags;  /**< lua_own:1bit */\n"
"} obj_udata;\n"
"\n"
"int memcmp(const void *s1, const void *s2, size_t n);\n"
"\n"
"]])\n"
"\n"
"local nobj_callback_states = {}\n"
"local nobj_weak_objects = setmetatable({}, {__mode = \"v\"})\n"
"local nobj_obj_flags = {}\n"
"\n"
"local function obj_ptr_to_id(ptr)\n"
"	return tonumber(f_cast('uintptr_t', ptr))\n"
"end\n"
"\n"
"local function obj_to_id(ptr)\n"
"	return tonumber(f_cast('uintptr_t', f_cast('void *', ptr)))\n"
"end\n"
"\n"
"local function register_default_constructor(_pub, obj_name, constructor)\n"
"	local obj_pub = _pub[obj_name]\n"
"	if type(obj_pub) == 'table' then\n"
"		-- copy table since it might have a locked metatable\n"
"		local new_pub = {}\n"
"		for k,v in pairs(obj_pub) do\n"
"			new_pub[k] = v\n"
"		end\n"
"		setmetatable(new_pub, { __call = function(t,...)\n"
"			return constructor(...)\n"
"		end,\n"
"		__metatable = false,\n"
"		})\n"
"		obj_pub = new_pub\n"
"	else\n"
"		obj_pub = constructor\n"
"	end\n"
"	_pub[obj_name] = obj_pub\n"
"	_M[obj_name] = obj_pub\n"
"	if REG_OBJECTS_AS_GLOBALS then\n"
"		_G[obj_name] = obj_pub\n"
"	end\n"
"end\n"
"\n"
"ffi_safe_cdef(\"BufferIF\", [[\n"
"typedef struct Buffer_if {\n"
"  const uint8_t * (* const const_data)(void *this_v);\n"
"  size_t (* const get_size)(void *this_v);\n"
"} BufferIF;\n"
"]])\n"
"\n"
"ffi_safe_cdef(\"FDIF\", [[\n"
"typedef struct FD_if {\n"
"  int (* const get_fd)(void *this_v);\n"
"  int (* const get_type)(void *this_v);\n"
"} FDIF;\n"
"]])\n"
"\n"
"ffi_safe_cdef(\"MutableBufferIF\", [[\n"
"typedef struct MutableBuffer_if {\n"
"  uint8_t * (* const data)(void *this_v);\n"
"  size_t (* const get_size)(void *this_v);\n"
"} MutableBufferIF;\n"
"]])\n"
"\n"
"local Cmod = ffi_load_cmodule(\"buf\", false)\n"
"local C = Cmod\n"
"\n"
"ffi.cdef[[\n"
"typedef struct LBuffer LBuffer;\n"
"\n"
"]]\n"
"\n"
"ffi.cdef[[\n"
"typedef struct LBuffer LBuffer;\n"
"\n"
"typedef uint32_t buflen_t;\n"
"\n"
"struct LBuffer {\n"
"	uint8_t   *_data;           /**< Buffer data. */\n"
"	buflen_t  _tail;            /**< start offset, tail must always be lower then head. */\n"
"	buflen_t  _head;            /**< end offset, head must always be lower then size. */\n"
"	buflen_t  _size;            /**< total memory size of data. */\n"
"	uint32_t  _free_struct: 1;  /**< Should the LBuffer struct be freed. */\n"
"};\n"
"\n"
"\n"
"void l_buffer_free(LBuffer *);\n"
"\n"
"char * l_buffer_data(LBuffer *);\n"
"\n"
"size_t l_buffer_length(LBuffer *);\n"
"\n"
"void l_buffer_reset(LBuffer *);\n"
"\n"
"bool l_buffer_set_length(LBuffer *, size_t);\n"
"\n"
"void * l_buffer_data1(LBuffer *) asm(\"l_buffer_data\");\n"
"const char * l_buffer_sub(LBuffer *, size_t, size_t*);\n"
"\n"
"size_t l_buffer_size(LBuffer *);\n"
"\n"
"bool l_buffer_resize(LBuffer *, size_t);\n"
"\n"
"const char * l_buffer_read_data_len(LBuffer *, size_t);\n"
"\n"
"const char * l_buffer_read_string_len(LBuffer *, size_t*);\n"
"\n"
"int l_buffer_read_uint8_t(LBuffer *, uint8_t*);\n"
"\n"
"int l_buffer_read_uint8_t1(LBuffer *, int8_t*) asm(\"l_buffer_read_uint8_t\");\n"
"int l_buffer_read_uint16_t(LBuffer *, uint16_t*);\n"
"\n"
"int l_buffer_read_uint16_t1(LBuffer *, int16_t*) asm(\"l_buffer_read_uint16_t\");\n"
"int l_buffer_read_uint32_t(LBuffer *, uint32_t*);\n"
"\n"
"int l_buffer_read_uint32_t1(LBuffer *, int32_t*) asm(\"l_buffer_read_uint32_t\");\n"
"int l_buffer_read_uint64_t(LBuffer *, uint64_t*);\n"
"\n"
"int l_buffer_read_uint64_t1(LBuffer *, int64_t*) asm(\"l_buffer_read_uint64_t\");\n"
"int l_buffer_read_float(LBuffer *, float*);\n"
"\n"
"int l_buffer_read_double(LBuffer *, double*);\n"
"\n"
"int l_buffer_read_b128_uvar32(LBuffer *, uint32_t*);\n"
"\n"
"int l_buffer_read_b128_var32(LBuffer *, int32_t*, bool);\n"
"\n"
"int l_buffer_read_b128_uvar64(LBuffer *, uint64_t*);\n"
"\n"
"int l_buffer_read_b128_var64(LBuffer *, int64_t*, bool);\n"
"\n"
"int l_buffer_append_data_len(LBuffer *, const char *, size_t);\n"
"\n"
"int l_buffer_append_string_len(LBuffer *, const char *, size_t);\n"
"\n"
"int l_buffer_append_uint8_t(LBuffer *, uint8_t);\n"
"\n"
"int l_buffer_append_uint8_t1(LBuffer *, int8_t) asm(\"l_buffer_append_uint8_t\");\n"
"int l_buffer_append_uint16_t(LBuffer *, uint16_t);\n"
"\n"
"int l_buffer_append_uint16_t1(LBuffer *, int16_t) asm(\"l_buffer_append_uint16_t\");\n"
"int l_buffer_append_uint32_t(LBuffer *, uint32_t);\n"
"\n"
"int l_buffer_append_uint32_t1(LBuffer *, int32_t) asm(\"l_buffer_append_uint32_t\");\n"
"int l_buffer_append_uint64_t(LBuffer *, uint64_t);\n"
"\n"
"int l_buffer_append_uint64_t1(LBuffer *, int64_t) asm(\"l_buffer_append_uint64_t\");\n"
"int l_buffer_append_float(LBuffer *, float);\n"
"\n"
"int l_buffer_append_double(LBuffer *, double);\n"
"\n"
"int l_buffer_append_b128_uvar32(LBuffer *, uint32_t);\n"
"\n"
"int l_buffer_append_b128_var32(LBuffer *, int32_t, bool);\n"
"\n"
"int l_buffer_append_b128_uvar64(LBuffer *, uint64_t);\n"
"\n"
"int l_buffer_append_b128_var64(LBuffer *, int64_t, bool);\n"
"\n"
"void l_buffer_init(LBuffer *, const uint8_t *, size_t);\n"
"\n"
"\n"
"]]\n"
"\n"
"REG_OBJECTS_AS_GLOBALS = false\n"
"local _obj_interfaces_ffi = {}\n"
"local _pub = {}\n"
"local _meth = {}\n"
"local _push = {}\n"
"local _obj_subs = {}\n"
"for obj_name,mt in pairs(_priv) do\n"
"	if type(mt) == 'table' then\n"
"		_obj_subs[obj_name] = {}\n"
"		if mt.__index then\n"
"			_meth[obj_name] = mt.__index\n"
"		end\n"
"	end\n"
"end\n"
"for obj_name,pub in pairs(_M) do\n"
"	_pub[obj_name] = pub\n"
"end\n"
"\n"
"--\n"
"-- CData Metatable access\n"
"--\n"
"local _ctypes = {}\n"
"local _type_names = {}\n"
"local _get_mt_key = {}\n"
"local _ctype_meta_map = {}\n"
"\n"
"local f_typeof = ffi.typeof\n"
"local function get_cdata_type_id(cdata)\n"
"	return tonumber(f_typeof(cdata))\n"
"end\n"
"local function get_cdata_mt(cdata)\n"
"	return _ctype_meta_map[tonumber(f_typeof(cdata))]\n"
"end\n"
"\n"
"local function obj_register_ctype(name, ctype)\n"
"	local obj_mt = _priv[name]\n"
"	local obj_type = obj_mt['.type']\n"
"	local obj_ctype = ffi.typeof(ctype)\n"
"	local obj_type_id = tonumber(obj_ctype)\n"
"	_ctypes[name] = obj_ctype\n"
"	_type_names[name] = tostring(obj_ctype)\n"
"	_ctype_meta_map[obj_type_id] = obj_mt\n"
"	_ctype_meta_map[obj_mt] = obj_type_id\n"
"	return obj_mt, obj_type, obj_ctype\n"
"end\n"
"\n"
"--\n"
"-- Interfaces helper code.\n"
"--\n"
"local _obj_interfaces_key = \"obj_interfaces<1.0>_table_key\"\n"
"local _obj_interfaces_ud = reg_table[_obj_interfaces_key]\n"
"local _obj_interfaces_key_ffi = _obj_interfaces_key .. \"_LJ2_FFI\"\n"
"_obj_interfaces_ffi = reg_table[_obj_interfaces_key_ffi]\n"
"if not _obj_interfaces_ffi then\n"
"	-- create missing interfaces table for FFI bindings.\n"
"	_obj_interfaces_ffi = {}\n"
"	reg_table[_obj_interfaces_key_ffi] = _obj_interfaces_ffi\n"
"end\n"
"\n"
"local function obj_get_userdata_interface(if_name, expected_err)\n"
"	local impls_ud = _obj_interfaces_ud[if_name]\n"
"	if not impls_ud then\n"
"		impls_ud = {}\n"
"		_obj_interfaces_ud[if_name] = impls_ud\n"
"	end\n"
"	-- create cdata check function to be used by non-ffi bindings.\n"
"	if not impls_ud.cdata then\n"
"		function impls_ud.cdata(obj)\n"
"			return assert(impls_ud[get_cdata_mt(obj)], expected_err)\n"
"		end\n"
"	end\n"
"	return impls_ud\n"
"end\n"
"\n"
"local function obj_get_interface_check(if_name, expected_err)\n"
"	local impls_ffi = _obj_interfaces_ffi[if_name]\n"
"	if not impls_ffi then\n"
"		local if_type = ffi.typeof(if_name .. \" *\")\n"
"		local impls_ud = obj_get_userdata_interface(if_name, expected_err)\n"
"		-- create table for FFI-based interface implementations.\n"
"		impls_ffi = setmetatable({}, {\n"
"		__index = function(impls_ffi, mt)\n"
"			local impl = impls_ud[mt]\n"
"			if impl then\n"
"				-- cast to cdata\n"
"				impl = if_type(impl)\n"
"				rawset(impls_ffi, mt, impl)\n"
"			end\n"
"			return impl\n"
"		end})\n"
"		_obj_interfaces_ffi[if_name] = impls_ffi\n"
"\n"
"		-- create check function for this interface.\n"
"		function impls_ffi.check(obj)\n"
"			local impl\n"
"			if type(obj) == 'cdata' then\n"
"				impl = impls_ffi[get_cdata_type_id(obj)]\n"
"			else\n"
"				impl = impls_ud.userdata(impls_ffi, obj)\n"
"			end\n"
"			return assert(impl, expected_err)\n"
"		end\n"
"	end\n"
"	return impls_ffi.check\n"
"end\n"
"\n"
"local function obj_register_interface(if_name, obj_name)\n"
"	-- loopkup cdata id\n"
"	local obj_mt = _priv[obj_name]\n"
"	local obj_type_id = _ctype_meta_map[obj_mt]\n"
"	local impl_meths = {}\n"
"	local ffi_impls = _obj_interfaces_ffi[if_name]\n"
"	ffi_impls[obj_type_id] = impl_meths\n"
"	_meth[obj_name]['NOBJ_get_' .. if_name] = impl_meths\n"
"	return impl_meths\n"
"end\n"
"\n"
"\n"
"local obj_type_LBuffer_check\n"
"local obj_type_LBuffer_delete\n"
"local obj_type_LBuffer_push\n"
"\n"
"do\n"
"	local obj_mt, obj_type, obj_ctype = obj_register_ctype(\"LBuffer\", \"LBuffer\")\n"
"	local LBuffer_sizeof = ffi.sizeof\"LBuffer\"\n"
"\n"
"	function obj_type_LBuffer_check(obj)\n"
"		return obj\n"
"	end\n"
"\n"
"	function obj_type_LBuffer_delete(obj)\n"
"		return obj\n"
"	end\n"
"\n"
"	function obj_type_LBuffer_push(obj)\n"
"		return obj\n"
"	end\n"
"\n"
"	function obj_mt:__tostring()\n"
"		return sformat(\"LBuffer: %p\", self)\n"
"	end\n"
"\n"
"	function obj_mt.__eq(val1, val2)\n"
"		if not ffi.istype(obj_ctype, val2) then return false end\n"
"		assert(ffi.istype(obj_ctype, val1), \"expected LBuffer\")\n"
"		return (C.memcmp(val1, val2, LBuffer_sizeof) == 0)\n"
"	end\n"
"\n"
"	-- type checking function for C API.\n"
"	local function c_check(obj)\n"
"		if ffi.istype(obj_ctype, obj) then return obj end\n"
"		return nil\n"
"	end\n"
"	_priv[obj_type] = c_check\n"
"	-- push function for C API.\n"
"	reg_table[obj_type] = function(ptr)\n"
"		local obj = obj_ctype()\n"
"		ffi.copy(obj, ptr, LBuffer_sizeof);\n"
"		return obj\n"
"	end\n"
"\n"
"	-- export check functions for use in other modules.\n"
"	obj_mt.c_check = c_check\n"
"	obj_mt.ffi_check = obj_type_LBuffer_check\n"
"end\n"
"\n"
"\n"
"local obj_type_Buffer_check =\n"
"	obj_get_interface_check(\"BufferIF\", \"Expected object with Buffer interface\")\n"
"\n"
"local obj_type_FD_check =\n"
"	obj_get_interface_check(\"FDIF\", \"Expected object with FD interface\")\n"
"\n"
"local obj_type_MutableBuffer_check =\n"
"	obj_get_interface_check(\"MutableBufferIF\", \"Expected object with MutableBuffer interface\")\n"
"\n"
"\n"
"-- Start \"LBuffer\" FFI interface\n"
"-- method: free\n"
"function _meth.LBuffer.free(self)\n"
"  local self = obj_type_LBuffer_delete(self)\n"
"  if not self then return end\n"
"  C.l_buffer_free(self)\n"
"  return \n"
"end\n"
"_priv.LBuffer.__gc = _meth.LBuffer.free\n"
"\n"
"-- method: __tostring\n"
"function _priv.LBuffer.__tostring(self)\n"
"  \n"
"  local data_len = 0\n"
"  local data\n"
"  data = C.l_buffer_data(self)\n"
"  data_len = C.l_buffer_length(self)\n"
"  return ffi_string_len(data,data_len)\n"
"end\n"
"\n"
"-- method: tostring\n"
"function _meth.LBuffer.tostring(self)\n"
"  \n"
"  local data_len = 0\n"
"  local data\n"
"  data = C.l_buffer_data(self)\n"
"  data_len = C.l_buffer_length(self)\n"
"  return ffi_string_len(data,data_len)\n"
"end\n"
"\n"
"-- method: reset\n"
"function _meth.LBuffer.reset(self)\n"
"  \n"
"  C.l_buffer_reset(self)\n"
"  return \n"
"end\n"
"\n"
"-- method: length\n"
"function _meth.LBuffer.length(self)\n"
"  \n"
"  local size = 0\n"
"  size = C.l_buffer_length(self)\n"
"  return tonumber(size)\n"
"end\n"
"\n"
"-- method: __len\n"
"function _priv.LBuffer.__len(self)\n"
"  \n"
"  local size = 0\n"
"  size = C.l_buffer_length(self)\n"
"  return tonumber(size)\n"
"end\n"
"\n"
"-- method: set_length\n"
"function _meth.LBuffer.set_length(self, len)\n"
"  \n"
"  \n"
"  local rc_l_buffer_set_length = 0\n"
"  rc_l_buffer_set_length = C.l_buffer_set_length(self, len)\n"
"  return rc_l_buffer_set_length\n"
"end\n"
"\n"
"-- method: data_ptr\n"
"function _meth.LBuffer.data_ptr(self)\n"
"  \n"
"  local rc_l_buffer_data\n"
"  rc_l_buffer_data = C.l_buffer_data1(self)\n"
"  return rc_l_buffer_data\n"
"end\n"
"\n"
"do\n"
"  local sub_len_tmp = ffi.new(\"size_t[1]\")\n"
"\n"
"-- method: sub\n"
"function _meth.LBuffer.sub(self, off, len)\n"
"  \n"
"  off = off or 0\n"
"  len = len or 0\n"
"  local data\n"
"  sub_len_tmp[0] = len\n"
"  data = C.l_buffer_sub(self, off, sub_len_tmp)\n"
"  len = sub_len_tmp[0]\n"
"  return ffi_string_len(data,len)\n"
"end\n"
"end\n"
"\n"
"-- method: size\n"
"function _meth.LBuffer.size(self)\n"
"  \n"
"  local size = 0\n"
"  size = C.l_buffer_size(self)\n"
"  return tonumber(size)\n"
"end\n"
"\n"
"-- method: resize\n"
"function _meth.LBuffer.resize(self, len)\n"
"  \n"
"  \n"
"  local rc_l_buffer_resize = 0\n"
"  rc_l_buffer_resize = C.l_buffer_resize(self, len)\n"
"  return rc_l_buffer_resize\n"
"end\n"
"\n"
"-- method: read_data\n"
"function _meth.LBuffer.read_data(self, len)\n"
"  \n"
"  \n"
"  local data\n"
"  data = C.l_buffer_read_data_len(self, len)\n"
"  return ffi_string_len(data,len)\n"
"end\n"
"\n"
"do\n"
"  local read_string_str_tmp = ffi.new(\"size_t[1]\")\n"
"\n"
"-- method: read_string\n"
"function _meth.LBuffer.read_string(self)\n"
"  \n", /* ----- CUT ----- */
"  local str_len = 0\n"
"  local str\n"
"  str = C.l_buffer_read_string_len(self, read_string_str_tmp)\n"
"  str_len = read_string_str_tmp[0]\n"
"  return ffi_string_len(str,str_len)\n"
"end\n"
"end\n"
"\n"
"do\n"
"  local read_uint8_num_tmp = ffi.new(\"uint8_t[1]\")\n"
"\n"
"-- method: read_uint8\n"
"function _meth.LBuffer.read_uint8(self)\n"
"  \n"
"  local num\n"
"  local rc_l_buffer_read_uint8_t = 0\n"
"  rc_l_buffer_read_uint8_t = C.l_buffer_read_uint8_t(self, read_uint8_num_tmp)\n"
"  num = read_uint8_num_tmp[0]\n"
"  return num, rc_l_buffer_read_uint8_t\n"
"end\n"
"end\n"
"\n"
"do\n"
"  local read_int8_num_tmp = ffi.new(\"int8_t[1]\")\n"
"\n"
"-- method: read_int8\n"
"function _meth.LBuffer.read_int8(self)\n"
"  \n"
"  local num\n"
"  local rc_l_buffer_read_uint8_t = 0\n"
"  rc_l_buffer_read_uint8_t = C.l_buffer_read_uint8_t1(self, read_int8_num_tmp)\n"
"  num = read_int8_num_tmp[0]\n"
"  return num, rc_l_buffer_read_uint8_t\n"
"end\n"
"end\n"
"\n"
"do\n"
"  local read_uint16_num_tmp = ffi.new(\"uint16_t[1]\")\n"
"\n"
"-- method: read_uint16\n"
"function _meth.LBuffer.read_uint16(self)\n"
"  \n"
"  local num\n"
"  local rc_l_buffer_read_uint16_t = 0\n"
"  rc_l_buffer_read_uint16_t = C.l_buffer_read_uint16_t(self, read_uint16_num_tmp)\n"
"  num = read_uint16_num_tmp[0]\n"
"  return num, rc_l_buffer_read_uint16_t\n"
"end\n"
"end\n"
"\n"
"do\n"
"  local read_int16_num_tmp = ffi.new(\"int16_t[1]\")\n"
"\n"
"-- method: read_int16\n"
"function _meth.LBuffer.read_int16(self)\n"
"  \n"
"  local num\n"
"  local rc_l_buffer_read_uint16_t = 0\n"
"  rc_l_buffer_read_uint16_t = C.l_buffer_read_uint16_t1(self, read_int16_num_tmp)\n"
"  num = read_int16_num_tmp[0]\n"
"  return num, rc_l_buffer_read_uint16_t\n"
"end\n"
"end\n"
"\n"
"do\n"
"  local read_uint32_num_tmp = ffi.new(\"uint32_t[1]\")\n"
"\n"
"-- method: read_uint32\n"
"function _meth.LBuffer.read_uint32(self)\n"
"  \n"
"  local num\n"
"  local rc_l_buffer_read_uint32_t = 0\n"
"  rc_l_buffer_read_uint32_t = C.l_buffer_read_uint32_t(self, read_uint32_num_tmp)\n"
"  num = read_uint32_num_tmp[0]\n"
"  return num, rc_l_buffer_read_uint32_t\n"
"end\n"
"end\n"
"\n"
"do\n"
"  local read_int32_num_tmp = ffi.new(\"int32_t[1]\")\n"
"\n"
"-- method: read_int32\n"
"function _meth.LBuffer.read_int32(self)\n"
"  \n"
"  local num\n"
"  local rc_l_buffer_read_uint32_t = 0\n"
"  rc_l_buffer_read_uint32_t = C.l_buffer_read_uint32_t1(self, read_int32_num_tmp)\n"
"  num = read_int32_num_tmp[0]\n"
"  return num, rc_l_buffer_read_uint32_t\n"
"end\n"
"end\n"
"\n"
"do\n"
"  local read_uint64_num_tmp = ffi.new(\"uint64_t[1]\")\n"
"\n"
"-- method: read_uint64\n"
"function _meth.LBuffer.read_uint64(self)\n"
"  \n"
"  local num\n"
"  local rc_l_buffer_read_uint64_t = 0\n"
"  rc_l_buffer_read_uint64_t = C.l_buffer_read_uint64_t(self, read_uint64_num_tmp)\n"
"  num = read_uint64_num_tmp[0]\n"
"  return num, rc_l_buffer_read_uint64_t\n"
"end\n"
"end\n"
"\n"
"do\n"
"  local read_int64_num_tmp = ffi.new(\"int64_t[1]\")\n"
"\n"
"-- method: read_int64\n"
"function _meth.LBuffer.read_int64(self)\n"
"  \n"
"  local num\n"
"  local rc_l_buffer_read_uint64_t = 0\n"
"  rc_l_buffer_read_uint64_t = C.l_buffer_read_uint64_t1(self, read_int64_num_tmp)\n"
"  num = read_int64_num_tmp[0]\n"
"  return num, rc_l_buffer_read_uint64_t\n"
"end\n"
"end\n"
"\n"
"do\n"
"  local read_float_num_tmp = ffi.new(\"float[1]\")\n"
"\n"
"-- method: read_float\n"
"function _meth.LBuffer.read_float(self)\n"
"  \n"
"  local num\n"
"  local rc_l_buffer_read_float = 0\n"
"  rc_l_buffer_read_float = C.l_buffer_read_float(self, read_float_num_tmp)\n"
"  num = read_float_num_tmp[0]\n"
"  return num, rc_l_buffer_read_float\n"
"end\n"
"end\n"
"\n"
"do\n"
"  local read_double_num_tmp = ffi.new(\"double[1]\")\n"
"\n"
"-- method: read_double\n"
"function _meth.LBuffer.read_double(self)\n"
"  \n"
"  local num\n"
"  local rc_l_buffer_read_double = 0\n"
"  rc_l_buffer_read_double = C.l_buffer_read_double(self, read_double_num_tmp)\n"
"  num = read_double_num_tmp[0]\n"
"  return num, rc_l_buffer_read_double\n"
"end\n"
"end\n"
"\n"
"do\n"
"  local read_b128_uvar32_num_tmp = ffi.new(\"uint32_t[1]\")\n"
"\n"
"-- method: read_b128_uvar32\n"
"function _meth.LBuffer.read_b128_uvar32(self)\n"
"  \n"
"  local num\n"
"  local rc_l_buffer_read_b128_uvar32 = 0\n"
"  rc_l_buffer_read_b128_uvar32 = C.l_buffer_read_b128_uvar32(self, read_b128_uvar32_num_tmp)\n"
"  num = read_b128_uvar32_num_tmp[0]\n"
"  return num, rc_l_buffer_read_b128_uvar32\n"
"end\n"
"end\n"
"\n"
"do\n"
"  local read_b128_var32_num_tmp = ffi.new(\"int32_t[1]\")\n"
"\n"
"-- method: read_b128_var32\n"
"function _meth.LBuffer.read_b128_var32(self, zigzag)\n"
"  \n"
"  zigzag = zigzag or 0\n"
"  local num\n"
"  local rc_l_buffer_read_b128_var32 = 0\n"
"  rc_l_buffer_read_b128_var32 = C.l_buffer_read_b128_var32(self, read_b128_var32_num_tmp, zigzag)\n"
"  num = read_b128_var32_num_tmp[0]\n"
"  return num, rc_l_buffer_read_b128_var32\n"
"end\n"
"end\n"
"\n"
"do\n"
"  local read_b128_uvar64_num_tmp = ffi.new(\"uint64_t[1]\")\n"
"\n"
"-- method: read_b128_uvar64\n"
"function _meth.LBuffer.read_b128_uvar64(self)\n"
"  \n"
"  local num\n"
"  local rc_l_buffer_read_b128_uvar64 = 0\n"
"  rc_l_buffer_read_b128_uvar64 = C.l_buffer_read_b128_uvar64(self, read_b128_uvar64_num_tmp)\n"
"  num = read_b128_uvar64_num_tmp[0]\n"
"  return num, rc_l_buffer_read_b128_uvar64\n"
"end\n"
"end\n"
"\n"
"do\n"
"  local read_b128_var64_num_tmp = ffi.new(\"int64_t[1]\")\n"
"\n"
"-- method: read_b128_var64\n"
"function _meth.LBuffer.read_b128_var64(self, zigzag)\n"
"  \n"
"  zigzag = zigzag or 0\n"
"  local num\n"
"  local rc_l_buffer_read_b128_var64 = 0\n"
"  rc_l_buffer_read_b128_var64 = C.l_buffer_read_b128_var64(self, read_b128_var64_num_tmp, zigzag)\n"
"  num = read_b128_var64_num_tmp[0]\n"
"  return num, rc_l_buffer_read_b128_var64\n"
"end\n"
"end\n"
"\n"
"-- method: append_data\n"
"function _meth.LBuffer.append_data(self, data)\n"
"  \n"
"  local data_len = #data\n"
"  local rc_l_buffer_append_data_len = 0\n"
"  rc_l_buffer_append_data_len = C.l_buffer_append_data_len(self, data, data_len)\n"
"  return rc_l_buffer_append_data_len\n"
"end\n"
"\n"
"-- method: append_string\n"
"function _meth.LBuffer.append_string(self, str)\n"
"  \n"
"  local str_len = #str\n"
"  local rc_l_buffer_append_string_len = 0\n"
"  rc_l_buffer_append_string_len = C.l_buffer_append_string_len(self, str, str_len)\n"
"  return rc_l_buffer_append_string_len\n"
"end\n"
"\n"
"-- method: append_uint8\n"
"function _meth.LBuffer.append_uint8(self, num)\n"
"  \n"
"  \n"
"  local rc_l_buffer_append_uint8_t = 0\n"
"  rc_l_buffer_append_uint8_t = C.l_buffer_append_uint8_t(self, num)\n"
"  return rc_l_buffer_append_uint8_t\n"
"end\n"
"\n"
"-- method: append_int8\n"
"function _meth.LBuffer.append_int8(self, num)\n"
"  \n"
"  \n"
"  local rc_l_buffer_append_uint8_t = 0\n"
"  rc_l_buffer_append_uint8_t = C.l_buffer_append_uint8_t1(self, num)\n"
"  return rc_l_buffer_append_uint8_t\n"
"end\n"
"\n"
"-- method: append_uint16\n"
"function _meth.LBuffer.append_uint16(self, num)\n"
"  \n"
"  \n"
"  local rc_l_buffer_append_uint16_t = 0\n"
"  rc_l_buffer_append_uint16_t = C.l_buffer_append_uint16_t(self, num)\n"
"  return rc_l_buffer_append_uint16_t\n"
"end\n"
"\n"
"-- method: append_int16\n"
"function _meth.LBuffer.append_int16(self, num)\n"
"  \n"
"  \n"
"  local rc_l_buffer_append_uint16_t = 0\n"
"  rc_l_buffer_append_uint16_t = C.l_buffer_append_uint16_t1(self, num)\n"
"  return rc_l_buffer_append_uint16_t\n"
"end\n"
"\n"
"-- method: append_uint32\n"
"function _meth.LBuffer.append_uint32(self, num)\n"
"  \n"
"  \n"
"  local rc_l_buffer_append_uint32_t = 0\n"
"  rc_l_buffer_append_uint32_t = C.l_buffer_append_uint32_t(self, num)\n"
"  return rc_l_buffer_append_uint32_t\n"
"end\n"
"\n"
"-- method: append_int32\n"
"function _meth.LBuffer.append_int32(self, num)\n"
"  \n"
"  \n"
"  local rc_l_buffer_append_uint32_t = 0\n"
"  rc_l_buffer_append_uint32_t = C.l_buffer_append_uint32_t1(self, num)\n"
"  return rc_l_buffer_append_uint32_t\n"
"end\n"
"\n"
"-- method: append_uint64\n"
"function _meth.LBuffer.append_uint64(self, num)\n"
"  \n"
"  \n"
"  local rc_l_buffer_append_uint64_t = 0\n"
"  rc_l_buffer_append_uint64_t = C.l_buffer_append_uint64_t(self, num)\n"
"  return rc_l_buffer_append_uint64_t\n"
"end\n"
"\n"
"-- method: append_int64\n"
"function _meth.LBuffer.append_int64(self, num)\n"
"  \n"
"  \n"
"  local rc_l_buffer_append_uint64_t = 0\n"
"  rc_l_buffer_append_uint64_t = C.l_buffer_append_uint64_t1(self, num)\n"
"  return rc_l_buffer_append_uint64_t\n"
"end\n"
"\n"
"-- method: append_float\n"
"function _meth.LBuffer.append_float(self, num)\n"
"  \n"
"  \n"
"  local rc_l_buffer_append_float = 0\n"
"  rc_l_buffer_append_float = C.l_buffer_append_float(self, num)\n"
"  return rc_l_buffer_append_float\n"
"end\n"
"\n"
"-- method: append_double\n"
"function _meth.LBuffer.append_double(self, num)\n"
"  \n"
"  \n"
"  local rc_l_buffer_append_double = 0\n"
"  rc_l_buffer_append_double = C.l_buffer_append_double(self, num)\n"
"  return rc_l_buffer_append_double\n"
"end\n"
"\n"
"-- method: append_b128_uvar32\n"
"function _meth.LBuffer.append_b128_uvar32(self, num)\n"
"  \n"
"  \n"
"  local rc_l_buffer_append_b128_uvar32 = 0\n"
"  rc_l_buffer_append_b128_uvar32 = C.l_buffer_append_b128_uvar32(self, num)\n"
"  return rc_l_buffer_append_b128_uvar32\n"
"end\n"
"\n"
"-- method: append_b128_var32\n"
"function _meth.LBuffer.append_b128_var32(self, num, zigzag)\n"
"  \n"
"  \n"
"  zigzag = zigzag or 0\n"
"  local rc_l_buffer_append_b128_var32 = 0\n"
"  rc_l_buffer_append_b128_var32 = C.l_buffer_append_b128_var32(self, num, zigzag)\n"
"  return rc_l_buffer_append_b128_var32\n"
"end\n"
"\n"
"-- method: append_b128_uvar64\n"
"function _meth.LBuffer.append_b128_uvar64(self, num)\n"
"  \n"
"  \n"
"  local rc_l_buffer_append_b128_uvar64 = 0\n"
"  rc_l_buffer_append_b128_uvar64 = C.l_buffer_append_b128_uvar64(self, num)\n"
"  return rc_l_buffer_append_b128_uvar64\n"
"end\n"
"\n"
"-- method: append_b128_var64\n"
"function _meth.LBuffer.append_b128_var64(self, num, zigzag)\n"
"  \n"
"  \n"
"  zigzag = zigzag or 0\n"
"  local rc_l_buffer_append_b128_var64 = 0\n"
"  rc_l_buffer_append_b128_var64 = C.l_buffer_append_b128_var64(self, num, zigzag)\n"
"  return rc_l_buffer_append_b128_var64\n"
"end\n"
"\n"
"-- LBuffer implements Buffer interface\n"
"do\n"
"  local impl_meths = obj_register_interface(\"BufferIF\", \"LBuffer\")\n"
"-- Buffer interface method const_data\n"
"impl_meths.const_data = C.l_buffer_data\n"
"-- Buffer interface method get_size\n"
"impl_meths.get_size = C.l_buffer_length\n"
"end\n"
"\n"
"-- LBuffer implements MutableBuffer interface\n"
"do\n"
"  local impl_meths = obj_register_interface(\"MutableBufferIF\", \"LBuffer\")\n"
"-- MutableBuffer interface method data\n"
"impl_meths.data = C.l_buffer_data\n"
"-- MutableBuffer interface method get_size\n"
"impl_meths.get_size = C.l_buffer_size\n"
"end\n"
"\n"
"_push.LBuffer = obj_type_LBuffer_push\n"
"ffi.metatype(\"LBuffer\", _priv.LBuffer)\n"
"-- End \"LBuffer\" FFI interface\n"
"\n"
"-- method: new\n"
"function _M.new(size_or_data)\n"
"  local buf = ffi.new(\"LBuffer\")\n"
"  local data\n"
"  local len = 0\n"
"	local ltype = type(size_or_data)\n"
"\n"
"	if ltype == 'string' then\n"
"		data = size_or_data\n"
"		len = #data\n"
"	elseif ltype == 'number' then\n"
"		len = size_or_data\n"
"	end\n"
"\n"
"  C.l_buffer_init(buf, data, len)\n"
"  return obj_type_LBuffer_push(buf)\n"
"end\n"
"\n", NULL };


/* method: free */
static int LBuffer__free__meth(lua_State *L) {
  LBuffer * this;
  this = obj_type_LBuffer_delete(L,1);
  l_buffer_free(this);
  return 0;
}

/* method: __tostring */
static int LBuffer____tostring__meth(lua_State *L) {
  LBuffer * this;
  size_t data_len = 0;
  char * data = NULL;
  this = obj_type_LBuffer_check(L,1);
  data = l_buffer_data(this);
  data_len = l_buffer_length(this);
  if(data == NULL) lua_pushnil(L);  else lua_pushlstring(L, data,data_len);
  return 1;
}

/* method: tostring */
static int LBuffer__tostring__meth(lua_State *L) {
  LBuffer * this;
  size_t data_len = 0;
  char * data = NULL;
  this = obj_type_LBuffer_check(L,1);
  data = l_buffer_data(this);
  data_len = l_buffer_length(this);
  if(data == NULL) lua_pushnil(L);  else lua_pushlstring(L, data,data_len);
  return 1;
}

/* method: reset */
static int LBuffer__reset__meth(lua_State *L) {
  LBuffer * this;
  this = obj_type_LBuffer_check(L,1);
  l_buffer_reset(this);
  return 0;
}

/* method: length */
static int LBuffer__length__meth(lua_State *L) {
  LBuffer * this;
  size_t size = 0;
  this = obj_type_LBuffer_check(L,1);
  size = l_buffer_length(this);
  lua_pushinteger(L, size);
  return 1;
}

/* method: __len */
static int LBuffer____len__meth(lua_State *L) {
  LBuffer * this;
  size_t size = 0;
  this = obj_type_LBuffer_check(L,1);
  size = l_buffer_length(this);
  lua_pushinteger(L, size);
  return 1;
}

/* method: set_length */
static int LBuffer__set_length__meth(lua_State *L) {
  LBuffer * this;
  size_t len;
  bool rc_l_buffer_set_length = 0;
  this = obj_type_LBuffer_check(L,1);
  len = luaL_checkinteger(L,2);
  rc_l_buffer_set_length = l_buffer_set_length(this, len);
  lua_pushboolean(L, rc_l_buffer_set_length);
  return 1;
}

/* method: data_ptr */
static int LBuffer__data_ptr__meth(lua_State *L) {
  LBuffer * this;
  void * rc_l_buffer_data = NULL;
  this = obj_type_LBuffer_check(L,1);
  rc_l_buffer_data = l_buffer_data(this);
  lua_pushlightuserdata(L, rc_l_buffer_data);
  return 1;
}

/* method: sub */
static int LBuffer__sub__meth(lua_State *L) {
  LBuffer * this;
  size_t off;
  size_t len;
  const char * data = NULL;
  this = obj_type_LBuffer_check(L,1);
  off = luaL_optinteger(L,2,0);
  len = luaL_optinteger(L,3,0);
  data = l_buffer_sub(this, off, &(len));
  if(data == NULL) lua_pushnil(L);  else lua_pushlstring(L, data,len);
  return 1;
}

/* method: size */
static int LBuffer__size__meth(lua_State *L) {
  LBuffer * this;
  size_t size = 0;
  this = obj_type_LBuffer_check(L,1);
  size = l_buffer_size(this);
  lua_pushinteger(L, size);
  return 1;
}

/* method: resize */
static int LBuffer__resize__meth(lua_State *L) {
  LBuffer * this;
  size_t len;
  bool rc_l_buffer_resize = 0;
  this = obj_type_LBuffer_check(L,1);
  len = luaL_checkinteger(L,2);
  rc_l_buffer_resize = l_buffer_resize(this, len);
  lua_pushboolean(L, rc_l_buffer_resize);
  return 1;
}

/* method: read_data */
static int LBuffer__read_data__meth(lua_State *L) {
  LBuffer * this;
  size_t len;
  const char * data = NULL;
  this = obj_type_LBuffer_check(L,1);
  len = luaL_checkinteger(L,2);
  data = l_buffer_read_data_len(this, len);
  if(data == NULL) lua_pushnil(L);  else lua_pushlstring(L, data,len);
  return 1;
}

/* method: read_string */
static int LBuffer__read_string__meth(lua_State *L) {
  LBuffer * this;
  size_t str_len = 0;
  const char * str = NULL;
  this = obj_type_LBuffer_check(L,1);
  str = l_buffer_read_string_len(this, &(str_len));
  if(str == NULL) lua_pushnil(L);  else lua_pushlstring(L, str,str_len);
  return 1;
}

/* method: read_uint8 */
static int LBuffer__read_uint8__meth(lua_State *L) {
  LBuffer * this;
  uint8_t num = 0;
  int rc_l_buffer_read_uint8_t = 0;
  this = obj_type_LBuffer_check(L,1);
  rc_l_buffer_read_uint8_t = l_buffer_read_uint8_t(this, &(num));
  lua_pushinteger(L, num);
  lua_pushinteger(L, rc_l_buffer_read_uint8_t);
  return 2;
}

/* method: read_int8 */
static int LBuffer__read_int8__meth(lua_State *L) {
  LBuffer * this;
  int8_t num = 0;
  int rc_l_buffer_read_uint8_t = 0;
  this = obj_type_LBuffer_check(L,1);
  rc_l_buffer_read_uint8_t = l_buffer_read_uint8_t(this, &(num));
  lua_pushinteger(L, num);
  lua_pushinteger(L, rc_l_buffer_read_uint8_t);
  return 2;
}

/* method: read_uint16 */
static int LBuffer__read_uint16__meth(lua_State *L) {
  LBuffer * this;
  uint16_t num = 0;
  int rc_l_buffer_read_uint16_t = 0;
  this = obj_type_LBuffer_check(L,1);
  rc_l_buffer_read_uint16_t = l_buffer_read_uint16_t(this, &(num));
  lua_pushinteger(L, num);
  lua_pushinteger(L, rc_l_buffer_read_uint16_t);
  return 2;
}

/* method: read_int16 */
static int LBuffer__read_int16__meth(lua_State *L) {
  LBuffer * this;
  int16_t num = 0;
  int rc_l_buffer_read_uint16_t = 0;
  this = obj_type_LBuffer_check(L,1);
  rc_l_buffer_read_uint16_t = l_buffer_read_uint16_t(this, &(num));
  lua_pushinteger(L, num);
  lua_pushinteger(L, rc_l_buffer_read_uint16_t);
  return 2;
}

/* method: read_uint32 */
static int LBuffer__read_uint32__meth(lua_State *L) {
  LBuffer * this;
  uint32_t num = 0;
  int rc_l_buffer_read_uint32_t = 0;
  this = obj_type_LBuffer_check(L,1);
  rc_l_buffer_read_uint32_t = l_buffer_read_uint32_t(this, &(num));
  lua_pushinteger(L, num);
  lua_pushinteger(L, rc_l_buffer_read_uint32_t);
  return 2;
}

/* method: read_int32 */
static int LBuffer__read_int32__meth(lua_State *L) {
  LBuffer * this;
  int32_t num = 0;
  int rc_l_buffer_read_uint32_t = 0;
  this = obj_type_LBuffer_check(L,1);
  rc_l_buffer_read_uint32_t = l_buffer_read_uint32_t(this, &(num));
  lua_pushinteger(L, num);
  lua_pushinteger(L, rc_l_buffer_read_uint32_t);
  return 2;
}

/* method: read_uint64 */
static int LBuffer__read_uint64__meth(lua_State *L) {
  LBuffer * this;
  uint64_t num = 0;
  int rc_l_buffer_read_uint64_t = 0;
  this = obj_type_LBuffer_check(L,1);
  rc_l_buffer_read_uint64_t = l_buffer_read_uint64_t(this, &(num));
  lua_pushinteger(L, num);
  lua_pushinteger(L, rc_l_buffer_read_uint64_t);
  return 2;
}

/* method: read_int64 */
static int LBuffer__read_int64__meth(lua_State *L) {
  LBuffer * this;
  int64_t num = 0;
  int rc_l_buffer_read_uint64_t = 0;
  this = obj_type_LBuffer_check(L,1);
  rc_l_buffer_read_uint64_t = l_buffer_read_uint64_t(this, &(num));
  lua_pushinteger(L, num);
  lua_pushinteger(L, rc_l_buffer_read_uint64_t);
  return 2;
}

/* method: read_float */
static int LBuffer__read_float__meth(lua_State *L) {
  LBuffer * this;
  float num = 0.0;
  int rc_l_buffer_read_float = 0;
  this = obj_type_LBuffer_check(L,1);
  rc_l_buffer_read_float = l_buffer_read_float(this, &(num));
  lua_pushnumber(L, num);
  lua_pushinteger(L, rc_l_buffer_read_float);
  return 2;
}

/* method: read_double */
static int LBuffer__read_double__meth(lua_State *L) {
  LBuffer * this;
  double num = 0.0;
  int rc_l_buffer_read_double = 0;
  this = obj_type_LBuffer_check(L,1);
  rc_l_buffer_read_double = l_buffer_read_double(this, &(num));
  lua_pushnumber(L, num);
  lua_pushinteger(L, rc_l_buffer_read_double);
  return 2;
}

/* method: read_b128_uvar32 */
static int LBuffer__read_b128_uvar32__meth(lua_State *L) {
  LBuffer * this;
  uint32_t num = 0;
  int rc_l_buffer_read_b128_uvar32 = 0;
  this = obj_type_LBuffer_check(L,1);
  rc_l_buffer_read_b128_uvar32 = l_buffer_read_b128_uvar32(this, &(num));
  lua_pushinteger(L, num);
  lua_pushinteger(L, rc_l_buffer_read_b128_uvar32);
  return 2;
}

/* method: read_b128_var32 */
static int LBuffer__read_b128_var32__meth(lua_State *L) {
  LBuffer * this;
  bool zigzag;
  int32_t num = 0;
  int rc_l_buffer_read_b128_var32 = 0;
  this = obj_type_LBuffer_check(L,1);
  zigzag = lua_toboolean(L,2);
  rc_l_buffer_read_b128_var32 = l_buffer_read_b128_var32(this, &(num), zigzag);
  lua_pushinteger(L, num);
  lua_pushinteger(L, rc_l_buffer_read_b128_var32);
  return 2;
}

/* method: read_b128_uvar64 */
static int LBuffer__read_b128_uvar64__meth(lua_State *L) {
  LBuffer * this;
  uint64_t num = 0;
  int rc_l_buffer_read_b128_uvar64 = 0;
  this = obj_type_LBuffer_check(L,1);
  rc_l_buffer_read_b128_uvar64 = l_buffer_read_b128_uvar64(this, &(num));
  lua_pushinteger(L, num);
  lua_pushinteger(L, rc_l_buffer_read_b128_uvar64);
  return 2;
}

/* method: read_b128_var64 */
static int LBuffer__read_b128_var64__meth(lua_State *L) {
  LBuffer * this;
  bool zigzag;
  int64_t num = 0;
  int rc_l_buffer_read_b128_var64 = 0;
  this = obj_type_LBuffer_check(L,1);
  zigzag = lua_toboolean(L,2);
  rc_l_buffer_read_b128_var64 = l_buffer_read_b128_var64(this, &(num), zigzag);
  lua_pushinteger(L, num);
  lua_pushinteger(L, rc_l_buffer_read_b128_var64);
  return 2;
}

/* method: append_data */
static int LBuffer__append_data__meth(lua_State *L) {
  LBuffer * this;
  size_t data_len;
  const char * data;
  int rc_l_buffer_append_data_len = 0;
  this = obj_type_LBuffer_check(L,1);
  data = luaL_checklstring(L,2,&(data_len));
  rc_l_buffer_append_data_len = l_buffer_append_data_len(this, data, data_len);
  lua_pushinteger(L, rc_l_buffer_append_data_len);
  return 1;
}

/* method: append_string */
static int LBuffer__append_string__meth(lua_State *L) {
  LBuffer * this;
  size_t str_len;
  const char * str;
  int rc_l_buffer_append_string_len = 0;
  this = obj_type_LBuffer_check(L,1);
  str = luaL_checklstring(L,2,&(str_len));
  rc_l_buffer_append_string_len = l_buffer_append_string_len(this, str, str_len);
  lua_pushinteger(L, rc_l_buffer_append_string_len);
  return 1;
}

/* method: append_uint8 */
static int LBuffer__append_uint8__meth(lua_State *L) {
  LBuffer * this;
  uint8_t num;
  int rc_l_buffer_append_uint8_t = 0;
  this = obj_type_LBuffer_check(L,1);
  num = luaL_checkinteger(L,2);
  rc_l_buffer_append_uint8_t = l_buffer_append_uint8_t(this, num);
  lua_pushinteger(L, rc_l_buffer_append_uint8_t);
  return 1;
}

/* method: append_int8 */
static int LBuffer__append_int8__meth(lua_State *L) {
  LBuffer * this;
  int8_t num;
  int rc_l_buffer_append_uint8_t = 0;
  this = obj_type_LBuffer_check(L,1);
  num = luaL_checkinteger(L,2);
  rc_l_buffer_append_uint8_t = l_buffer_append_uint8_t(this, num);
  lua_pushinteger(L, rc_l_buffer_append_uint8_t);
  return 1;
}

/* method: append_uint16 */
static int LBuffer__append_uint16__meth(lua_State *L) {
  LBuffer * this;
  uint16_t num;
  int rc_l_buffer_append_uint16_t = 0;
  this = obj_type_LBuffer_check(L,1);
  num = luaL_checkinteger(L,2);
  rc_l_buffer_append_uint16_t = l_buffer_append_uint16_t(this, num);
  lua_pushinteger(L, rc_l_buffer_append_uint16_t);
  return 1;
}

/* method: append_int16 */
static int LBuffer__append_int16__meth(lua_State *L) {
  LBuffer * this;
  int16_t num;
  int rc_l_buffer_append_uint16_t = 0;
  this = obj_type_LBuffer_check(L,1);
  num = luaL_checkinteger(L,2);
  rc_l_buffer_append_uint16_t = l_buffer_append_uint16_t(this, num);
  lua_pushinteger(L, rc_l_buffer_append_uint16_t);
  return 1;
}

/* method: append_uint32 */
static int LBuffer__append_uint32__meth(lua_State *L) {
  LBuffer * this;
  uint32_t num;
  int rc_l_buffer_append_uint32_t = 0;
  this = obj_type_LBuffer_check(L,1);
  num = luaL_checkinteger(L,2);
  rc_l_buffer_append_uint32_t = l_buffer_append_uint32_t(this, num);
  lua_pushinteger(L, rc_l_buffer_append_uint32_t);
  return 1;
}

/* method: append_int32 */
static int LBuffer__append_int32__meth(lua_State *L) {
  LBuffer * this;
  int32_t num;
  int rc_l_buffer_append_uint32_t = 0;
  this = obj_type_LBuffer_check(L,1);
  num = luaL_checkinteger(L,2);
  rc_l_buffer_append_uint32_t = l_buffer_append_uint32_t(this, num);
  lua_pushinteger(L, rc_l_buffer_append_uint32_t);
  return 1;
}

/* method: append_uint64 */
static int LBuffer__append_uint64__meth(lua_State *L) {
  LBuffer * this;
  uint64_t num;
  int rc_l_buffer_append_uint64_t = 0;
  this = obj_type_LBuffer_check(L,1);
  num = luaL_checkinteger(L,2);
  rc_l_buffer_append_uint64_t = l_buffer_append_uint64_t(this, num);
  lua_pushinteger(L, rc_l_buffer_append_uint64_t);
  return 1;
}

/* method: append_int64 */
static int LBuffer__append_int64__meth(lua_State *L) {
  LBuffer * this;
  int64_t num;
  int rc_l_buffer_append_uint64_t = 0;
  this = obj_type_LBuffer_check(L,1);
  num = luaL_checkinteger(L,2);
  rc_l_buffer_append_uint64_t = l_buffer_append_uint64_t(this, num);
  lua_pushinteger(L, rc_l_buffer_append_uint64_t);
  return 1;
}

/* method: append_float */
static int LBuffer__append_float__meth(lua_State *L) {
  LBuffer * this;
  float num;
  int rc_l_buffer_append_float = 0;
  this = obj_type_LBuffer_check(L,1);
  num = luaL_checknumber(L,2);
  rc_l_buffer_append_float = l_buffer_append_float(this, num);
  lua_pushinteger(L, rc_l_buffer_append_float);
  return 1;
}

/* method: append_double */
static int LBuffer__append_double__meth(lua_State *L) {
  LBuffer * this;
  double num;
  int rc_l_buffer_append_double = 0;
  this = obj_type_LBuffer_check(L,1);
  num = luaL_checknumber(L,2);
  rc_l_buffer_append_double = l_buffer_append_double(this, num);
  lua_pushinteger(L, rc_l_buffer_append_double);
  return 1;
}

/* method: append_b128_uvar32 */
static int LBuffer__append_b128_uvar32__meth(lua_State *L) {
  LBuffer * this;
  uint32_t num;
  int rc_l_buffer_append_b128_uvar32 = 0;
  this = obj_type_LBuffer_check(L,1);
  num = luaL_checkinteger(L,2);
  rc_l_buffer_append_b128_uvar32 = l_buffer_append_b128_uvar32(this, num);
  lua_pushinteger(L, rc_l_buffer_append_b128_uvar32);
  return 1;
}

/* method: append_b128_var32 */
static int LBuffer__append_b128_var32__meth(lua_State *L) {
  LBuffer * this;
  int32_t num;
  bool zigzag;
  int rc_l_buffer_append_b128_var32 = 0;
  this = obj_type_LBuffer_check(L,1);
  num = luaL_checkinteger(L,2);
  zigzag = lua_toboolean(L,3);
  rc_l_buffer_append_b128_var32 = l_buffer_append_b128_var32(this, num, zigzag);
  lua_pushinteger(L, rc_l_buffer_append_b128_var32);
  return 1;
}

/* method: append_b128_uvar64 */
static int LBuffer__append_b128_uvar64__meth(lua_State *L) {
  LBuffer * this;
  uint64_t num;
  int rc_l_buffer_append_b128_uvar64 = 0;
  this = obj_type_LBuffer_check(L,1);
  num = luaL_checkinteger(L,2);
  rc_l_buffer_append_b128_uvar64 = l_buffer_append_b128_uvar64(this, num);
  lua_pushinteger(L, rc_l_buffer_append_b128_uvar64);
  return 1;
}

/* method: append_b128_var64 */
static int LBuffer__append_b128_var64__meth(lua_State *L) {
  LBuffer * this;
  int64_t num;
  bool zigzag;
  int rc_l_buffer_append_b128_var64 = 0;
  this = obj_type_LBuffer_check(L,1);
  num = luaL_checkinteger(L,2);
  zigzag = lua_toboolean(L,3);
  rc_l_buffer_append_b128_var64 = l_buffer_append_b128_var64(this, num, zigzag);
  lua_pushinteger(L, rc_l_buffer_append_b128_var64);
  return 1;
}

/* method: new */
static int buf__new__func(lua_State *L) {
  LBuffer buf_store;
  LBuffer * buf = &(buf_store);
  const uint8_t * data = NULL;
  size_t len = 0;
	int ltype = lua_type(L, 1);

	if(ltype == LUA_TSTRING) {
		data = lua_tolstring(L, 1, &len);
	} else if(ltype == LUA_TNUMBER) {
		len = lua_tointeger(L, 1);
	}

  l_buffer_init(buf, data, len);
  obj_type_LBuffer_push(L, buf);
  return 1;
}



static const luaL_Reg obj_LBuffer_pub_funcs[] = {
  {NULL, NULL}
};

static const luaL_Reg obj_LBuffer_methods[] = {
  {"free", LBuffer__free__meth},
  {"tostring", LBuffer__tostring__meth},
  {"reset", LBuffer__reset__meth},
  {"length", LBuffer__length__meth},
  {"set_length", LBuffer__set_length__meth},
  {"data_ptr", LBuffer__data_ptr__meth},
  {"sub", LBuffer__sub__meth},
  {"size", LBuffer__size__meth},
  {"resize", LBuffer__resize__meth},
  {"read_data", LBuffer__read_data__meth},
  {"read_string", LBuffer__read_string__meth},
  {"read_uint8", LBuffer__read_uint8__meth},
  {"read_int8", LBuffer__read_int8__meth},
  {"read_uint16", LBuffer__read_uint16__meth},
  {"read_int16", LBuffer__read_int16__meth},
  {"read_uint32", LBuffer__read_uint32__meth},
  {"read_int32", LBuffer__read_int32__meth},
  {"read_uint64", LBuffer__read_uint64__meth},
  {"read_int64", LBuffer__read_int64__meth},
  {"read_float", LBuffer__read_float__meth},
  {"read_double", LBuffer__read_double__meth},
  {"read_b128_uvar32", LBuffer__read_b128_uvar32__meth},
  {"read_b128_var32", LBuffer__read_b128_var32__meth},
  {"read_b128_uvar64", LBuffer__read_b128_uvar64__meth},
  {"read_b128_var64", LBuffer__read_b128_var64__meth},
  {"append_data", LBuffer__append_data__meth},
  {"append_string", LBuffer__append_string__meth},
  {"append_uint8", LBuffer__append_uint8__meth},
  {"append_int8", LBuffer__append_int8__meth},
  {"append_uint16", LBuffer__append_uint16__meth},
  {"append_int16", LBuffer__append_int16__meth},
  {"append_uint32", LBuffer__append_uint32__meth},
  {"append_int32", LBuffer__append_int32__meth},
  {"append_uint64", LBuffer__append_uint64__meth},
  {"append_int64", LBuffer__append_int64__meth},
  {"append_float", LBuffer__append_float__meth},
  {"append_double", LBuffer__append_double__meth},
  {"append_b128_uvar32", LBuffer__append_b128_uvar32__meth},
  {"append_b128_var32", LBuffer__append_b128_var32__meth},
  {"append_b128_uvar64", LBuffer__append_b128_uvar64__meth},
  {"append_b128_var64", LBuffer__append_b128_var64__meth},
  {NULL, NULL}
};

static const luaL_Reg obj_LBuffer_metas[] = {
  {"__gc", LBuffer__free__meth},
  {"__tostring", LBuffer____tostring__meth},
  {"__len", LBuffer____len__meth},
  {"__eq", obj_simple_udata_default_equal},
  {NULL, NULL}
};

static const obj_base obj_LBuffer_bases[] = {
  {-1, NULL}
};

static const obj_field obj_LBuffer_fields[] = {
  {NULL, 0, 0, 0}
};

static const obj_const obj_LBuffer_constants[] = {
  {NULL, NULL, 0.0 , 0}
};

static const reg_impl obj_LBuffer_implements[] = {
  { "BufferIF", &(LBuffer_Buffer) },

  { "MutableBufferIF", &(LBuffer_MutableBuffer) },

  {NULL, NULL}
};

static const luaL_Reg buf_function[] = {
  {"new", buf__new__func},
  {NULL, NULL}
};

static const obj_const buf_constants[] = {
  {NULL, NULL, 0.0 , 0}
};



static const reg_sub_module reg_sub_modules[] = {
  { &(obj_type_LBuffer), REG_OBJECT, obj_LBuffer_pub_funcs, obj_LBuffer_methods, obj_LBuffer_metas, obj_LBuffer_bases, obj_LBuffer_fields, obj_LBuffer_constants, obj_LBuffer_implements, 0},
  {NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0}
};





#if LUAJIT_FFI
static const ffi_export_symbol buf_ffi_export[] = {
  {NULL, { NULL } }
};
#endif




static const luaL_Reg submodule_libs[] = {
  {NULL, NULL}
};



static void create_object_instance_cache(lua_State *L) {
	lua_pushlightuserdata(L, obj_udata_weak_ref_key); /* key for weak table. */
	lua_rawget(L, LUA_REGISTRYINDEX);  /* check if weak table exists already. */
	if(!lua_isnil(L, -1)) {
		lua_pop(L, 1); /* pop weak table. */
		return;
	}
	lua_pop(L, 1); /* pop nil. */
	/* create weak table for object instance references. */
	lua_pushlightuserdata(L, obj_udata_weak_ref_key); /* key for weak table. */
	lua_newtable(L);               /* weak table. */
	lua_newtable(L);               /* metatable for weak table. */
	lua_pushliteral(L, "__mode");
	lua_pushliteral(L, "v");
	lua_rawset(L, -3);             /* metatable.__mode = 'v'  weak values. */
	lua_setmetatable(L, -2);       /* add metatable to weak table. */
	lua_rawset(L, LUA_REGISTRYINDEX);  /* create reference to weak table. */
}

LUA_NOBJ_API int luaopen_buf(lua_State *L) {
	const reg_sub_module *reg = reg_sub_modules;
	const luaL_Reg *submodules = submodule_libs;
	int priv_table = -1;

	/* register interfaces */
	obj_register_interfaces(L, obj_interfaces);

	/* private table to hold reference to object metatables. */
	lua_newtable(L);
	priv_table = lua_gettop(L);
	lua_pushlightuserdata(L, obj_udata_private_key);
	lua_pushvalue(L, priv_table);
	lua_rawset(L, LUA_REGISTRYINDEX);  /* store private table in registry. */

	/* create object cache. */
	create_object_instance_cache(L);

	/* module table. */
#if REG_MODULES_AS_GLOBALS
	luaL_register(L, "buf", buf_function);
#else
	lua_newtable(L);
	luaL_setfuncs(L, buf_function, 0);
#endif

	/* register module constants. */
	obj_type_register_constants(L, buf_constants, -1, 0);

	for(; submodules->func != NULL ; submodules++) {
		lua_pushcfunction(L, submodules->func);
		lua_pushstring(L, submodules->name);
		lua_call(L, 1, 0);
	}

	/* register objects */
	for(; reg->type != NULL ; reg++) {
		lua_newtable(L); /* create public API table for object. */
		lua_pushvalue(L, -1); /* dup. object's public API table. */
		lua_setfield(L, -3, reg->type->name); /* module["<object_name>"] = <object public API> */
#if REG_OBJECTS_AS_GLOBALS
		lua_pushvalue(L, -1);                 /* dup value. */
		lua_setglobal(L, reg->type->name);    /* global: <object_name> = <object public API> */
#endif
		obj_type_register(L, reg, priv_table);
	}

#if LUAJIT_FFI
	if(nobj_check_ffi_support(L)) {
		nobj_try_loading_ffi(L, "buf.nobj.ffi.lua", buf_ffi_lua_code,
			buf_ffi_export, priv_table);
	}
#endif



	return 1;
}


