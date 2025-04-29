## Számítógépi grafika gyakorlati repository

**Név:** Czibik Lóránt Patrik

**Neptun:** K1IFAB

## Féléves feladat specifikáció

**Projekt neve: L.O.P.A.S.** 

A programot a R.E.P.O. nevű early access roguelite játék inspirálta. Az elnevezés hasonló formátumú, de a "repossession" (visszavétel) szó fordítása helyett a "lopás" szóra utal.

### Cél
A program célja, hogy a felhasználó tetszőleges mennyiségű tárgyat gyűjtsön össze, amelyeket a játék végén a gyűjtési ponton eladhat. A cél az adott érték elérése, de ezen felül is lehet értékesíteni. A nehézséget a tárgyak épségben tartása jelenti, mivel azok megsérülhetnek vagy összetörhetnek.

### Alapfunkcionalitások

**Kamerakezelés:**
- 3rd person kamera (F4 billenytűvel orbitális kamera is elérhető a pálya áttekintéséhez)
- Állítható FOV (látómező)

**Objektumok:**
- Térbeli elhelyezésű modellek
- Anyagjellemzők, textúrák és transzformációk
- Egyéb tulajdonságok

**Animáció és mozgatás:**
- Egérrel történő objektummanipuláció:
  - Bal egérgomb nyomva tartása + mozgás: fel/le/jobbra/balra
  - Görgő: előre/hátra mozgatás
  - Jobb egérgomb nyomva tartása: forgatás tetszőleges irányba

**Fények:**
- Több fényforrás a jelenetben
- Fényerő szabályozása: F5 (csökkentés) és F6 (növelés) billentyűkkel

**Használati útmutató:**
- Elérhető a Manual/Instructions menüpont alatt

### Többlet funkciók

**Fizika és ütközésvizsgálat:**
- ODE (Open Dynamics Engine) könyvtár alkalmazása
- Közel valósághű fizikai szimuláció

**Objektumkijelölés:**
- Egérrel történő kijelölés Ray-OBB vizsgálattal
- Kijelölt objektum OBB keretének megjelenítése

**Felhasználói felület (SDL2 alapú):**

**Főmenü:**
- Start
- Settings
    - Fullscreen
    - FOV
    - Brightness
- Manual
    - About
    - Controls
    - Instructions
- Exit

**Játékmenü:**
- Resume
- Settings
- Manual
- Exit

> **Megjegyzés**
>
> A többlet funkciók az alábbi elemekkel bővülhetnek, ha elegendő fejlesztési idő áll rendelkezésre:
> 
> - Részecskerendszerek
> - Átlátszóság