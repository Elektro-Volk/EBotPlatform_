CC=g++
CFLAGS=-std=c++11 -c
LDFLAGS=-L./lua -llua -lcurl -DLUA_USER_H=\"lua/lua_user.h\" -ldl -lmysqlclient -lpthread
INCLUDES=-I./core -Irjson/include/
SOURCES= \
	core/cmd.cpp \
	core/console.cpp \
	core/cvars.cpp \
	core/filesystem.cpp \
	core/main.cpp \
	core/net.cpp \
	core/strutils.cpp \
	core/vk/vkapi.cpp \
	core/vk/LongPollConnection.cpp \
	core/vk/GroupLongPollConnection.cpp \
	core/vk/longpoll.cpp \
	core/lua/luawork.cpp \
	core/lua/luapool.cpp \
	core/lua/luaModules.cpp \
	core/lua/lJson.cpp \
	core/lua/api/ln_vkapi.cpp \
	core/lua/api/ln_mysql.cpp \
	core/lua/api/ln_cmd.cpp \
	core/lua/api/ln_cvars.cpp \
	core/lua/api/ln_console.cpp \
	core/lua/api/ln_fs.cpp \
	core/lua/api/lu_json.cpp \
	core/lua/api/lu_string.cpp \
	core/lua/api/ln_net.cpp \
	core/lua/api/lu_other.cpp \
	core/lua/api/luaapi.cpp \
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
