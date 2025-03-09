#ifndef CUBOID_H
#define CUBOID_H

/**
 * @brief Téglatest struktúra
 */
typedef struct Cuboid {
    float length;
    float width;
    float height;
} Cuboid;

/**
 * @brief Pozitív lebegőpontos szám bekérés
 */
void get_positive_float(const char *prompt, float *value);

/**
 * @brief A téglatest 3 élének beállítása
 */
void set_size(Cuboid* cuboid);

/**
 * @brief A téglatest térfogatának kiszámítása
 */
float calc_volume(Cuboid cuboid);

/**
 * @brief A téglatest felületének kiszámítása
 */
float calc_surface(Cuboid cuboid);

/**
 * @brief A téglatestnek van-e négyzet alakú lapja
 */
int has_square_face(Cuboid cuboid);

#endif