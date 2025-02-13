#include "cuboid.h"

#include <stdio.h>


int main(int argc, char* argv[]) {
    Cuboid cuboid;

    set_size(cuboid);
    float volume = calc_volume(cuboid);
    printf("Térfogat: %f\n", volume);

    float surface = calc_surface(cuboid);
    printf("Felszín: %f\n", surface);

    int isSquareFace = has_square_face(cuboid);
    printf(isSquareFace ? "Van négyzet alakú lap" : "Nincs négyzet alakú lap");

    return 0;
}
