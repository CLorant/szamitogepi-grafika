## 1. gyakorlati feladat: Téglatestek

Téglatest struktúra
```c
typedef struct Cuboid {
    float length;
    float width;
    float height;
} Cuboid;
```
Pozitív lebegőpontos szám bekérés
```c
void get_positive_float(const char *prompt, float *value);
```
A téglatest 3 élének beállítása
```c
void set_size(Cuboid* cuboid);
```
A téglatest térfogatának kiszámítása
```c
float calc_volume(Cuboid cuboid);
```
A téglatest felületének kiszámítása
```c
float calc_surface(Cuboid cuboid);
```
A téglatestnek van-e négyzet alakú lapja
```c
int has_square_face(Cuboid cuboid);
```
