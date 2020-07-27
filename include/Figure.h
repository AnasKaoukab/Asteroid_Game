#ifndef FIGURE_H
#define FIGURE_H

#include "Point.h"

#include <SDL2/SDL.h>
#include <cmath>

#include <iostream>
#include <vector>

using namespace std;

#define PI 3.14159

#define VA 10

#define BIG 200
#define MEDIUM 100
#define SMALL 50


#define AGG 50
#define T_X WIDTH/2
#define T_Y HEIGHT/2

#define SIZE_SHOT 5
#define ACCELERATION_SHOT 30
#define SH_SPEED 20

#define MAX_SPEED_SS 25
#define DECELERATION 0.25
#define ACCELERATION 1


struct MovementSpaceship {
	bool left = false;
	bool right = false;
	bool up = false;

	float angle = 90;
	float angle_acc = 0;

	float vx = 0;
	float vy = 0;
};

struct MovementAsteroid {
	float vx = 2;
	float vy = 2;
	float angle_rot = 0.25*PI/180;
};

struct MovementShot {
	float vx = 0;
	float vy = 0;
};


class Figure {
public:
		Figure();
		~Figure();

		vector<Point> getList(void);
		virtual Point getCenter();
		virtual float getAngle();
		virtual void move();
    virtual void display(SDL_Renderer* renderer);
		void wrap();

	protected:
    int size_list;
		vector <Point> list;
		float angle;
		float speed;
		float angle_acc;
};

class Spaceship : public Figure {
	public:
		Spaceship();
		Point getHead();
		Point getCenter();
		int getLife();
		void setLife(int x);
		void setTime(unsigned int t);
		unsigned int getTime();
		void switchInvincible();
		bool getInvincible();
		float getAngle();
		void move();
		void display(SDL_Renderer *renderer);
		void turnLeft(bool b);
		void turnRight(bool b);
		void advance(bool b);
		void teleport();


	private:
		MovementSpaceship m;
		int life = 100;
		bool invincible = true;
		unsigned int time = SDL_GetTicks();


};

class Shot : public Figure {
	public:
		Shot(Point p, float angle);
		Point getCenter();
		void incrNb();
		int getNb();
		void move();


	private:
		MovementShot m;
		int nb = 1;
};

class Asteroid : public Figure {
	public:
		Asteroid();
		Asteroid(int size);
		Asteroid(int size, Point p);
		Point getCenter();
		int getLevel();
		void move();
		unsigned int getPoint();
	private:
		MovementAsteroid m;
		int size;
		unsigned int point;
};

#include "Game.h"

#endif
