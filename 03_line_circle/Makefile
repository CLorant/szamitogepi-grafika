CC = gcc
CFLAGS = -Wall -Wextra -lm -lmingw32 -lSDL2main -lSDL2

SRC_DIR = src
SHAPES_DIR = $(SRC_DIR)/shapes
STRUCTS_DIR = $(SRC_DIR)/structs

OBJS = $(SRC_DIR)/main.o \
       $(SRC_DIR)/palette.o \
       $(SHAPES_DIR)/line.o \
       $(SHAPES_DIR)/rectangle.o 

HEADERS = $(SRC_DIR)/palette.h \
          $(SHAPES_DIR)/line.h \
          $(SHAPES_DIR)/rectangle.h \
          $(STRUCTS_DIR)/point.h \
          $(STRUCTS_DIR)/color.h \
          $(STRUCTS_DIR)/line_struct.h \
          $(STRUCTS_DIR)/rectangle_struct.h 

all: shapes.exe

shapes.exe: $(OBJS)
	$(CC) $(OBJS) -o shapes.exe $(CFLAGS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	$(CC) -c $< -o $@ $(CFLAGS)

$(SHAPES_DIR)/%.o: $(SHAPES_DIR)/%.c $(HEADERS)
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f $(OBJS) shapes.exe
