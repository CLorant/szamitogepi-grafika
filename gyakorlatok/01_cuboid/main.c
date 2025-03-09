#include "cuboid.h"

#include <stdio.h>

int main(int argc, char* argv[]) {
    printf("\n1. gyakorlat - Téglatest Program\n\n");

    Cuboid cuboid;

    set_size(&cuboid);

    float volume = calc_volume(cuboid);
    printf("Térfogat: %f cm³\n", volume);

    float surface = calc_surface(cuboid);
    printf("Felszín: %f cm²\n", surface);

    int isSquareFace = has_square_face(cuboid);
    printf(isSquareFace ? "Van négyzet alakú lap" : "Nincs négyzet alakú lap");

    return 0;
}
