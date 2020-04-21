CC = gcc

CFLAGS = -c $(shell pkg-config fuse --cflags)
LDFLAGS = 
LIBS = $(shell pkg-config fuse --libs)

BUILD := build/
SRC := src/
SRCS := $(wildcard $(SRC)*.c)
OBJS := $(patsubst $(SRC)%.c,$(BUILD)%.o,$(SRCS))

MAIN := blahajfs

all: $(MAIN)

$(BUILD)%.o: $(SRC)%.c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -o $@ $<

$(MAIN): $(OBJS)
	$(CC) $(LDFLAGS) -o $(MAIN) $(OBJS) $(LIBS)