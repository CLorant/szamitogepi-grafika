CC = gcc
SRC_DIR = src
INCLUDE_DIR = include
CFLAGS = -Wall -Wextra -I$(INCLUDE_DIR) -I$(INCLUDE_DIR)/core -I$(INCLUDE_DIR)/shapes -I$(INCLUDE_DIR)/structs
LDFLAGS = -lm -lmingw32 -lSDL2main -lSDL2

CORE_DIR = $(SRC_DIR)/core
SHAPES_DIR = $(SRC_DIR)/shapes

OBJS = $(SRC_DIR)/main.o \
       $(CORE_DIR)/drawing.o \
       $(CORE_DIR)/event_handler.o \
       $(CORE_DIR)/palette.o \
       $(CORE_DIR)/sdl_utils.o \
       $(SHAPES_DIR)/line.o \
       $(SHAPES_DIR)/rectangle.o \
       $(SHAPES_DIR)/circle.o

HEADERS = $(INCLUDE_DIR)/core/consts.h \
          $(INCLUDE_DIR)/core/drawing.h \
          $(INCLUDE_DIR)/core/event_handler.h \
          $(INCLUDE_DIR)/core/palette.h \
          $(INCLUDE_DIR)/core/sdl_utils.h \
          $(INCLUDE_DIR)/shapes/line.h \
          $(INCLUDE_DIR)/shapes/rectangle.h \
          $(INCLUDE_DIR)/shapes/circle.h \
          $(INCLUDE_DIR)/structs/point.h \
          $(INCLUDE_DIR)/structs/color.h \
          $(INCLUDE_DIR)/structs/line_struct.h \
          $(INCLUDE_DIR)/structs/rectangle_struct.h \
          $(INCLUDE_DIR)/structs/circle_struct.h \
          $(INCLUDE_DIR)/structs/draw_mode.h \
          $(INCLUDE_DIR)/structs/draw_state.h

all: shapes.exe

shapes.exe: $(OBJS)
	$(CC) $(OBJS) -o shapes.exe $(LDFLAGS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	$(CC) -c $< -o $@ $(CFLAGS)

$(CORE_DIR)/%.o: $(CORE_DIR)/%.c $(HEADERS)
	$(CC) -c $< -o $@ $(CFLAGS)

$(SHAPES_DIR)/%.o: $(SHAPES_DIR)/%.c $(HEADERS)
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	-del /f /q $(SRC_DIR)\main.o
	-del /f /q $(CORE_DIR)\*.o
	-del /f /q $(SHAPES_DIR)\*.o
	-del /f /q shapes.exe