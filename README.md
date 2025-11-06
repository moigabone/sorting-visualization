This is a C project for ESIEA in 3A that visualizes sorting algorithms using the SDL2 library.

It fulfills all requirements for Steps 1, 2, and 3 of the project subject.

Features:

4 Algorithms : Visualize Bubble Sort, Selection Sort, Insertion Sort and Quick Sort.

Interactive Menu : Select your algorithm from the side-menu with the keyboard.

Real-Time Stats : A live dashboard shows Execution time, Comparisons and Memory Accesses.

Full Controls : 

  'S' to Start / Resume.
  'E' to Stop / Pause.
  'R' to Reset the array and all stats.

How to Compile & Run

1. Install Libraries

You must install the SDL2 and SDL2_ttf development libraries:
sudo apt install libsdl2-dev libsdl2-ttf-dev

2. Place Font

Place your font file (e.g., font.otf) in the root of the project folder (next to where program will be).

3. Compile

Run this command from the root of the project folder. This command includes the required -Werror flag.
gcc src/main.c src/utils.c src/visual.c src/sorting.c src/stats.c -o program -Iinclude $(sdl2-config --cflags --libs) -lSDL2_ttf -lm -Werror

4. Run

./program
