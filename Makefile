CC=g++
CFLAGS=-std=c++11 -c
LDFLAGS=-L./lua -llua -lcurl -DLUA_USER_H=\"lua/lua_user.h\" -ldl -lmysqlclient -lpthread
INCLUDES=-I./core -Irjson/include/
SOURCES=core/*.cpp core/vk/*.cpp core/lua/*.cpp core/lua/api/*.cpp core/eapi/*.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=EBP

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $(EXECUTABLE)

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@
clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)
