#include "cuboid.h"

#include <math.h>

void set_size(Cuboid* cuboid) {
    printf("Adja meg a téglatest hosszát: ");
    while (scanf("%f", &cuboid->length) != 1 || cuboid->length <= 0) {
        printf("Hiba: Érvényes, pozitív számot adjon meg a hosszhoz!\n");
        while(getchar() != '\n');
        printf("Adja meg a téglatest hosszát: ");
    }

    printf("Adja meg a téglatest szélességét: ");
    while (scanf("%f", &cuboid->width) != 1 || cuboid->width <= 0) {
        printf("Hiba: Érvényes, pozitív számot adjon meg a szélességhez!\n");
        while(getchar() != '\n');
        printf("Adja meg a téglatest szélességét: ");
    }

    printf("Adja meg a téglatest magasságát: ");
    while (scanf("%f", &cuboid->height) != 1 || cuboid->height <= 0) {
        printf("Hiba: Érvényes, pozitív számot adjon meg a magassághoz!\n");
        while(getchar() != '\n');
        printf("Adja meg a téglatest magasságát: ");
    }
}

float calc_volume(Cuboid cuboid) {
    return cuboid.length * cuboid.width * cuboid.height;
}

float calc_surface(Cuboid cuboid) {
    return 2 * (cuboid.length * cuboid.width + cuboid.width * cuboid.height + cuboid.height * cuboid.length);
}

int has_square_face(Cuboid cuboid) {
    if (cuboid.length == cuboid.width || cuboid.width == cuboid.height || cuboid.height == cuboid.length) {
        return 1;
    } else {
        return 0;
    }
}