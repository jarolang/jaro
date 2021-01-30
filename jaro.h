/*
** Jaro - a very tiny and simple to learn programming Language
** See Copyright Notice at the end of this file
*/


#ifndef jaro_h
#define jaro_h

#include <stdarg.h>
#include <stddef.h>


#include "jaroconf.h"


#define JARO_VERSION_MAJOR	"1"
#define JARO_VERSION_MINOR	"0"
#define JARO_VERSION_NUM		100
#define JARO_VERSION_RELEASE	"0"

#define JARO_VERSION	"Jaro " JARO_VERSION_MAJOR "." JARO_VERSION_MINOR
#define JARO_RELEASE	JARO_VERSION "." JARO_VERSION_RELEASE
#define JARO_COPYRIGHT	JARO_RELEASE " Copyright (C) 2016-2021, https://github.com/jarolang/jaro. All rights reserved."
#define JARO_AUTHORS	" Vittorio Cieri"


/* mark for precompiled code ('<esc>Jaro') */
#define JARO_SIGNATURE	"\033Jaro"

/* option for multiple returns in 'jaro_pcall' and 'jaro_call' */
#define JARO_MULTRET	(-1)


/*
** pseudo-indices
*/
#define JARO_REGISTRYINDEX	JAROI_FIRSTPSEUDOIDX
#define jaro_upvalueindex(i)	(JARO_REGISTRYINDEX - (i))


/* thread status */
#define JARO_OK		0
#define JARO_YIELD	1
#define JARO_ERRRUN	2
#define JARO_ERRSYNTAX	3
#define JARO_ERRMEM	4
#define JARO_ERRGCMM	5
#define JARO_ERRERR	6


typedef struct jaro_State jaro_State;

typedef int (*jaro_CFunction) (jaro_State *L);


/*
** functions that read/write blocks when loading/dumping Jaro chunks
*/
typedef const char * (*jaro_Reader) (jaro_State *L, void *ud, size_t *sz);

typedef int (*jaro_Writer) (jaro_State *L, const void* p, size_t sz, void* ud);


/*
** prototype for memory-allocation functions
*/
typedef void * (*jaro_Alloc) (void *ud, void *ptr, size_t osize, size_t nsize);


/*
** basic types
*/
#define JARO_TNONE		(-1)

#define JARO_TNIL		0
#define JARO_TBOOLEAN		1
#define JARO_TLIGHTUSERDATA	2
#define JARO_TNUMBER		3
#define JARO_TSTRING		4
#define JARO_TTABLE		5
#define JARO_TFUNCTION		6
#define JARO_TUSERDATA		7
#define JARO_TTHREAD		8

#define JARO_NUMTAGS		9



/* minimum Jaro stack available to a C function */
#define JARO_MINSTACK	20


/* predefined values in the registry */
#define JARO_RIDX_MAINTHREAD	1
#define JARO_RIDX_GLOBALS	2
#define JARO_RIDX_LAST		JARO_RIDX_GLOBALS


/* type of numbers in Jaro */
typedef JARO_NUMBER jaro_Number;


/* type for integer functions */
typedef JARO_INTEGER jaro_Integer;

/* unsigned integer type */
typedef JARO_UNSIGNED jaro_Unsigned;



/*
** generic extra include file
*/
#if defined(JARO_USER_H)
#include JARO_USER_H
#endif



/*
** state manipulation
*/
JARO_API jaro_State *(jaro_newstate) (jaro_Alloc f, void *ud);
JARO_API void       (jaro_close) (jaro_State *L);
JARO_API jaro_State *(jaro_newthread) (jaro_State *L);

JARO_API jaro_CFunction (jaro_atpanic) (jaro_State *L, jaro_CFunction panicf);


JARO_API const jaro_Number *(jaro_version) (jaro_State *L);


/*
** basic stack manipulation
*/
JARO_API int   (jaro_absindex) (jaro_State *L, int idx);
JARO_API int   (jaro_gettop) (jaro_State *L);
JARO_API void  (jaro_settop) (jaro_State *L, int idx);
JARO_API void  (jaro_pushvalue) (jaro_State *L, int idx);
JARO_API void  (jaro_remove) (jaro_State *L, int idx);
JARO_API void  (jaro_insert) (jaro_State *L, int idx);
JARO_API void  (jaro_replace) (jaro_State *L, int idx);
JARO_API void  (jaro_copy) (jaro_State *L, int fromidx, int toidx);
JARO_API int   (jaro_checkstack) (jaro_State *L, int sz);

JARO_API void  (jaro_xmove) (jaro_State *from, jaro_State *to, int n);


/*
** access functions (stack -> C)
*/

JARO_API int             (jaro_isnumber) (jaro_State *L, int idx);
JARO_API int             (jaro_isstring) (jaro_State *L, int idx);
JARO_API int             (jaro_iscfunction) (jaro_State *L, int idx);
JARO_API int             (jaro_isuserdata) (jaro_State *L, int idx);
JARO_API int             (jaro_type) (jaro_State *L, int idx);
JARO_API const char     *(jaro_typename) (jaro_State *L, int tp);

JARO_API jaro_Number      (jaro_tonumberx) (jaro_State *L, int idx, int *isnum);
JARO_API jaro_Integer     (jaro_tointegerx) (jaro_State *L, int idx, int *isnum);
JARO_API jaro_Unsigned    (jaro_tounsignedx) (jaro_State *L, int idx, int *isnum);
JARO_API int             (jaro_toboolean) (jaro_State *L, int idx);
JARO_API const char     *(jaro_tolstring) (jaro_State *L, int idx, size_t *len);
JARO_API size_t          (jaro_rawlen) (jaro_State *L, int idx);
JARO_API jaro_CFunction   (jaro_tocfunction) (jaro_State *L, int idx);
JARO_API void	       *(jaro_touserdata) (jaro_State *L, int idx);
JARO_API jaro_State      *(jaro_tothread) (jaro_State *L, int idx);
JARO_API const void     *(jaro_topointer) (jaro_State *L, int idx);


/*
** Comparison and arithmetic functions
*/

#define JARO_OPADD	0	/* ORDER TM */
#define JARO_OPSUB	1
#define JARO_OPMUL	2
#define JARO_OPDIV	3
#define JARO_OPMOD	4
#define JARO_OPPOW	5
#define JARO_OPUNM	6

JARO_API void  (jaro_arith) (jaro_State *L, int op);

#define JARO_OPEQ	0
#define JARO_OPLT	1
#define JARO_OPLE	2

JARO_API int   (jaro_rawequal) (jaro_State *L, int idx1, int idx2);
JARO_API int   (jaro_compare) (jaro_State *L, int idx1, int idx2, int op);


/*
** push functions (C -> stack)
*/
JARO_API void        (jaro_pushnil) (jaro_State *L);
JARO_API void        (jaro_pushnumber) (jaro_State *L, jaro_Number n);
JARO_API void        (jaro_pushinteger) (jaro_State *L, jaro_Integer n);
JARO_API void        (jaro_pushunsigned) (jaro_State *L, jaro_Unsigned n);
JARO_API const char *(jaro_pushlstring) (jaro_State *L, const char *s, size_t l);
JARO_API const char *(jaro_pushstring) (jaro_State *L, const char *s);
JARO_API const char *(jaro_pushvfstring) (jaro_State *L, const char *fmt,
                                                      va_list argp);
JARO_API const char *(jaro_pushfstring) (jaro_State *L, const char *fmt, ...);
JARO_API void  (jaro_pushcclosure) (jaro_State *L, jaro_CFunction fn, int n);
JARO_API void  (jaro_pushboolean) (jaro_State *L, int b);
JARO_API void  (jaro_pushlightuserdata) (jaro_State *L, void *p);
JARO_API int   (jaro_pushthread) (jaro_State *L);


/*
** get functions (Jaro -> stack)
*/
JARO_API void  (jaro_getglobal) (jaro_State *L, const char *var);
JARO_API void  (jaro_gettable) (jaro_State *L, int idx);
JARO_API void  (jaro_getfield) (jaro_State *L, int idx, const char *k);
JARO_API void  (jaro_rawget) (jaro_State *L, int idx);
JARO_API void  (jaro_rawgeti) (jaro_State *L, int idx, int n);
JARO_API void  (jaro_rawgetp) (jaro_State *L, int idx, const void *p);
JARO_API void  (jaro_createtable) (jaro_State *L, int narr, int nrec);
JARO_API void *(jaro_newuserdata) (jaro_State *L, size_t sz);
JARO_API int   (jaro_getmetatable) (jaro_State *L, int objindex);
JARO_API void  (jaro_getuservalue) (jaro_State *L, int idx);


/*
** set functions (stack -> Jaro)
*/
JARO_API void  (jaro_setglobal) (jaro_State *L, const char *var);
JARO_API void  (jaro_settable) (jaro_State *L, int idx);
JARO_API void  (jaro_setfield) (jaro_State *L, int idx, const char *k);
JARO_API void  (jaro_rawset) (jaro_State *L, int idx);
JARO_API void  (jaro_rawseti) (jaro_State *L, int idx, int n);
JARO_API void  (jaro_rawsetp) (jaro_State *L, int idx, const void *p);
JARO_API int   (jaro_setmetatable) (jaro_State *L, int objindex);
JARO_API void  (jaro_setuservalue) (jaro_State *L, int idx);


/*
** 'load' and 'call' functions (load and run Jaro code)
*/
JARO_API void  (jaro_callk) (jaro_State *L, int nargs, int nresults, int ctx,
                           jaro_CFunction k);
#define jaro_call(L,n,r)		jaro_callk(L, (n), (r), 0, NULL)

JARO_API int   (jaro_getctx) (jaro_State *L, int *ctx);

JARO_API int   (jaro_pcallk) (jaro_State *L, int nargs, int nresults, int errfunc,
                            int ctx, jaro_CFunction k);
#define jaro_pcall(L,n,r,f)	jaro_pcallk(L, (n), (r), (f), 0, NULL)

JARO_API int   (jaro_load) (jaro_State *L, jaro_Reader reader, void *dt,
                                        const char *chunkname,
                                        const char *mode);

JARO_API int (jaro_dump) (jaro_State *L, jaro_Writer writer, void *data);


/*
** coroutine functions
*/
JARO_API int  (jaro_yieldk) (jaro_State *L, int nresults, int ctx,
                           jaro_CFunction k);
#define jaro_yield(L,n)		jaro_yieldk(L, (n), 0, NULL)
JARO_API int  (jaro_resume) (jaro_State *L, jaro_State *from, int narg);
JARO_API int  (jaro_status) (jaro_State *L);

/*
** garbage-collection function and options
*/

#define JARO_GCSTOP		0
#define JARO_GCRESTART		1
#define JARO_GCCOLLECT		2
#define JARO_GCCOUNT		3
#define JARO_GCCOUNTB		4
#define JARO_GCSTEP		5
#define JARO_GCSETPAUSE		6
#define JARO_GCSETSTEPMUL	7
define JARO_GCSETMAJORINC	8
#define JARO_GCISRUNNING	9
#define JARO_GCGEN		10
#define JARO_GCINC		11

JARO_API int (jaro_gc) (jaro_State *L, int what, int data);


/*
** miscellaneous functions
*/

JARO_API int   (jaro_error) (jaro_State *L);

JARO_API int   (jaro_next) (jaro_State *L, int idx);

JARO_API void  (jaro_concat) (jaro_State *L, int n);
JARO_API void  (jaro_len)    (jaro_State *L, int idx);

JARO_API jaro_Alloc (jaro_getallocf) (jaro_State *L, void **ud);
JARO_API void      (jaro_setallocf) (jaro_State *L, jaro_Alloc f, void *ud);



/*
** ===============================================================
** some useful macros
** ===============================================================
*/

#define jaro_tonumber(L,i)	jaro_tonumberx(L,i,NULL)
#define jaro_tointeger(L,i)	jaro_tointegerx(L,i,NULL)
#define jaro_tounsigned(L,i)	jaro_tounsignedx(L,i,NULL)

#define jaro_pop(L,n)		jaro_settop(L, -(n)-1)

#define jaro_newtable(L)		jaro_createtable(L, 0, 0)

#define jaro_register(L,n,f) (jaro_pushcfunction(L, (f)), jaro_setglobal(L, (n)))

#define jaro_pushcfunction(L,f)	jaro_pushcclosure(L, (f), 0)

#define jaro_isfunction(L,n)	(jaro_type(L, (n)) == JARO_TFUNCTION)
#define jaro_istable(L,n)	(jaro_type(L, (n)) == JARO_TTABLE)
#define jaro_islightuserdata(L,n)	(jaro_type(L, (n)) == JARO_TLIGHTUSERDATA)
#define jaro_isnil(L,n)		(jaro_type(L, (n)) == JARO_TNIL)
#define jaro_isboolean(L,n)	(jaro_type(L, (n)) == JARO_TBOOLEAN)
#define jaro_isthread(L,n)	(jaro_type(L, (n)) == JARO_TTHREAD)
#define jaro_isnone(L,n)		(jaro_type(L, (n)) == JARO_TNONE)
#define jaro_isnoneornil(L, n)	(jaro_type(L, (n)) <= 0)

#define jaro_pushliteral(L, s)	\
	jaro_pushlstring(L, "" s, (sizeof(s)/sizeof(char))-1)

#define jaro_pushglobaltable(L)  \
	jaro_rawgeti(L, JARO_REGISTRYINDEX, JARO_RIDX_GLOBALS)

#define jaro_tostring(L,i)	jaro_tolstring(L, (i), NULL)



/*
** {======================================================================
** Debug API
** =======================================================================
*/


/*
** Event codes
*/
#define JARO_HOOKCALL	0
#define JARO_HOOKRET	1
#define JARO_HOOKLINE	2
#define JARO_HOOKCOUNT	3
#define JARO_HOOKTAILCALL 4


/*
** Event masks
*/
#define JARO_MASKCALL	(1 << JARO_HOOKCALL)
#define JARO_MASKRET	(1 << JARO_HOOKRET)
#define JARO_MASKLINE	(1 << JARO_HOOKLINE)
#define JARO_MASKCOUNT	(1 << JARO_HOOKCOUNT)

typedef struct jaro_Debug jaro_Debug;  /* activation record */


/* Functions to be called by the debugger in specific events */
typedef void (*jaro_Hook) (jaro_State *L, jaro_Debug *ar);


JARO_API int (jaro_getstack) (jaro_State *L, int level, jaro_Debug *ar);
JARO_API int (jaro_getinfo) (jaro_State *L, const char *what, jaro_Debug *ar);
JARO_API const char *(jaro_getlocal) (jaro_State *L, const jaro_Debug *ar, int n);
JARO_API const char *(jaro_setlocal) (jaro_State *L, const jaro_Debug *ar, int n);
JARO_API const char *(jaro_getupvalue) (jaro_State *L, int funcindex, int n);
JARO_API const char *(jaro_setupvalue) (jaro_State *L, int funcindex, int n);

JARO_API void *(jaro_upvalueid) (jaro_State *L, int fidx, int n);
JARO_API void  (jaro_upvaluejoin) (jaro_State *L, int fidx1, int n1,
                                               int fidx2, int n2);

JARO_API int (jaro_sethook) (jaro_State *L, jaro_Hook func, int mask, int count);
JARO_API jaro_Hook (jaro_gethook) (jaro_State *L);
JARO_API int (jaro_gethookmask) (jaro_State *L);
JARO_API int (jaro_gethookcount) (jaro_State *L);


struct jaro_Debug {
  int event;
  const char *name;	/* (n) */
  const char *namewhat;	/* (n) 'global', 'local', 'field', 'method' */
  const char *what;	/* (S) 'Jaro', 'C', 'main', 'tail' */
  const char *source;	/* (S) */
  int currentline;	/* (l) */
  int linedefined;	/* (S) */
  int lastlinedefined;	/* (S) */
  unsigned char nups;	/* (u) number of upvalues */
  unsigned char nparams;/* (u) number of parameters */
  char isvararg;        /* (u) */
  char istailcall;	/* (t) */
  char short_src[JARO_IDSIZE]; /* (S) */
  /* private part */
  struct CallInfo *i_ci;  /* active function */
};

/* }====================================================================== */


/******************************************************************************
* Copyright (C) 2016-2021, https://github.com/jarolang/jaro, Vittorio Cieri.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
******************************************************************************/


#endif
