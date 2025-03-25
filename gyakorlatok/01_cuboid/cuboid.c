#include "cuboid.h"

#include <stdio.h>

void get_positive_float(const char *prompt, float *value) {
    printf("%s", prompt);

    while (scanf("%f", value) != 1 || *value <= 0) {
        perror("Error: Enter a valid, positive number!\n\n");
        while (getchar() != '\n');
        printf("%s", prompt);
    }
}

void set_size(Cuboid *c) {
    get_positive_float("Length (cm): ", &c->length);
    get_positive_float("Width (cm): ", &c->width);
    get_positive_float("Height (cm): ", &c->height);
}

float calc_volume(Cuboid c) {
    return c.length * c.width * c.height;
}

float calc_surface(Cuboid c) {
    return 2 * (c.length * c.width + c.width * c.height + c.height * c.length);
}

bool has_square_face(Cuboid c) {
    return c.length == c.width || c.width == c.height || c.height == c.length;
}