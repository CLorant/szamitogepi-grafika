## 3. gyakorlati feladat: Szakaszok rajzolása, Kör közelítése

### Projekt leírása
Ez a program egy egyszerű rajzolóeszköz, amely lehetővé teszi a felhasználó számára, hogy különböző alakzatokat (vonalak, téglalapok, körök) rajzoljon egy képernyőre. A program interaktív módon kezeli a felhasználói bemeneteket (egér, billentyűzet).

### Főbb funkciók
- Vonalak rajzolása: Két kattintással meghatározhatók a vonalak kezdő- és végpontjai.

- Téglalapok rajzolása: Két kattintással meghatározhatók a téglalapok sarokpontjai.

- Körök rajzolása: Az első kattintással a kör középpontja, a másodikkal pedig a sugara határozható meg.

- Körök mozgatása: A megrajzolt körök kijelölhetők és mozgathatók.

- Színválasztás: A program tartalmaz egy színpalettát, ahonnan a felhasználó kattintással kiválaszthatja a rajzoláshoz használt színt.

### Futtatás
- ```make``` parancs használata fordításhoz
- ```shapes.exe``` futtatáshoz
- ```make clean``` parancs használata javasolt változtatások esetén (legfőbbkép ```consts.h``` miatt). Ezzel törlődnek a fordított ```.o``` fájlok és a ```.exe``` fájl.

### Dokumentáció
A ```.h``` fájlokban találhatóak hosszabb leírások, viszont ahol szükséges ott a ```.c``` fájlokban is találhatóak rövid kommentek.

**/src:**
- ```main.c:``` A program kezdőpontja

**src/core:**

- ```drawing.c:``` Alakzatok rajzolása, előnézetesen is
- ```event_handler.c:``` Alakzat létrehozás, egér és billenytűzet kezelés
- ```palette.c:``` RGB színpaletta, kattintással választható szín
- ```sdl_utils.c:``` SDL inicializálása és takarítása

**src/shapes:**

- ```line.c:``` Vonal beállítása és veremkezelése
- ```rectangle.c:``` Téglalap beállítása és veremkezelése
- ```circle.c:``` Kör beállítása és veremkezelése

**include/core:**
- ```consts.h:``` A programban használt konstansokat, globális változókat tartalmazza 

**include/structs:**
- ```draw_mode.h:``` Rajzolási módok
- ```draw_state.h:``` Rajzolási állapotok
- ```shape_type.h:``` Alakzat típusát meghatározó állapotok
- ```point.h:``` Pont struktúra (x, y koordináták)
- ```color.h:``` Szín struktúra (RGB értékek)
- ```line_struct.h:``` Vonal struktúra (kezdőpont, végpont, szín)
- ```rect_struct.h:``` Téglalap struktúra (sarokpontok, szín)
- ```circle_struct.h:``` Kör struktúra (középpont, sugár, szín)

### Továbbfejlesztési lehetőségek
- Képernyőméret dinamizálás
- Undo / Redo könnyen implementálható
- Váltás kitöltött és üres alakzatok rajzolása között
- Vonal vastagság megadása
- Éppen aktív szín jelzése
- Redundancia csökkentése
- Még több funkcióra bontás
- Globális változók átdolgozása
- Hibarendszer átdolgozása
- Makefile ```clean``` opciójának megoldása Linuxra is
- Dokumentáció részletes kidolgozása

Ezek nem voltak prioritások a feladat elvégzéséhez, szóval jelenleg nincsenek implementálva ezek a funkcionalitások.