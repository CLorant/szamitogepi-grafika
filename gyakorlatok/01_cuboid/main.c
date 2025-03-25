#include "cuboid.h"

#include <stdio.h>

int main(int argc, char* argv[]) {
    printf("\nCuboid\n\n");

    Cuboid cuboid;

    set_size(&cuboid);

    float volume = calc_volume(cuboid);
    printf("Volume: %f cm³\n", volume);

    float surface = calc_surface(cuboid);
    printf("Surface Area: %f cm²\n", surface);

    int isSquareFace = has_square_face(cuboid);
    printf(isSquareFace ? "Contains square face" : "Doesn't contain square face");

    return 0;
}
