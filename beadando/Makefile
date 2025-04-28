all:
	gcc -Iinclude/ src/app.c src/camera.c src/main.c src/physics.c src/scene.c src/texture.c src/utils.c src/config.c -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lobj -lopengl32 -lglu32 -lode-single -lm  -o models.exe -Wall -Wextra -Wpedantic

linux:
	gcc -Iinclude/ src/app.c src/camera.c src/main.c src/physics.c src/scene.c src/texture.c src/utils.c src/config.c -lobj -lSDL2 -lglu32 -lSDL2_image -lGL -lode-single -lm -o models -Wall -Wextra -Wpedantic
