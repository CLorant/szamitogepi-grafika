CC = gcc
SRC_DIR = src
CFLAGS = -Wall -Wextra -I$(SRC_DIR)
LDFLAGS = -lm -lmingw32 -lSDL2main -lSDL2

SDL_DIR = $(SRC_DIR)\sdl
SHAPES_DIR = $(SRC_DIR)\shapes
STRUCTS_DIR = $(SRC_DIR)\structs

OBJS = $(SRC_DIR)\main.o \
       $(SDL_DIR)\drawing.o \
       $(SDL_DIR)\event_handler.o \
       $(SDL_DIR)\palette.o \
       $(SDL_DIR)\sdl_utils.o \
       $(SHAPES_DIR)\line.o \
       $(SHAPES_DIR)\rectangle.o \
       $(SHAPES_DIR)\circle.o

HEADERS = $(SDL_DIR)\consts.h \
          $(SDL_DIR)\drawing.h \
          $(SDL_DIR)\event_handler.h \
          $(SDL_DIR)\palette.h \
          $(SDL_DIR)\sdl_utils.h \
          $(SHAPES_DIR)\line.h \
          $(SHAPES_DIR)\rectangle.h \
          $(SHAPES_DIR)\circle.h \
          $(STRUCTS_DIR)\point.h \
          $(STRUCTS_DIR)\color.h \
          $(STRUCTS_DIR)\line_struct.h \
          $(STRUCTS_DIR)\rectangle_struct.h \
          $(STRUCTS_DIR)\circle_struct.h \
          $(STRUCTS_DIR)\draw_mode.h \
          $(STRUCTS_DIR)\draw_state.h

all: shapes.exe

shapes.exe: $(OBJS)
	$(CC) $(OBJS) -o shapes.exe $(LDFLAGS)

$(SRC_DIR)\%.o: $(SRC_DIR)\%.c $(HEADERS)
	$(CC) -c $< -o $@ $(CFLAGS)

$(SDL_DIR)\%.o: $(SDL_DIR)\%.c $(HEADERS)
	$(CC) -c $< -o $@ $(CFLAGS)

$(SHAPES_DIR)\%.o: $(SHAPES_DIR)\%.c $(HEADERS)
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	-del /f /q $(OBJS) shapes.exe