CC=g++ -g
CFLAGS=-std=c++11 -c
LDFLAGS=-L./lua -llua -lcurl -DLUA_USER_H=\"lua/lua_user.h\" -ldl -lmysqlclient -lpthread
INCLUDES=-L/usr/local/lib -Irjson/include/
SOURCES=	\
	core/main.cpp \
	core/net.cpp \
	core/console.cpp \
	core/vk/longpoll.cpp \
	core/vk/vkapi.cpp \
	core/cmd.cpp \
	core/cvar.cpp \
	core/lua/luawork.cpp \
	core/lua/lJson.cpp \
	core/lua/lApi.cpp \
	core/lua/lApiFunctions.cpp \
	core/strutils.cpp \
	core/lua/luaModules.cpp \
	core/filesystem.cpp \
	core/mysql_work.cpp \
	core/lua/luapool.cpp \
	core/eapi/eapi.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=EBP

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $(EXECUTABLE)

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@
clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)
