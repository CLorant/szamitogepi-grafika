## 5. gyakorlati feladat: Origin program módosítása

### Projekt leírása
Ez a program Piller Imre "Origin" példa programjának egy módosított verziója, aminek a forráskódja: https://gitlab.com/imre-piller/me-courses/-/tree/master/grafika/examples/origin

### Módosítások
- Kamera függőleges mozgatása `q` és `e` billentyűkkel
- Kamera jobbra-balra mozgatása `j` és `l` billentyűkkel
- Színes háromszög elhelyezve
- Alacsony tesszalált szürke gömb elhelyezve
- Gömb forgatás implementálva
- Google Maps-hez hasonló kamera mozgatás (WIP)
- Sakktábla minta elhelyezve a koordináta rendszer alján
- Henger és kúp közelítések elhelyezve

### Futtatás
- `make` parancs használata fordításhoz
- `shapes.exe` vagy `./shapes` futtatáshoz

### Dokumentáció

**/src:**
- `main.c:` A program belépési pontja. Inicializálja a koordináta rendszert, és kezeli a fő ciklust.
- `app.c:` A program fő logikáját tartalmazza, beleértve az eseménykezelést, frissítést és renderelést.
- `camera.c:` A kamera beállításáért és kezelésért felelős.
- `scene.c:` A koordináta rendszer és alakzatok beállításáért és kezelésért felelős.
- `utils.c:` Egyéb helper függvényeket tartalmaz.