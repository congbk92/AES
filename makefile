TARGET = main

CC       = g++
CFLAGS   = -g -Wall

LDFLAGS  = 

INCLUDES = -I./include


SRCDIR = src
OBJDIR = obj
BINDIR = bin

SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:src/%.cpp=obj/%.o)

all: $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

$(OBJS): obj/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<	

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

.PHONY: clean
clean:
	rm -rf obj/* 
	rm -f $(BINDIR)/$(TARGET)

