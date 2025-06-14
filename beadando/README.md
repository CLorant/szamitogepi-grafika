# Projekt beállítása

## Assetek

- A modellek, textúrák és a használati útmutató szöveges fájlja elérhető ezen a linken: [lopas_assets.zip](https://drive.google.com/file/d/1R0g4qejwjD2iUC2yY1PqGZK-9NvAloCk/view?usp=sharing)
- Az archívum assets mappáját szükséges bemásolni a program gyökerébe.

## Könyvtárak

Az alábbi könyvtárakat CMake használatával ajánlott beüzemeltetni, a lépéseket ehhez írtam.

ODE:
- `git clone https://bitbucket.org/odedevs/ode/`
- `mkdir ode-build && cd ode_build`
- CMake GUI -> source: /ode build: /ode_build
- Configure gomb
- Generate gomb
- `make` parancs kiadása terminálon
- MinGW/include mappa -> ode mappa létrehozása
- /ode mappába a .h fájlok ami a klónozott repoban van, kivéve az odecpp.h és odecpp_collision.h
- MinGW/lib mappába a libode-single.dll.a fájl
- MinGW/bin mappába a libode-single.dll fájl

json-c:
- `git clone https://github.com/json-c/json-c.git`
- `mkdir json-c-build && cd json-c-build`
- `cmake -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -G "MinGW Makefiles" ../json-c`
- `make`
- MinGW/include/json-c mappába .h fájlok
- MinGW/lib mappába .a és .dll.a fájlok
- MinGW/bin mappába .dll fájl

> Megjegyzések:
> - Headerek, statikus könytárak és windows 10 binárisok ha bármi probléma akadna a beüzemeltetéssel: [lopas_libs.zip](https://drive.google.com/file/d/1X-fGPROsnLuwVIHEDoHjUfZa0e7GOLeG/view?usp=sharing)
> - Kérem írjon ha valami oknál fogva nem működik az alkalmazás.