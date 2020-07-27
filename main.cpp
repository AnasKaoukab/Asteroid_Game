#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>

#include <cmath>
#include <iostream>
#include <vector>

#include "include/Point.h"
#include "include/Figure.h"
#include "include/Game.h"

using namespace std;


int main(void){
	Game game;

	game.menu();
	game.start();
}
