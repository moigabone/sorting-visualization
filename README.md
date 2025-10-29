To start the program : 

compil : gcc src/main.c src/utils.c src/visual.c src/sorting.c -o program -Iinclude $(sdl2-config --cflags --libs) -lSDL2_ttf -Werror

then launch : ./program
