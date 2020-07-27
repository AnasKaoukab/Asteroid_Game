#include "include/Point.h"


Point::Point() {};

Point::Point(float x, float y):x(x), y(y) {};

void Point::setX(float new_x){
	x = new_x;
}

void Point::setY(float new_y){
	y = new_y;
}

void Point::incr(float speed, float angle) {
	float a = angle * PI / 180;
	x = x + speed*cos(a);
	y = y - speed*sin(a);
}

float Point::getX(void){
	return x;
}

float Point::getY(void){
	return y;
}
