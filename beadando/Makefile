CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -Wpedantic -g -O3
LDFLAGS =
SRC = src
BUILD = build

SRC_FILES := $(wildcard $(SRC)/*.c)
OBJ_FILES := $(patsubst $(SRC)/%.c, $(BUILD)/%.o, $(SRC_FILES))

TARGET_WINDOWS = lopas.exe
TARGET_LINUX = lopas

ifeq ($(OS), Windows_NT)
    TARGET = $(TARGET_WINDOWS)
    LIBS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lobj -lopengl32 -lglu32 -lode_single -ljson-c -lm
    MKDIR = if not exist $(BUILD) mkdir $(BUILD)
    RM = del /f /q
    RMDIR = rmdir /s /q
else
    TARGET = $(TARGET_LINUX)
    LIBS = -lobj -lSDL2 -lSDL2_image -lGL -lGLU -lode_single -ljson-c -lm
    MKDIR = mkdir -p $(BUILD)
    RM = rm -f
    RMDIR = rm -rf
endif

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -o $@ $(LDFLAGS) $(LIBS)

$(BUILD)/%.o: $(SRC)/%.c
	@$(MKDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
ifeq ($(OS), Windows_NT)
	-$(RM) $(BUILD)\*.o
	-$(RM) $(TARGET_WINDOWS)
	-$(RMDIR) $(BUILD)
else
	$(RM) $(BUILD)/*.o
	$(RM) $(TARGET_LINUX)
	$(RMDIR) $(BUILD)
endif

.PHONY: all clean
