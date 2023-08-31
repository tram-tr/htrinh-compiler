CC = gcc
CFLAGS = -std=gnu99 -g
SHELL = /bin/bash
SRCDIR = src
INCDIR = inc 
OBJSDIR = objs
TESTSDIR = tests
TARGET = bminor

all: dirs $(TARGET)

dirs:
	mkdir -p $(OBJSDIR)

$(OBJSDIR)/%.o: $(SRCDIR)/%.c 
	$(CC) $(CFLAGS) -c $^ -o $@

test_encoder: dirs $(TARGET)
	$(SHELL) runtest.sh encoder

$(TARGET): $(OBJSDIR)/hash_table.o $(OBJSDIR)/library.o $(OBJSDIR)/encoder.o $(OBJSDIR)/bminor.o
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf $(TARGET) $(OBJSDIR)

.PHONY: all clean test_encoder