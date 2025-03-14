## 4. gyakorlati feladat: Pong program módosítása

### Projekt leírása
Ez a program Piller Imre "Pong" példa programjának egy módosított verziója, aminek a forráskódja: https://gitlab.com/imre-piller/me-courses/-/tree/master/grafika/examples/pong

### Főbb funkciók
- Két játékos mód: A játékot két játékos játszhatja, az egyik a bal oldali, a másik a jobb oldali ütőt irányítja.
- Billentyűzet és egér vezérlés: A játékosok billentyűzettel és egérrel irányíthatják az ütőket.

### Módosítások
- Növelt labda és bal ütő sebesség.
- Labda visszapattintása csak akkor, ha az ütő mögötte van.
- Kattintással áthelyezhető a labda
- ```1``` és ```2``` billenyűkkel csökkenthető / növelhető a labda mérete
- Labda forgatás implementációja
- Pontszámok megjelenítése

**Megjegyzés:** A pontszámok megjelenítése nem használ külön könyvtárt (viszont az SDL2 és GL könyvtárak előfeltételek a program futtatásához)

### Futtatás
- ```make``` parancs használata fordításhoz
- ```shapes.exe``` vagy ```./shapes``` futtatáshoz

**Megjegyzés:** Ezek előtt szükséges leklónozni a repository-t és belépni a 04_pong mappába.

### Dokumentáció

**/src:**
- ```main.c:``` A program belépési pontja. Inicializálja a játékot, és kezeli a fő ciklust.
- ```game.c:``` A játék fő logikáját tartalmazza, beleértve az eseménykezelést, frissítést és renderelést.
- ```pong.c:``` A Pong játék magját implementálja, beleértve a labda és az ütők kezelését.
- ```ball.c:``` A labda viselkedését és mozgását kezeli.
- ```pad.c:``` Az ütők kezelését és megjelenítését implementálja.
- ```text.c:``` A szöveg megjelenítéséért felelős, beleértve a betűkészlet betöltését és renderelését.

**/assets:**
- ```textures/charmap.bmp:``` A betűkészletet tartalmazó textúrafájl, amely a szöveg megjelenítéséhez használt.