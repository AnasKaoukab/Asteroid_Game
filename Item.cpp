#include "include/Item.h"

#define PROB_MED 0.1


Medikit::Medikit(){
  srand(time(NULL));
  float x = ((float) rand()) / (float) RAND_MAX * (WIDTH-10);
  float y = ((float) rand()) / (float) RAND_MAX * (HEIGHT-10);

  Point p(x,y);
  center = p;
};

Medikit::~Medikit(){};

void Medikit::display(SDL_Renderer* renderer) {

  // ne pas afficher si il n'y a pas de medikit;
  if (!already) {
    return;
  }

  if (len >= LEN+15) {
    sign = -1;
  }
  else if (len <= LEN-0){
    sign = 1;
  }


  // len/wdth = 3;
  wdth = wdth + sign * 0.25;
  len = len + 3 * sign * 0.25;

  SDL_Rect r;

  // --
  SDL_SetRenderDrawColor(renderer, 255,0,0,255);
  r = {(int) (center.getX()-wdth/2), (int) (center.getY()-len/2), (int) wdth, (int) len};
  SDL_RenderFillRect( renderer, &r );
  // |
  r = {(int) (center.getX()-len/2), (int) (center.getY()-wdth/2), (int) len, (int) wdth};
  SDL_RenderFillRect( renderer, &r );


};

void Medikit::testNew(){
  float x = 42;
  if (!already && SDL_GetTicks()-timer > 500) {
    timer = SDL_GetTicks();
    srand(time(NULL));
    x = ((float) rand()) / (float) RAND_MAX;
  }
  already = (x < PROB_MED);
};

bool Medikit::getAlready(){
  return already;
};


Point Medikit::getCenter(){
  return center;
};
float Medikit::getLen(){
  return len;
};

void Medikit::switchAlready(){
  already = !already;
};

int Medikit::action(){
  return 1;
}
