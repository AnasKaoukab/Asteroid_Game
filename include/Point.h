#ifndef POINT_H
#define POINT_H

#include <cmath>
#include <iostream>

using namespace std;

#define SPEED 10
#define PI 3.14159

class Point {
	public:
		Point();
		Point(float x, float y);
		void incr(float speed, float a);
		float getX(void);
		float getY(void);
		void setX(float x);
		void setY(float y);

	private :
		float x ;
		float y ;
};


#endif
