#include "cuboid.h"

#include <stdio.h>

#define PROMPT_LENGTH "Adja meg a téglatest hosszát (cm): "
#define PROMPT_WIDTH "Adja meg a téglatest szélességét (cm): "
#define PROMPT_HEIGHT "Adja meg a téglatest magasságát (cm): "
#define ERROR_MSG "Hiba: Érvényes, pozitív számot adjon meg!\n\n"

void get_positive_float(const char *prompt, float *value) {
    printf("%s", prompt);
    while (scanf("%f", value) != 1 || *value <= 0) {
        printf("%s", ERROR_MSG);
        while (getchar() != '\n');
        printf("%s", prompt);
    }
}

void set_size(Cuboid *c) {
    get_positive_float(PROMPT_LENGTH, &c->length);
    get_positive_float(PROMPT_WIDTH, &c->width);
    get_positive_float(PROMPT_HEIGHT, &c->height);
}

float calc_volume(Cuboid c) {
    return c.length * c.width * c.height;
}

float calc_surface(Cuboid c) {
    return 2 * (c.length * c.width + c.width * c.height + c.height * c.length);
}

int has_square_face(Cuboid c) {
    return c.length == c.width || c.width == c.height || c.height == c.length;
}