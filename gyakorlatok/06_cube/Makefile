all:
	gcc -Iinclude/ src/app.c src/camera.c src/main.c src/scene.c src/texture.c src/utils.c -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lobj -lopengl32 -lglu32 -lm -o models.exe -Wall -Wextra -Wpedantic

linux:
	gcc -Iinclude/ src/app.c src/camera.c src/main.c src/scene.c src/texture.c src/utils.c -lobj -lSDL2 -lglu32 -lSDL2_image -lGL -lm -o models -Wall -Wextra -Wpedantic
