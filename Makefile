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

$(SRCDIR)/parser.c $(INCDIR)/parser.h: $(SRCDIR)/parser.bison 
	bison --defines=inc/parser.h --output=src/parser.c -v src/parser.bison

$(SRCDIR)/scanner.c: $(SRCDIR)/scanner.flex $(INCDIR)/parser.h
	flex -o src/scanner.c src/scanner.flex

test_encoder: dirs $(TARGET)
	$(SHELL) runtest.sh encode

test: dirs $(TARGET)
	$(SHELL) runtest.sh encode
	$(SHELL) runtest.sh scanner
	$(SHELL) runtest.sh parser

$(TARGET): $(OBJSDIR)/parser.o $(OBJSDIR)/scanner.o $(OBJSDIR)/encoder.o $(OBJSDIR)/bminor.o $(OBJSDIR)/hash_table.o $(OBJSDIR)/library.o  
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf $(TARGET) $(OBJSDIR) $(SRCDIR)/scanner.c $(SRCDIR)/parser.c $(SRCDIR)/parser.output $(INCDIR)/parser.h

.PHONY: all clean test test_encoder