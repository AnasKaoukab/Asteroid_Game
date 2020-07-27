#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>

#include <cmath>
#include <iostream>
#include <vector>
#include <thread>

#include "Point.h"
#include "Figure.h"
#include "Item.h"


using namespace std;

#define WIDTH 1200
#define HEIGHT 800

#define DELAY 25

#define DISTANCE_SHOT 30
#define NB_AST_MIN 4

#define INV_TIME 2000

struct Sound {
  const char* name;
	Uint8 *audio_pos; // global pointer to the audio buffer to be played
	Uint32 audio_len; // remaining length of the sample we have to play

	Uint32 wav_length; // length of our sample
	Uint8 *wav_buffer = nullptr; // buffer containing our audio file
	SDL_AudioSpec wav_spec; // the specs of our piece of music
} ;


class Game {
public:
  Game();
  ~Game();
  void start();
  void menu();
  void end();
  void new_shot(Point p, float angle);
  void new_asteroid(int size, int nb, Point p);
  void new_asteroid(int size, int nb);
  void update();
  void updateScore(int life);
  void playSong();


private:
  Sound s;

  SDL_Window* window;
  SDL_Renderer* renderer;
  int width;
  int height;

  bool end_b = false;
  bool restart = false;


  //VARIABLE
  bool shoot = false;
  SDL_Surface* surface;
  SDL_Texture* texture;


  //BACKGROUND
  SDL_Texture* background;

  //SCORE
  TTF_Font* font;
  unsigned int score = 0;

  Spaceship ss;
  vector<Asteroid> a_list;
  vector<Shot> s_list;

  Medikit medikit;

};


#endif
