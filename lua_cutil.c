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

#define uchar(c) ((unsigned char)(c))

#define isdigit(s) (s>=48 && s<=57)
#define isalpha(s) ((s>=65 && s<=90) || (s>=97 && s<=122))


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
	const char* src = luaL_checklstring(L, 1, &srcl);
	char* tmp = (char *)malloc(sizeof(char) * srcl);
	for ( i=0; i<srcl; i++) {
		unsigned char s = uchar(src[i]);
		if (isdigit(s) || isalpha(s)) {
			tmp[l++] = s;
			continue;
		}
		if (s>=228 && s<=233 && i<srcl-2) {
			unsigned char s1 = uchar(src[i+1]);
			unsigned char s2 = uchar(src[i+2]); 
			if (s1>=128 && s1<=191 && s2>=128 && s2<=191){
				tmp[l++] = s;
				tmp[l++] = s1;
				tmp[l++] = s2;
				i = i+2;
			}
		}
	}
	p = luaL_buffinitsize(L, &b, l);
	memcpy(p, tmp, l * sizeof(char));
	free(tmp);
	luaL_pushresultsize(&b, l);
	return 1;
}

int luaopen_cutil(lua_State *L)
{
	static const luaL_Reg funcs[] = {
		{"filter_spec_chars", filter_spec_chars},
		{NULL, NULL}
	};
	luaL_newlib(L, funcs);

	return 1;
}
