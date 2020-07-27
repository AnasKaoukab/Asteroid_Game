main:  Point.cpp Figure.cpp main.cpp  Item.cpp Game.cpp
	g++ -o main Point.cpp Figure.cpp main.cpp Item.cpp Game.cpp  `sdl2-config --cflags --libs` -lSDL2 -lSDL2_image -lSDL2_ttf -Wall -Wextra

clean:
	rm main
