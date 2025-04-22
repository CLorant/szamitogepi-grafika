## 6. gyakorlati feladat: Cube program módosítása

### Projekt leírása
Ez a program Piller Imre "Cube" példa programjának egy módosított verziója, amelynek a forráskódja: https://gitlab.com/imre-piller/me-courses/-/tree/master/grafika/examples/cube

### Módosítások
- Orbitális kamera implementálva az origin program alapján
- 9 modell betöltve
- Bal egérgombbal kiválaszthatóak modellek (Ray AABB Intersection)
- A kiválasztott objektum körül megjelenik a bounding box, bal egérgomb lenyomva tartásával tetszőlegesen forgatható
- Objektum kiválasztás visszaállítása `T` billenytűvel.
- Objektum forgatásának visszaállítása `R` billenytűvel.
- Fényerő változtatása `PageUp` és `PageDown` billenytűkkel.
- Minden objektum modellje megfelelően transzformált inicializáskor
- A nem statikus objektumok idő függvényében forgatottak
- A globális anyagtulajdonság beállítva fémes hatású szürkére (fallback)
- Fény rendszer refaktorálva, hogy könnyebben bővíthető legyen több fényforrással
- Fényforrás diffuse színe animálva a `sine_animate_color` függvénnyel

### Szükséges fájlok
Az `/assets` mappába szükséges kicsomagolni az `assets.txt` linken elérhető `cube_assets.zip` fájlt.

### Fordítás és Futtatás
```bash
make && models.exe
```

### Dokumentáció

**/src:**
- `main.c:` A program belépési pontja, ami kezeli a fő ciklust.
- `app.c:` A program fő logikáját tartalmazza, beleértve az eseménykezelést, frissítést és renderelést.
- `camera.c:` A kamera beállításáért és kezelésért felelős.
- `scene.c:` A jelenet beállításáért és kezelésért felelős.
- `utils.c:` Az objektum transformáció rendszert és egyéb helper függvényeket tartalmaz.