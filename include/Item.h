#ifndef ITEM_H
#define ITEM_H

#include "Point.h"

#include <SDL2/SDL.h>
#include <cmath>

#include <iostream>
#include <vector>

using namespace std;


#define LEN 30



class Medikit {
  public:
      Medikit();
      ~Medikit();
      void testNew();
      bool getAlready();
      void display(SDL_Renderer* renderer);
			Point getCenter();
			float getLen();
			void switchAlready();
			int action();
  private:
    unsigned int timer = SDL_GetTicks();
    bool already = false;
    Point center;
    float len = LEN;
    float wdth = LEN/3;
	 	int sign = 1;
};

#include "Game.h"

#endif
