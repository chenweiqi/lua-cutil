##### Available defines for CUTIL_CFLAGS #####
##
## AUTHOR: chenweiqi [2016-4-13]
## https://github.com/chenweiqi/lua-cutil
##

##### Build defaults #####
LUA_VERSION =       5.3
TARGET =            cutil.so
PREFIX =            /usr/local
#CFLAGS =            -g -Wall -pedantic -fno-inline
CFLAGS =            -O3 -Wall -pedantic -DNDEBUG
CUTIL_CFLAGS =      -fpic
CUTIL_LDFLAGS =     -shared
LUA_INCLUDE_DIR =   $(PREFIX)/include
LUA_CMODULE_DIR =   $(PREFIX)/lib/lua/$(LUA_VERSION)
LUA_MODULE_DIR =    $(PREFIX)/share/lua/$(LUA_VERSION)
LUA_BIN_DIR =       $(PREFIX)/bin

##### Platform overrides #####
##
## Tweak one of the platform sections below to suit your situation.
##
## See http://lua-users.org/wiki/BuildingModules for further platform
## specific details.

## Linux

## FreeBSD
#LUA_INCLUDE_DIR =   $(PREFIX)/include/lua51

## MacOSX (Macports)
#PREFIX =            /opt/local
#CUTIL_LDFLAGS =     -bundle -undefined dynamic_lookup

## Solaris
#CC           =      gcc
#CUTIL_CFLAGS =      -fpic -DUSE_INTERNAL_ISINF

## Windows (MinGW)
#TARGET =            cutil.dll
#PREFIX =            /home/user/opt
#CUTIL_CFLAGS =      -DDISABLE_INVALID_NUMBERS
#CUTIL_LDFLAGS =     -shared -L$(PREFIX)/lib -llua53
#LUA_BIN_SUFFIX =    .lua

##### End customisable sections #####

EXECPERM =          755

BUILD_CFLAGS =      -I$(LUA_INCLUDE_DIR) $(CUTIL_CFLAGS)
OBJS =              lua_cutil.o

.PHONY: all clean install

.c.o:
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(BUILD_CFLAGS) -o $@ $<

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(CUTIL_LDFLAGS) -o $@ $(OBJS)

install: $(TARGET)
	mkdir -p $(DESTDIR)$(LUA_CMODULE_DIR)
	cp $(TARGET) $(DESTDIR)$(LUA_CMODULE_DIR)
	chmod $(EXECPERM) $(DESTDIR)$(LUA_CMODULE_DIR)/$(TARGET)

clean:
	rm -f *.o $(TARGET)
