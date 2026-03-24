CC = g++
CPPFLAGS = -std=c++17 -O3 -w
LDFLAGS = -lstdc++ -lm -g -lssl -lcrypto -I/usr/include/openssl/ -L/usr/lib/ssl

MAKEFLAGS += -j8 --silent

SOURCES = $(shell find . -name "*.cpp")
OBJECTS = $(patsubst %.cpp, %.o, $(SOURCES))

%.o: %.cpp
	@$(CC) $(CPPFLAGS) -c $< -o $@

game: $(OBJECTS)
	@$(CC) $(OBJECTS) $(LDFLAGS) -o game

clean: 
	@rm -f $(OBJECTS) game

.PHONY: clean
