/* Lua CUTIL - a util library for Lua
 *
 * Copyright (c) 2016  chenweiqi
 *
 * The MIT License (MIT)
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
 */

#include <stdlib.h>
#include <string.h>
#include <lua.h>
#include <lauxlib.h>
#include <time.h>

#define CUTIL_BUF_SIZE 3097

typedef struct {
	int buf_sz;
	char* buf;
} cutil_conf_t;

#define uchar(c) ((unsigned char)(c))

#define isdigit(s) (s>=48 && s<=57)
#define isalpha(s) ((s>=65 && s<=90) || (s>=97 && s<=122))


static cutil_conf_t* cutil_fetch_info(lua_State *L)
{
	cutil_conf_t* cfg;
	cfg = lua_touserdata(L, lua_upvalueindex(1));
	if (!cfg)
		luaL_error(L, "Unable to fetch CUTIL cfg");

	return cfg;
}


/* filter special characters, only keep Chinese characters, English letters and numbers.
 * (support for utf8)
 */
static int filter_spec_chars(lua_State *L)
{
	size_t srcl;
	int i;
	int l = 0;
	luaL_Buffer b;
	char *p;
	char* tmp;
	int use_buf = NULL;
	const char* src = luaL_checklstring(L, 1, &srcl);
	cutil_conf_t * cfg = cutil_fetch_info(L);

	if (srcl < cfg->buf_sz) {
		tmp = cfg->buf;
		use_buf = !NULL;
	} else {
		tmp = (char *)malloc(sizeof(char) * srcl);
	}
	if (!tmp) {
		luaL_error(L, "Out of memory");
		return 0;
	}

	for ( i=0; i<srcl; i++) {
		unsigned char s = uchar(src[i]);
		if (isdigit(s) || isalpha(s)) {
			tmp[l++] = s;
			continue;
		}

		if (i<srcl-2) {
			if (s>=228 && s<=233) {
				unsigned char s1 = uchar(src[i+1]);
				unsigned char s2 = uchar(src[i+2]); 
				int a1=128, a2=191, a3=128, a4=191;
				if (s == 228) {
					a1 = 184;
				} else if (s == 233) {
					a2 = 190;
					a4 = 165;
				}
				if (s1>=a1 && s1<=a2 && s2>=a3 && s2<=a4){
					tmp[l++] = s;
					tmp[l++] = s1;
					tmp[l++] = s2;
					i += 2;
				}
			}
		}
	}

	p = luaL_buffinitsize(L, &b, l);
	memcpy(p, tmp, l * sizeof(char));
	if (!use_buf)
		free(tmp);
	luaL_pushresultsize(&b, l);
	return 1;
}


/* GC, clean up the buf */
static int cutil_gc(lua_State *L)
{
	cutil_conf_t *cfg;
	cfg = lua_touserdata(L, 1);
	if (cfg && cfg->buf)
		free(cfg->buf);

	cfg = NULL;
	return 0;
}

static void cutil_create_config(lua_State *L)
{
	cutil_conf_t *cfg;
	cfg = lua_newuserdata(L, sizeof(*cfg));
	/* Create GC method to clean up buf */
	lua_newtable(L);
	lua_pushcfunction(L, cutil_gc);
	lua_setfield(L, -2, "__gc");
	lua_setmetatable(L, -2);

	cfg->buf = (char *)malloc(sizeof(char) * CUTIL_BUF_SIZE);
	if (!cfg->buf) {
		luaL_error(L, "Unable to create CUTIL cfg");
		return;
	}
	cfg->buf_sz = CUTIL_BUF_SIZE;
}


int luaopen_cutil(lua_State *L)
{
	static const luaL_Reg funcs[] = {
		{"filter_spec_chars", filter_spec_chars},
		{NULL, NULL}
	};

	lua_newtable(L);
	cutil_create_config(L);
	luaL_setfuncs(L, funcs, 1);

	return 1;
}
