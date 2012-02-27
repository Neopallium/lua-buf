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

typedef struct reg_sub_module {
	obj_type        *type;
	module_reg_type req_type;
	const luaL_reg  *pub_funcs;
	const luaL_reg  *methods;
	const luaL_reg  *metas;
	const obj_base  *bases;
	const obj_field *fields;
	const obj_const *constants;
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
		return rc; /* return results of previous check. */
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

	/* turn-on hint that there is FFI code enabled. */
	if(rc) {
		nobj_ffi_support_enabled_hint = 1;
	}

	return rc;
}

static int nobj_try_loading_ffi(lua_State *L, const char *ffi_mod_name,
		const char *ffi_init_code, const ffi_export_symbol *ffi_exports, int priv_table)
{
	int err;

	/* export symbols to priv_table. */
	while(ffi_exports->name != NULL) {
		lua_pushstring(L, ffi_exports->name);
		lua_pushlightuserdata(L, ffi_exports->sym.data);
		lua_settable(L, priv_table);
		ffi_exports++;
	}
	err = luaL_loadbuffer(L, ffi_init_code, strlen(ffi_init_code), ffi_mod_name);
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

#ifndef REG_PACKAGE_IS_CONSTRUCTOR
#define REG_PACKAGE_IS_CONSTRUCTOR 1
#endif

#ifndef REG_MODULES_AS_GLOBALS
#define REG_MODULES_AS_GLOBALS 0
#endif

#ifndef REG_OBJECTS_AS_GLOBALS
#define REG_OBJECTS_AS_GLOBALS 0
#endif

#ifndef OBJ_DATA_HIDDEN_METATABLE
#define OBJ_DATA_HIDDEN_METATABLE 1
#endif

static FUNC_UNUSED obj_udata *obj_udata_toobj(lua_State *L, int _index) {
	obj_udata *ud;
	size_t len;

	/* make sure it's a userdata value. */
	ud = (obj_udata *)lua_touserdata(L, _index);
	if(ud == NULL) {
		luaL_typerror(L, _index, "userdata"); /* is not a userdata value. */
	}
	/* verify userdata size. */
	len = lua_objlen(L, _index);
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
	if(lua_rawequal(L, -1, -2)) {
		*obj = ud->obj;
		/* same type no casting needed. */
		return 1;
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
	} else {
		/* handle cdata. */
		/* get private table. */
		lua_pushlightuserdata(L, obj_udata_private_key);
		lua_rawget(L, LUA_REGISTRYINDEX); /* private table. */
		/* get cdata type check function from private table. */
		lua_pushlightuserdata(L, type);
		lua_rawget(L, -2);

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
	if(lua_isnil(L, _index)) {
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
			if(lua_rawequal(L, -1, -2)) {
				lua_pop(L, 2); /* pop both metatables. */
				return ud;
			}
		}
	} else {
		/* handle cdata. */
		/* get private table. */
		lua_pushlightuserdata(L, obj_udata_private_key);
		lua_rawget(L, LUA_REGISTRYINDEX); /* private table. */
		/* get cdata type check function from private table. */
		lua_pushlightuserdata(L, type);
		lua_rawget(L, -2);

		/* pass cdata value to type checking function. */
		lua_pushvalue(L, _index);
		lua_call(L, 1, 1);
		if(!lua_isnil(L, -1)) {
			/* valid type get pointer from cdata. */
			lua_pop(L, 2);
			return (void *)lua_topointer(L, _index);
		}
		lua_pop(L, 2);
	}
	luaL_typerror(L, _index, type->name); /* is not a userdata value. */
	return NULL;
}

static FUNC_UNUSED void * obj_simple_udata_luaoptional(lua_State *L, int _index, obj_type *type) {
	if(lua_isnil(L, _index)) {
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
	size_t len1 = lua_objlen(L, 1);
	void *ud2 = obj_simple_udata_toobj(L, 2);
	size_t len2 = lua_objlen(L, 2);

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
	const luaL_reg *reg_list = type_reg->pub_funcs;

	/* create public functions table. */
	if(reg_list != NULL && reg_list[0].name != NULL) {
		/* register functions */
		luaL_register(L, NULL, reg_list);
	}

	obj_type_register_constants(L, type_reg->constants, -1, type_reg->bidirectional_consts);

	lua_pop(L, 1);  /* drop package table */
}

static void obj_type_register_meta(lua_State *L, const reg_sub_module *type_reg) {
	const luaL_reg *reg_list;

	/* create public functions table. */
	reg_list = type_reg->pub_funcs;
	if(reg_list != NULL && reg_list[0].name != NULL) {
		/* register functions */
		luaL_register(L, NULL, reg_list);
	}

	obj_type_register_constants(L, type_reg->constants, -1, type_reg->bidirectional_consts);

	/* register methods. */
	luaL_register(L, NULL, type_reg->methods);

	/* create metatable table. */
	lua_newtable(L);
	luaL_register(L, NULL, type_reg->metas); /* fill metatable */
	/* setmetatable on meta-object. */
	lua_setmetatable(L, -2);

	lua_pop(L, 1);  /* drop meta-object */
}

static void obj_type_register(lua_State *L, const reg_sub_module *type_reg, int priv_table) {
	const luaL_reg *reg_list;
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
		luaL_register(L, NULL, reg_list); /* fill public API table. */

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

	luaL_register(L, NULL, type_reg->methods); /* fill methods table. */

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

	luaL_register(L, NULL, type_reg->metas); /* fill metatable */

	/* add obj_bases to metatable. */
	while(base->id >= 0) {
		lua_pushlightuserdata(L, (void *)base);
		lua_rawseti(L, -2, base->id);
		base++;
	}

	obj_type_register_constants(L, type_reg->constants, -2, type_reg->bidirectional_consts);

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



#define obj_type_LBuffer_check(L, _index) \
	(LBuffer *)obj_simple_udata_luacheck(L, _index, &(obj_type_LBuffer))
#define obj_type_LBuffer_optional(L, _index) \
	(LBuffer *)obj_simple_udata_luaoptional(L, _index, &(obj_type_LBuffer))
#define obj_type_LBuffer_delete(L, _index) \
	(LBuffer *)obj_simple_udata_luadelete(L, _index, &(obj_type_LBuffer))
#define obj_type_LBuffer_push(L, obj) \
	obj_simple_udata_luapush(L, obj, sizeof(LBuffer), &(obj_type_LBuffer))




static const char buf_ffi_lua_code[] = "local ffi=require\"ffi\"\n"
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
"local REG_MODULES_AS_GLOBALS = false\n"
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
"	return tonumber(ffi.cast('uintptr_t', ptr))\n"
"end\n"
"\n"
"local function obj_to_id(ptr)\n"
"	return tonumber(ffi.cast('uintptr_t', ffi.cast('void *', ptr)))\n"
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
"uint8_t *l_buffer_sub(LBuffer *buf, size_t off, size_t *plen);\n"
"\n"
"const uint8_t *l_buffer_read_data_len(LBuffer *buf, size_t len);\n"
"\n"
"const char *l_buffer_read_string_len(LBuffer *buf, size_t *plen);\n"
"\n"
"\n"
"void l_buffer_free(LBuffer *);\n"
"\n"
"const char * l_buffer_data(LBuffer *);\n"
"\n"
"size_t l_buffer_length(LBuffer *);\n"
"\n"
"void l_buffer_reset(LBuffer *);\n"
"\n"
"bool l_buffer_set_length(LBuffer *, size_t);\n"
"\n"
"void * l_buffer_data1(LBuffer *) asm(\"l_buffer_data\");\n"
"size_t l_buffer_size(LBuffer *);\n"
"\n"
"bool l_buffer_resize(LBuffer *, size_t);\n"
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
"\n"
"]]\n"
"\n"
"REG_MODULES_AS_GLOBALS = false\n"
"REG_OBJECTS_AS_GLOBALS = false\n"
"local _pub = {}\n"
"local _meth = {}\n"
"local _push = {}\n"
"local _obj_subs = {}\n"
"local _type_names = {}\n"
"local _ctypes = {}\n"
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
"\n"
"local obj_type_LBuffer_check\n"
"local obj_type_LBuffer_delete\n"
"local obj_type_LBuffer_push\n"
"\n"
"do\n"
"	local obj_mt = _priv.LBuffer\n"
"	local obj_type = obj_mt['.type']\n"
"	local obj_ctype = ffi.typeof(\"LBuffer\")\n"
"	_ctypes.LBuffer = obj_ctype\n"
"	_type_names.LBuffer = tostring(obj_ctype)\n"
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
"		if not ffi.istype(obj_type, val2) then return false end\n"
"		assert(ffi.istype(obj_type, val1), \"expected LBuffer\")\n"
"		return (C.memcmp(val1, val2, LBuffer_sizeof) == 0)\n"
"	end\n"
"\n"
"	-- type checking function for C API.\n"
"	_priv[obj_type] = function(obj)\n"
"		if ffi.istype(obj_type, obj) then return obj end\n"
"		return nil\n"
"	end\n"
"	-- push function for C API.\n"
"	reg_table[obj_type] = function(ptr)\n"
"		local obj = obj_ctype()\n"
"		ffi.copy(obj, ptr, LBuffer_sizeof);\n"
"		return obj\n"
"	end\n"
"\n"
"end\n"
"\n"
"\n"
"\n"
"-- Start \"LBuffer\" FFI interface\n"
"local LBuffer_tmp = ffi.new(\"LBuffer\")\n"
"\n"
"-- method: new\n"
"function _pub.LBuffer.new(size_or_data)\n"
"  local self = ffi.new(\"LBuffer\")\n"
"	local buf\n"
"	local data\n"
"	local len\n"
"	local ltype = type(size_or_data)\n"
"\n"
"	if ltype == 'string' then\n"
"		data = size_or_data\n"
"		len = #data\n"
"	elseif ltype == 'number' then\n"
"		len = size_or_data\n"
"	end\n"
"\n"
"	self = LBuffer_tmp\n"
"	C.l_buffer_init(self, data, len)\n"
"\n"
"  return obj_type_LBuffer_push(self)\n"
"end\n"
"register_default_constructor(_pub,\"LBuffer\",_pub.LBuffer.new)\n"
"\n"
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
"  local rc_l_buffer_length = 0\n"
"  rc_l_buffer_length = C.l_buffer_length(self)\n"
"  return rc_l_buffer_length\n"
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
"local sub_len_tmp = ffi.new(\"size_t[1]\")\n"
"\n"
"-- method: sub\n"
"function _meth.LBuffer.sub(self, off, len)\n"
"  \n"
"    off = off or 0\n"
"    len = len or 0\n"
"  local data_len = 0\n"
"  local data\n"
"	sub_len_tmp[0] = len;\n"
"	data = C.l_buffer_sub(self, off, sub_len_tmp);\n"
"	data_len = sub_len_tmp[0];\n"
"\n"
"  return ffi_string_len(data,data_len)\n"
"end\n"
"\n"
"-- method: size\n"
"function _meth.LBuffer.size(self)\n"
"  \n"
"  local rc_l_buffer_size = 0\n"
"  rc_l_buffer_size = C.l_buffer_size(self)\n"
"  return rc_l_buffer_size\n"
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
"  local data_len = 0\n"
"  local data\n"
"	data = C.l_buffer_read_data_len(self, len)\n"
"	data_len = len\n"
"\n"
"  return ffi_string_len(data,data_len)\n"
"end\n"
"\n"
"local read_string_len_tmp = ffi.new(\"size_t[1]\")\n"
"\n"
"-- method: read_string\n"
"function _meth.LBuffer.read_string(self)\n"
"  \n"
"  local str_len = 0\n"
"  local str\n"
"	str_len = read_string_len_tmp\n"
"	str = C.l_buffer_read_string_len(self, str_len)\n"
"	str_len = str_len[0]\n"
"\n"
"  return ffi_string_len(str,str_len)\n"
"end\n"
"\n"
"do\n"
"  local read_uint8_num_tmp = ffi.new(\"uint8_t[1]\")\n"
"-- method: read_uint8\n"
"function _meth.LBuffer.read_uint8(self)\n"
"  \n"
"  local num = read_uint8_num_tmp\n"
"  local rc_l_buffer_read_uint8_t = 0\n"
"  rc_l_buffer_read_uint8_t = C.l_buffer_read_uint8_t(self, num)\n"
"  return num[0], rc_l_buffer_read_uint8_t\n"
"end\n"
"end\n"
"\n"
"do\n"
"  local read_int8_num_tmp = ffi.new(\"int8_t[1]\")\n"
"-- method: read_int8\n"
"function _meth.LBuffer.read_int8(self)\n"
"  \n"
"  local num = read_int8_num_tmp\n"
"  local rc_l_buffer_read_uint8_t = 0\n"
"  rc_l_buffer_read_uint8_t = C.l_buffer_read_uint8_t1(self, num)\n"
"  return num[0], rc_l_buffer_read_uint8_t\n"
"end\n"
"end\n"
"\n"
"do\n"
"  local read_uint16_num_tmp = ffi.new(\"uint16_t[1]\")\n"
"-- method: read_uint16\n"
"function _meth.LBuffer.read_uint16(self)\n"
"  \n"
"  local num = read_uint16_num_tmp\n"
"  local rc_l_buffer_read_uint16_t = 0\n"
"  rc_l_buffer_read_uint16_t = C.l_buffer_read_uint16_t(self, num)\n"
"  return num[0], rc_l_buffer_read_uint16_t\n"
"end\n"
"end\n"
"\n"
"do\n"
"  local read_int16_num_tmp = ffi.new(\"int16_t[1]\")\n"
"-- method: read_int16\n"
"function _meth.LBuffer.read_int16(self)\n"
"  \n"
"  local num = read_int16_num_tmp\n"
"  local rc_l_buffer_read_uint16_t = 0\n"
"  rc_l_buffer_read_uint16_t = C.l_buffer_read_uint16_t1(self, num)\n"
"  return num[0], rc_l_buffer_read_uint16_t\n"
"end\n"
"end\n"
"\n"
"do\n"
"  local read_uint32_num_tmp = ffi.new(\"uint32_t[1]\")\n"
"-- method: read_uint32\n"
"function _meth.LBuffer.read_uint32(self)\n"
"  \n"
"  local num = read_uint32_num_tmp\n"
"  local rc_l_buffer_read_uint32_t = 0\n"
"  rc_l_buffer_read_uint32_t = C.l_buffer_read_uint32_t(self, num)\n"
"  return num[0], rc_l_buffer_read_uint32_t\n"
"end\n"
"end\n"
"\n"
"do\n"
"  local read_int32_num_tmp = ffi.new(\"int32_t[1]\")\n"
"-- method: read_int32\n"
"function _meth.LBuffer.read_int32(self)\n"
"  \n"
"  local num = read_int32_num_tmp\n"
"  local rc_l_buffer_read_uint32_t = 0\n"
"  rc_l_buffer_read_uint32_t = C.l_buffer_read_uint32_t1(self, num)\n"
"  return num[0], rc_l_buffer_read_uint32_t\n"
"end\n"
"end\n"
"\n"
"do\n"
"  local read_uint64_num_tmp = ffi.new(\"uint64_t[1]\")\n"
"-- method: read_uint64\n"
"function _meth.LBuffer.read_uint64(self)\n"
"  \n"
"  local num = read_uint64_num_tmp\n"
"  local rc_l_buffer_read_uint64_t = 0\n"
"  rc_l_buffer_read_uint64_t = C.l_buffer_read_uint64_t(self, num)\n"
"  return num[0], rc_l_buffer_read_uint64_t\n"
"end\n"
"end\n"
"\n"
"do\n"
"  local read_int64_num_tmp = ffi.new(\"int64_t[1]\")\n"
"-- method: read_int64\n"
"function _meth.LBuffer.read_int64(self)\n"
"  \n"
"  local num = read_int64_num_tmp\n"
"  local rc_l_buffer_read_uint64_t = 0\n"
"  rc_l_buffer_read_uint64_t = C.l_buffer_read_uint64_t1(self, num)\n"
"  return num[0], rc_l_buffer_read_uint64_t\n"
"end\n"
"end\n"
"\n"
"do\n"
"  local read_float_num_tmp = ffi.new(\"float[1]\")\n"
"-- method: read_float\n"
"function _meth.LBuffer.read_float(self)\n"
"  \n"
"  local num = read_float_num_tmp\n"
"  local rc_l_buffer_read_float = 0\n"
"  rc_l_buffer_read_float = C.l_buffer_read_float(self, num)\n"
"  return num[0], rc_l_buffer_read_float\n"
"end\n"
"end\n"
"\n"
"do\n"
"  local read_double_num_tmp = ffi.new(\"double[1]\")\n"
"-- method: read_double\n"
"function _meth.LBuffer.read_double(self)\n"
"  \n"
"  local num = read_double_num_tmp\n"
"  local rc_l_buffer_read_double = 0\n"
"  rc_l_buffer_read_double = C.l_buffer_read_double(self, num)\n"
"  return num[0], rc_l_buffer_read_double\n"
"end\n"
"end\n"
"\n"
"do\n"
"  local read_b128_uvar32_num_tmp = ffi.new(\"uint32_t[1]\")\n"
"-- method: read_b128_uvar32\n"
"function _meth.LBuffer.read_b128_uvar32(self)\n"
"  \n"
"  local num = read_b128_uvar32_num_tmp\n"
"  local rc_l_buffer_read_b128_uvar32 = 0\n"
"  rc_l_buffer_read_b128_uvar32 = C.l_buffer_read_b128_uvar32(self, num)\n"
"  return num[0], rc_l_buffer_read_b128_uvar32\n"
"end\n"
"end\n"
"\n"
"do\n"
"  local read_b128_var32_num_tmp = ffi.new(\"int32_t[1]\")\n"
"-- method: read_b128_var32\n"
"function _meth.LBuffer.read_b128_var32(self, zigzag)\n"
"  \n"
"    zigzag = zigzag or 0\n"
"  local num = read_b128_var32_num_tmp\n"
"  local rc_l_buffer_read_b128_var32 = 0\n"
"  rc_l_buffer_read_b128_var32 = C.l_buffer_read_b128_var32(self, num, zigzag)\n"
"  return num[0], rc_l_buffer_read_b128_var32\n"
"end\n"
"end\n"
"\n"
"do\n"
"  local read_b128_uvar64_num_tmp = ffi.new(\"uint64_t[1]\")\n"
"-- method: read_b128_uvar64\n"
"function _meth.LBuffer.read_b128_uvar64(self)\n"
"  \n"
"  local num = read_b128_uvar64_num_tmp\n"
"  local rc_l_buffer_read_b128_uvar64 = 0\n"
"  rc_l_buffer_read_b128_uvar64 = C.l_buffer_read_b128_uvar64(self, num)\n"
"  return num[0], rc_l_buffer_read_b128_uvar64\n"
"end\n"
"end\n"
"\n"
"do\n"
"  local read_b128_var64_num_tmp = ffi.new(\"int64_t[1]\")\n"
"-- method: read_b128_var64\n"
"function _meth.LBuffer.read_b128_var64(self, zigzag)\n"
"  \n"
"    zigzag = zigzag or 0\n"
"  local num = read_b128_var64_num_tmp\n"
"  local rc_l_buffer_read_b128_var64 = 0\n"
"  rc_l_buffer_read_b128_var64 = C.l_buffer_read_b128_var64(self, num, zigzag)\n"
"  return num[0], rc_l_buffer_read_b128_var64\n"
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
"    zigzag = zigzag or 0\n"
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
"    zigzag = zigzag or 0\n"
"  local rc_l_buffer_append_b128_var64 = 0\n"
"  rc_l_buffer_append_b128_var64 = C.l_buffer_append_b128_var64(self, num, zigzag)\n"
"  return rc_l_buffer_append_b128_var64\n"
"end\n"
"\n"
"_push.LBuffer = obj_type_LBuffer_push\n"
"ffi.metatype(\"LBuffer\", _priv.LBuffer)\n"
"-- End \"LBuffer\" FFI interface\n"
"\n"
"";


/* method: new */
static int LBuffer__new__meth(lua_State *L) {
  LBuffer this_store;
  LBuffer * this = &(this_store);
	LBuffer buf;
	const uint8_t *data = NULL;
	size_t len = 0;
	int ltype = lua_type(L, 1);

	if(ltype == LUA_TSTRING) {
		data = lua_tolstring(L, 1, &len);
	} else if(ltype == LUA_TNUMBER) {
		len = lua_tointeger(L, 1);
	}

	this = &buf;
	l_buffer_init(this, data, len);

  obj_type_LBuffer_push(L, this);
  return 1;
}

/* method: free */
static int LBuffer__free__meth(lua_State *L) {
  LBuffer * this = obj_type_LBuffer_delete(L,1);
  l_buffer_free(this);
  return 0;
}

/* method: __tostring */
static int LBuffer____tostring__meth(lua_State *L) {
  LBuffer * this = obj_type_LBuffer_check(L,1);
  size_t data_len = 0;
  const char * data = NULL;
  data = l_buffer_data(this);
  data_len = l_buffer_length(this);
  if(data == NULL) lua_pushnil(L);  else lua_pushlstring(L, data,data_len);
  return 1;
}

/* method: reset */
static int LBuffer__reset__meth(lua_State *L) {
  LBuffer * this = obj_type_LBuffer_check(L,1);
  l_buffer_reset(this);
  return 0;
}

/* method: length */
static int LBuffer__length__meth(lua_State *L) {
  LBuffer * this = obj_type_LBuffer_check(L,1);
  size_t rc_l_buffer_length = 0;
  rc_l_buffer_length = l_buffer_length(this);
  lua_pushinteger(L, rc_l_buffer_length);
  return 1;
}

/* method: set_length */
static int LBuffer__set_length__meth(lua_State *L) {
  LBuffer * this = obj_type_LBuffer_check(L,1);
  size_t len = luaL_checkinteger(L,2);
  bool rc_l_buffer_set_length = 0;
  rc_l_buffer_set_length = l_buffer_set_length(this, len);
  lua_pushboolean(L, rc_l_buffer_set_length);
  return 1;
}

/* method: data_ptr */
static int LBuffer__data_ptr__meth(lua_State *L) {
  LBuffer * this = obj_type_LBuffer_check(L,1);
  void * rc_l_buffer_data = NULL;
  rc_l_buffer_data = l_buffer_data(this);
  lua_pushlightuserdata(L, rc_l_buffer_data);
  return 1;
}

/* method: sub */
static int LBuffer__sub__meth(lua_State *L) {
  LBuffer * this = obj_type_LBuffer_check(L,1);
  size_t off = luaL_optinteger(L,2,0);
  size_t len = luaL_optinteger(L,3,0);
  size_t data_len = 0;
  const char * data = NULL;
	data_len = len;
	data = l_buffer_sub(this, off, &(data_len));

  if(data == NULL) lua_pushnil(L);  else lua_pushlstring(L, data,data_len);
  return 1;
}

/* method: size */
static int LBuffer__size__meth(lua_State *L) {
  LBuffer * this = obj_type_LBuffer_check(L,1);
  size_t rc_l_buffer_size = 0;
  rc_l_buffer_size = l_buffer_size(this);
  lua_pushinteger(L, rc_l_buffer_size);
  return 1;
}

/* method: resize */
static int LBuffer__resize__meth(lua_State *L) {
  LBuffer * this = obj_type_LBuffer_check(L,1);
  size_t len = luaL_checkinteger(L,2);
  bool rc_l_buffer_resize = 0;
  rc_l_buffer_resize = l_buffer_resize(this, len);
  lua_pushboolean(L, rc_l_buffer_resize);
  return 1;
}

/* method: read_data */
static int LBuffer__read_data__meth(lua_State *L) {
  LBuffer * this = obj_type_LBuffer_check(L,1);
  size_t len = luaL_checkinteger(L,2);
  size_t data_len = 0;
  const char * data = NULL;
	data = l_buffer_read_data_len(this, len);
	data_len = len;

  if(data == NULL) lua_pushnil(L);  else lua_pushlstring(L, data,data_len);
  return 1;
}

/* method: read_string */
static int LBuffer__read_string__meth(lua_State *L) {
  LBuffer * this = obj_type_LBuffer_check(L,1);
  size_t str_len = 0;
  const char * str = NULL;
	str = l_buffer_read_string_len(this, &(str_len));

  if(str == NULL) lua_pushnil(L);  else lua_pushlstring(L, str,str_len);
  return 1;
}

/* method: read_uint8 */
static int LBuffer__read_uint8__meth(lua_State *L) {
  LBuffer * this = obj_type_LBuffer_check(L,1);
  uint8_t num = 0;
  int rc_l_buffer_read_uint8_t = 0;
  rc_l_buffer_read_uint8_t = l_buffer_read_uint8_t(this, &(num));
  lua_pushinteger(L, num);
  lua_pushinteger(L, rc_l_buffer_read_uint8_t);
  return 2;
}

/* method: read_int8 */
static int LBuffer__read_int8__meth(lua_State *L) {
  LBuffer * this = obj_type_LBuffer_check(L,1);
  int8_t num = 0;
  int rc_l_buffer_read_uint8_t = 0;
  rc_l_buffer_read_uint8_t = l_buffer_read_uint8_t(this, &(num));
  lua_pushinteger(L, num);
  lua_pushinteger(L, rc_l_buffer_read_uint8_t);
  return 2;
}

/* method: read_uint16 */
static int LBuffer__read_uint16__meth(lua_State *L) {
  LBuffer * this = obj_type_LBuffer_check(L,1);
  uint16_t num = 0;
  int rc_l_buffer_read_uint16_t = 0;
  rc_l_buffer_read_uint16_t = l_buffer_read_uint16_t(this, &(num));
  lua_pushinteger(L, num);
  lua_pushinteger(L, rc_l_buffer_read_uint16_t);
  return 2;
}

/* method: read_int16 */
static int LBuffer__read_int16__meth(lua_State *L) {
  LBuffer * this = obj_type_LBuffer_check(L,1);
  int16_t num = 0;
  int rc_l_buffer_read_uint16_t = 0;
  rc_l_buffer_read_uint16_t = l_buffer_read_uint16_t(this, &(num));
  lua_pushinteger(L, num);
  lua_pushinteger(L, rc_l_buffer_read_uint16_t);
  return 2;
}

/* method: read_uint32 */
static int LBuffer__read_uint32__meth(lua_State *L) {
  LBuffer * this = obj_type_LBuffer_check(L,1);
  uint32_t num = 0;
  int rc_l_buffer_read_uint32_t = 0;
  rc_l_buffer_read_uint32_t = l_buffer_read_uint32_t(this, &(num));
  lua_pushinteger(L, num);
  lua_pushinteger(L, rc_l_buffer_read_uint32_t);
  return 2;
}

/* method: read_int32 */
static int LBuffer__read_int32__meth(lua_State *L) {
  LBuffer * this = obj_type_LBuffer_check(L,1);
  int32_t num = 0;
  int rc_l_buffer_read_uint32_t = 0;
  rc_l_buffer_read_uint32_t = l_buffer_read_uint32_t(this, &(num));
  lua_pushinteger(L, num);
  lua_pushinteger(L, rc_l_buffer_read_uint32_t);
  return 2;
}

/* method: read_uint64 */
static int LBuffer__read_uint64__meth(lua_State *L) {
  LBuffer * this = obj_type_LBuffer_check(L,1);
  uint64_t num = 0;
  int rc_l_buffer_read_uint64_t = 0;
  rc_l_buffer_read_uint64_t = l_buffer_read_uint64_t(this, &(num));
  lua_pushinteger(L, num);
  lua_pushinteger(L, rc_l_buffer_read_uint64_t);
  return 2;
}

/* method: read_int64 */
static int LBuffer__read_int64__meth(lua_State *L) {
  LBuffer * this = obj_type_LBuffer_check(L,1);
  int64_t num = 0;
  int rc_l_buffer_read_uint64_t = 0;
  rc_l_buffer_read_uint64_t = l_buffer_read_uint64_t(this, &(num));
  lua_pushinteger(L, num);
  lua_pushinteger(L, rc_l_buffer_read_uint64_t);
  return 2;
}

/* method: read_float */
static int LBuffer__read_float__meth(lua_State *L) {
  LBuffer * this = obj_type_LBuffer_check(L,1);
  float num = 0.0;
  int rc_l_buffer_read_float = 0;
  rc_l_buffer_read_float = l_buffer_read_float(this, &(num));
  lua_pushnumber(L, num);
  lua_pushinteger(L, rc_l_buffer_read_float);
  return 2;
}

/* method: read_double */
static int LBuffer__read_double__meth(lua_State *L) {
  LBuffer * this = obj_type_LBuffer_check(L,1);
  double num = 0.0;
  int rc_l_buffer_read_double = 0;
  rc_l_buffer_read_double = l_buffer_read_double(this, &(num));
  lua_pushnumber(L, num);
  lua_pushinteger(L, rc_l_buffer_read_double);
  return 2;
}

/* method: read_b128_uvar32 */
static int LBuffer__read_b128_uvar32__meth(lua_State *L) {
  LBuffer * this = obj_type_LBuffer_check(L,1);
  uint32_t num = 0;
  int rc_l_buffer_read_b128_uvar32 = 0;
  rc_l_buffer_read_b128_uvar32 = l_buffer_read_b128_uvar32(this, &(num));
  lua_pushinteger(L, num);
  lua_pushinteger(L, rc_l_buffer_read_b128_uvar32);
  return 2;
}

/* method: read_b128_var32 */
static int LBuffer__read_b128_var32__meth(lua_State *L) {
  LBuffer * this = obj_type_LBuffer_check(L,1);
  bool zigzag = lua_toboolean(L,2);
  int32_t num = 0;
  int rc_l_buffer_read_b128_var32 = 0;
  rc_l_buffer_read_b128_var32 = l_buffer_read_b128_var32(this, &(num), zigzag);
  lua_pushinteger(L, num);
  lua_pushinteger(L, rc_l_buffer_read_b128_var32);
  return 2;
}

/* method: read_b128_uvar64 */
static int LBuffer__read_b128_uvar64__meth(lua_State *L) {
  LBuffer * this = obj_type_LBuffer_check(L,1);
  uint64_t num = 0;
  int rc_l_buffer_read_b128_uvar64 = 0;
  rc_l_buffer_read_b128_uvar64 = l_buffer_read_b128_uvar64(this, &(num));
  lua_pushinteger(L, num);
  lua_pushinteger(L, rc_l_buffer_read_b128_uvar64);
  return 2;
}

/* method: read_b128_var64 */
static int LBuffer__read_b128_var64__meth(lua_State *L) {
  LBuffer * this = obj_type_LBuffer_check(L,1);
  bool zigzag = lua_toboolean(L,2);
  int64_t num = 0;
  int rc_l_buffer_read_b128_var64 = 0;
  rc_l_buffer_read_b128_var64 = l_buffer_read_b128_var64(this, &(num), zigzag);
  lua_pushinteger(L, num);
  lua_pushinteger(L, rc_l_buffer_read_b128_var64);
  return 2;
}

/* method: append_data */
static int LBuffer__append_data__meth(lua_State *L) {
  LBuffer * this = obj_type_LBuffer_check(L,1);
  size_t data_len;
  const char * data = luaL_checklstring(L,2,&(data_len));
  int rc_l_buffer_append_data_len = 0;
  rc_l_buffer_append_data_len = l_buffer_append_data_len(this, data, data_len);
  lua_pushinteger(L, rc_l_buffer_append_data_len);
  return 1;
}

/* method: append_string */
static int LBuffer__append_string__meth(lua_State *L) {
  LBuffer * this = obj_type_LBuffer_check(L,1);
  size_t str_len;
  const char * str = luaL_checklstring(L,2,&(str_len));
  int rc_l_buffer_append_string_len = 0;
  rc_l_buffer_append_string_len = l_buffer_append_string_len(this, str, str_len);
  lua_pushinteger(L, rc_l_buffer_append_string_len);
  return 1;
}

/* method: append_uint8 */
static int LBuffer__append_uint8__meth(lua_State *L) {
  LBuffer * this = obj_type_LBuffer_check(L,1);
  uint8_t num = luaL_checkinteger(L,2);
  int rc_l_buffer_append_uint8_t = 0;
  rc_l_buffer_append_uint8_t = l_buffer_append_uint8_t(this, num);
  lua_pushinteger(L, rc_l_buffer_append_uint8_t);
  return 1;
}

/* method: append_int8 */
static int LBuffer__append_int8__meth(lua_State *L) {
  LBuffer * this = obj_type_LBuffer_check(L,1);
  int8_t num = luaL_checkinteger(L,2);
  int rc_l_buffer_append_uint8_t = 0;
  rc_l_buffer_append_uint8_t = l_buffer_append_uint8_t(this, num);
  lua_pushinteger(L, rc_l_buffer_append_uint8_t);
  return 1;
}

/* method: append_uint16 */
static int LBuffer__append_uint16__meth(lua_State *L) {
  LBuffer * this = obj_type_LBuffer_check(L,1);
  uint16_t num = luaL_checkinteger(L,2);
  int rc_l_buffer_append_uint16_t = 0;
  rc_l_buffer_append_uint16_t = l_buffer_append_uint16_t(this, num);
  lua_pushinteger(L, rc_l_buffer_append_uint16_t);
  return 1;
}

/* method: append_int16 */
static int LBuffer__append_int16__meth(lua_State *L) {
  LBuffer * this = obj_type_LBuffer_check(L,1);
  int16_t num = luaL_checkinteger(L,2);
  int rc_l_buffer_append_uint16_t = 0;
  rc_l_buffer_append_uint16_t = l_buffer_append_uint16_t(this, num);
  lua_pushinteger(L, rc_l_buffer_append_uint16_t);
  return 1;
}

/* method: append_uint32 */
static int LBuffer__append_uint32__meth(lua_State *L) {
  LBuffer * this = obj_type_LBuffer_check(L,1);
  uint32_t num = luaL_checkinteger(L,2);
  int rc_l_buffer_append_uint32_t = 0;
  rc_l_buffer_append_uint32_t = l_buffer_append_uint32_t(this, num);
  lua_pushinteger(L, rc_l_buffer_append_uint32_t);
  return 1;
}

/* method: append_int32 */
static int LBuffer__append_int32__meth(lua_State *L) {
  LBuffer * this = obj_type_LBuffer_check(L,1);
  int32_t num = luaL_checkinteger(L,2);
  int rc_l_buffer_append_uint32_t = 0;
  rc_l_buffer_append_uint32_t = l_buffer_append_uint32_t(this, num);
  lua_pushinteger(L, rc_l_buffer_append_uint32_t);
  return 1;
}

/* method: append_uint64 */
static int LBuffer__append_uint64__meth(lua_State *L) {
  LBuffer * this = obj_type_LBuffer_check(L,1);
  uint64_t num = luaL_checkinteger(L,2);
  int rc_l_buffer_append_uint64_t = 0;
  rc_l_buffer_append_uint64_t = l_buffer_append_uint64_t(this, num);
  lua_pushinteger(L, rc_l_buffer_append_uint64_t);
  return 1;
}

/* method: append_int64 */
static int LBuffer__append_int64__meth(lua_State *L) {
  LBuffer * this = obj_type_LBuffer_check(L,1);
  int64_t num = luaL_checkinteger(L,2);
  int rc_l_buffer_append_uint64_t = 0;
  rc_l_buffer_append_uint64_t = l_buffer_append_uint64_t(this, num);
  lua_pushinteger(L, rc_l_buffer_append_uint64_t);
  return 1;
}

/* method: append_float */
static int LBuffer__append_float__meth(lua_State *L) {
  LBuffer * this = obj_type_LBuffer_check(L,1);
  float num = luaL_checknumber(L,2);
  int rc_l_buffer_append_float = 0;
  rc_l_buffer_append_float = l_buffer_append_float(this, num);
  lua_pushinteger(L, rc_l_buffer_append_float);
  return 1;
}

/* method: append_double */
static int LBuffer__append_double__meth(lua_State *L) {
  LBuffer * this = obj_type_LBuffer_check(L,1);
  double num = luaL_checknumber(L,2);
  int rc_l_buffer_append_double = 0;
  rc_l_buffer_append_double = l_buffer_append_double(this, num);
  lua_pushinteger(L, rc_l_buffer_append_double);
  return 1;
}

/* method: append_b128_uvar32 */
static int LBuffer__append_b128_uvar32__meth(lua_State *L) {
  LBuffer * this = obj_type_LBuffer_check(L,1);
  uint32_t num = luaL_checkinteger(L,2);
  int rc_l_buffer_append_b128_uvar32 = 0;
  rc_l_buffer_append_b128_uvar32 = l_buffer_append_b128_uvar32(this, num);
  lua_pushinteger(L, rc_l_buffer_append_b128_uvar32);
  return 1;
}

/* method: append_b128_var32 */
static int LBuffer__append_b128_var32__meth(lua_State *L) {
  LBuffer * this = obj_type_LBuffer_check(L,1);
  int32_t num = luaL_checkinteger(L,2);
  bool zigzag = lua_toboolean(L,3);
  int rc_l_buffer_append_b128_var32 = 0;
  rc_l_buffer_append_b128_var32 = l_buffer_append_b128_var32(this, num, zigzag);
  lua_pushinteger(L, rc_l_buffer_append_b128_var32);
  return 1;
}

/* method: append_b128_uvar64 */
static int LBuffer__append_b128_uvar64__meth(lua_State *L) {
  LBuffer * this = obj_type_LBuffer_check(L,1);
  uint64_t num = luaL_checkinteger(L,2);
  int rc_l_buffer_append_b128_uvar64 = 0;
  rc_l_buffer_append_b128_uvar64 = l_buffer_append_b128_uvar64(this, num);
  lua_pushinteger(L, rc_l_buffer_append_b128_uvar64);
  return 1;
}

/* method: append_b128_var64 */
static int LBuffer__append_b128_var64__meth(lua_State *L) {
  LBuffer * this = obj_type_LBuffer_check(L,1);
  int64_t num = luaL_checkinteger(L,2);
  bool zigzag = lua_toboolean(L,3);
  int rc_l_buffer_append_b128_var64 = 0;
  rc_l_buffer_append_b128_var64 = l_buffer_append_b128_var64(this, num, zigzag);
  lua_pushinteger(L, rc_l_buffer_append_b128_var64);
  return 1;
}

/* method: new */
static int buf__new__func(lua_State *L) {
  LBuffer this_store;
  LBuffer * this = &(this_store);
	LBuffer buf;
	const uint8_t *data = NULL;
	size_t len = 0;
	int ltype = lua_type(L, 1);

	if(ltype == LUA_TSTRING) {
		data = lua_tolstring(L, 1, &len);
	} else if(ltype == LUA_TNUMBER) {
		len = lua_tointeger(L, 1);
	}

	this = &buf;
	l_buffer_init(this, data, len);

  obj_type_LBuffer_push(L, this);
  return 1;
}



static const luaL_reg obj_LBuffer_pub_funcs[] = {
  {"new", LBuffer__new__meth},
  {NULL, NULL}
};

static const luaL_reg obj_LBuffer_methods[] = {
  {"free", LBuffer__free__meth},
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

static const luaL_reg obj_LBuffer_metas[] = {
  {"__gc", LBuffer__free__meth},
  {"__tostring", LBuffer____tostring__meth},
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

static const luaL_reg buf_function[] = {
  {"new", buf__new__func},
  {NULL, NULL}
};

static const obj_const buf_constants[] = {
  {NULL, NULL, 0.0 , 0}
};



static const reg_sub_module reg_sub_modules[] = {
  { &(obj_type_LBuffer), REG_OBJECT, obj_LBuffer_pub_funcs, obj_LBuffer_methods, obj_LBuffer_metas, obj_LBuffer_bases, obj_LBuffer_fields, obj_LBuffer_constants, 0},
  {NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, 0}
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
	luaL_register(L, NULL, buf_function);
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
		nobj_try_loading_ffi(L, "buf", buf_ffi_lua_code,
			buf_ffi_export, priv_table);
	}
#endif



	return 1;
}


