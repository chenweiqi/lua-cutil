# lua-cutil
Lua CUtil is a util library for Lua

## Build
make sure lua was installed in you linux, and the following command 'make install' is just match lua 5.3, so lua 5.3 is suggested.
```plain
$ make all
$ make install
```

the command 'make install' is short for the following commands: 
```plain
$ cp cutil.so /usr/local/lib/lua/5.3
$ chmod 755 /usr/local/lib/lua/5.3/cutil.so
```



## Test
there is an example in the 'tests' dir.
```plain
$ /usr/local/bin/lua
Lua 5.3.2  Copyright (C) 1994-2015 Lua.org, PUC-Rio
> dofile("tests/sample.lua")
lua version:    ssfoo明天a你好foo23333
cutil version:  ssfoo明天a你好foo23333
efficiency comparison: 
test lua version:       5.53
test cutil version:     0.06
```
