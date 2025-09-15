CC = gcc
CFLAGS = -Wall -Wextra -O2 -Iinclude

SRCDIR = src
BUILDDIR = build
TARGET = $(BUILDDIR)/program

SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(SRC))

.PHONY: all clean rebuild

all: $(TARGET)

$(TARGET): $(OBJ) | $(BUILDDIR)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

clean:
	rm -rf $(BUILDDIR)

rebuild: clean all
