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

$(SRCDIR)/scanner.c: $(SRCDIR)/scanner.flex 
	flex -o src/scanner.c src/scanner.flex

test_encoder: dirs $(TARGET)
	$(SHELL) runtest.sh encode

test: dirs $(TARGET)
	$(SHELL) runtest.sh encode
	$(SHELL) runtest.sh scanner

$(TARGET): $(OBJSDIR)/hash_table.o $(OBJSDIR)/library.o $(OBJSDIR)/encoder.o $(OBJSDIR)/scanner.o $(OBJSDIR)/bminor.o
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf $(TARGET) $(OBJSDIR) $(SRCDIR)/scanner.c

.PHONY: all clean test test_encoder