# UnnamedRoguelike

Requirements:
-------------
1. Ncurses lib
2. Modern terminal emulator with advanced color support (I recommend urxvt)

### Credits

Thanks to [Auburns](https://github.com/Auburns) for his [FastNoise](https://github.com/Auburns/FastNoise). Great job that saves thousands of hours.
Thanks to [Daancode](https://github.com/daancode) for his [A-star pathfinding](https://github.com/daancode/a-star). I took his code and rewrited it for 3-d usage.

Compile:
--------
```
g++ *.cpp interface/*.cpp -lncurses -lmenu -o Game
```
This project is buggy, so you may want to compile like this to run it with gdb:

```
g++ *.cpp interface/*.cpp -lncurses -lmenu -o Game -g && gdb ./Game
```
Run:
----
```
./Game
```
