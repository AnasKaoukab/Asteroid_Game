#include "include/Figure.h"

//////////////////////////////////////////////////////////

Point middle(Point p1, Point p2) {
	float x = abs((p1.getX()+p2.getX())/2);
	float y = abs((p1.getY()+p2.getY())/2);

	return Point(x,y);
}


float random(float min, float max) {
	return ((float) rand()) / (float) RAND_MAX * (max-min) + min;
};


//////////////////////////////////////////////////////////

Figure::Figure(){};

Figure::~Figure(){
};

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

void Figure::move(){};

vector<Point> Figure::getList(){
	return list;
}

float Figure::getAngle(){
	return 90;
};



Point Figure::getCenter(){
	return list[1];
}


void Figure::display(SDL_Renderer* renderer){
  int size = size_list;
  SDL_SetRenderDrawColor(renderer, 255,255,255,255);

 	vector<Point> l = list;

 	for (int i(0) ; i<size; i++){
 		if (i == size-1) {
 			SDL_RenderDrawLine(renderer, l[i].getX(),l[i].getY(),l[0].getX(),l[0].getY());
		}
 		else SDL_RenderDrawLine(renderer, l[i].getX(),l[i].getY(),l[i+1].getX(),l[i+1].getY());
 	}
}

int test_left(vector<Point> list, int size) {
	for (int i(0) ; i < size ; i++) {
		if (list[i].getX() > 0)
			return 0;
	}
	return 1;
}

int test_right(vector<Point> list, int size) {
	for (int i(0) ; i < size ; i++) {
		if (list[i].getX() < WIDTH)
			return 0;
	}
	return -1;
}

int test_up(vector<Point> list, int size) {
	for (int i(0) ; i < size ; i++) {
		if (list[i].getY() > 0)
			return 0;
	}
	return 1;
}

int test_down(vector<Point> list, int size) {
	for (int i(0) ; i < size ; i++) {
		if (list[i].getY() < HEIGHT)
			return 0;
	}
	return -1;
}

float dist(Point p1, Point p2) {
	float x = powf(p1.getX()-p2.getX(),2);
	float y = powf(p1.getY()-p2.getY(),2);

	return sqrtf(x+y);
}

void Figure::wrap() {
	int size = size_list;
	int sign_x = 0;
	int sign_y = 0;
	float shift = 0;

	sign_x = test_left(list,size) + test_right(list,size);
	sign_y = test_down(list,size) + test_up(list,size);

	if (sign_x)
		for (int i(0) ; i < size ; i++)
			list[i].setX(list[i].getX() + sign_x * (WIDTH + shift));

	else if (sign_y)
		for (int i(0) ; i < size ; i++)
			list[i].setY(list[i].getY() + sign_y * (HEIGHT + shift));
}
//////////////////////////////////////////////////

Spaceship::Spaceship() {
	angle = 90;
	angle_acc = angle;
  size_list = 4;
	speed = 0;

  list.push_back(Point(0.5, 0));
  list.push_back(Point(0.2, 1));
  list.push_back(Point(0.5, 0.65));
  list.push_back(Point(0.8, 1));

	for (int i(0) ; i < size_list ; i++) {
		list[i].setX(list[i].getX() * AGG + T_X);
		list[i].setY(list[i].getY() * AGG + T_Y);
	}
}

void Spaceship::turnLeft(bool b) {
	m.left = b;
};
void Spaceship::turnRight(bool b){
	m.right = b;
};
void Spaceship::advance(bool b){
	m.up = b;
};

float Spaceship::getAngle(){
	return m.angle;
};


Point operator+(Point p1, Point p2) {
	float x = p1.getX() + p2.getX();
	float y = p1.getY() + p2.getY();
	return Point(x,y);
};

Point operator+(Point p1, float t) {
	float x = p1.getX() + t;
	float y = p1.getY() + t;
	return Point(x,y);
};

void Spaceship::move(){
	float a;
	float dt_x = 0;
	float dt_y = 0;

	m.angle += (m.left - m.right) * VA;
	m.angle = fmod(360 + m.angle, 360);

	if (m.up) {
		m.angle_acc = m.angle;
	}

	if (m.left || m.right) {
		float x, y, x_c, y_c, x_p, y_p;
		Point p_c = getCenter();

		x_c = p_c.getX();
		y_c = p_c.getY();

		a = (m.left - m.right) * VA * PI / 180;

		for (int i(0) ; a != 0 && i < size_list ; i++) {
			x_p = list[i].getX();
			y_p = list[i].getY();
			x = x_p-x_c;
			y = y_p-y_c;

			list[i].setX(x*cos(a)+y*sin(a)+x_c);
			list[i].setY(-x*sin(a)+y*cos(a)+y_c);
		}
	}


	if (m.vx != 0 and fabs(m.vx)-DECELERATION >= 0) {
		m.vx -= DECELERATION*sgn(m.vx);
	}
	else {
		m.vx = 0;
	}

	if(m.vy != 0 and fabs(m.vy)-DECELERATION >= 0) {
		m.vy -= DECELERATION*sgn(m.vy);
	}
	else {
		m.vy = 0;
	}

	if(m.up) {
		float tmp_x, tmp_y;
		dt_x = ACCELERATION*cos(m.angle_acc*PI/180);
		dt_y = -ACCELERATION*sin(m.angle_acc*PI/180);

		tmp_x = fabs(m.vx+dt_x);
		tmp_y = fabs(m.vy+dt_y);

		if (sqrtf(tmp_x*tmp_x+tmp_y*tmp_y) <= MAX_SPEED_SS){
			m.vx += dt_x;
			m.vy += dt_y;
		}
	}

	// update

	for (int i(0) ; i < size_list ; i++) {
		list[i].setX(list[i].getX() + m.vx);
		list[i].setY(list[i].getY() + m.vy);
	}
}

Point milieu(Point p1, Point p2) {
	float x = (p1.getX() + p2.getX())/2;
	float y = (p1.getY() + p2.getY())/2;
	return Point(x,y);
}



void Spaceship::display(SDL_Renderer* renderer){
	if (invincible && fmod(SDL_GetTicks(),500) < 100) {
		return;
	}

  int size = size_list;
  SDL_SetRenderDrawColor(renderer, 255,255,255,255);

 	vector<Point> l = list;

 	for (int i(0) ; i<size; i++){
 		if (i == size-1) {
 			SDL_RenderDrawLine(renderer, l[i].getX(),l[i].getY(),l[0].getX(),l[0].getY());
		}
 		else SDL_RenderDrawLine(renderer, l[i].getX(),l[i].getY(),l[i+1].getX(),l[i+1].getY());
 	}

	if (m.up) {
		Point p1 = milieu(list[1], list[2]);
		Point p2 = milieu(list[2], list[3]);
		Point p3 = milieu(list[1], list[3]);

		SDL_SetRenderDrawColor(renderer, 255,150,50,255);

		SDL_RenderDrawLine(renderer, p1.getX(),p1.getY(),p3.getX(),p3.getY());
		SDL_RenderDrawLine(renderer, p3.getX(),p3.getY(),p2.getX(),p2.getY());
	}

}
Point Spaceship::getCenter() {
	return list[2];
}

Point Spaceship::getHead() {
	return list[0];
}

int Spaceship::getLife() {
	return life;
}

void Spaceship::setLife(int x) {
	if (x > 100) {
		life = 100;
	}
	else if (x < 0) {
		life = 0;
	}
	else life = x;
}

void Spaceship::setTime(unsigned int t) {
	time = t;
}
unsigned int Spaceship::getTime() {
	return time;
}

void Spaceship::switchInvincible() {
	invincible = !invincible;
}

bool Spaceship::getInvincible() {
	return invincible;
}

void Spaceship::teleport() {
	float x_t = random(0, WIDTH);
	float y_t = random(0, HEIGHT);
for (int i(0) ; i < size_list ; i++) {
		list[i].setX(list[i].getX() + x_t);
		list[i].setY(list[i].getY() + y_t);
	}
};





//////////////////////////////////////////////////

Shot::Shot(Point p, float angle_ss) {
	size_list = 2;
	angle = angle_ss;
	speed = SH_SPEED;

	float a = angle * PI / 180;


	// speed
	m.vx = ACCELERATION_SHOT*cos(angle_ss*PI/180);
	m.vy = -ACCELERATION_SHOT*sin(angle_ss*PI/180);

	list.push_back(p);
	float x = p.getX();
	float y = p.getY();
	Point p2 = Point(SIZE_SHOT*cos(a)+x,-SIZE_SHOT*sin(a)+y);
	list.push_back(p2);
}

void Shot::move(){
	for (int i(0) ; i < size_list ; i++) {
		list[i].setX(list[i].getX() + m.vx);
		list[i].setY(list[i].getY() + m.vy);
	}
};

void Shot::incrNb() {
	nb = nb + 1;
}
int Shot::getNb() {
	return nb;
}

Point Shot::getCenter(){
	return list[1];
}

/////////////////////////////////////////////////////////
#define SQRT2_SUR_2 0.7071

Asteroid::Asteroid() {}

Asteroid::Asteroid(int size, Point p) : size(size){
	switch (size) {
		case BIG:
			point = 5000;
			break;
		case MEDIUM:
			point = 2500;
			break;
		case SMALL:
			point = 1000;
			break;
	}


	size_list = 8;
	m.angle_rot = random(-1,1);
	m.vx = random(-5,5);
	m.vy = random(-5,5);

	float x_t = p.getX() + random(-50, 50);
	float y_t = p.getY() + random(-50, 50);

	list.push_back(Point(0.5,0));
	list.push_back(Point(0.5+SQRT2_SUR_2/2,0.5-SQRT2_SUR_2/2));
	list.push_back(Point(1,0.5));
	list.push_back(Point(0.5+SQRT2_SUR_2/2,0.5+SQRT2_SUR_2/2));
	list.push_back(Point(0.5,1));
	list.push_back(Point(0.5-SQRT2_SUR_2/2,0.5+SQRT2_SUR_2/2));
	list.push_back(Point(0,0.5));
	list.push_back(Point(0.5-SQRT2_SUR_2/2,0.5-SQRT2_SUR_2/2));

	for (int i(0) ; i < size_list ; i++) {
		list[i].setX(list[i].getX() * size + x_t);
		list[i].setY(list[i].getY() * size + y_t);
	}
};

Asteroid::Asteroid(int size) : size(size) {
	switch (size) {
		case BIG:
			point = 5000;
			break;
		case MEDIUM:
			point = 2500;
			break;
		case SMALL:
			point = 1000;
			break;
	}


	size_list = 8;
	m.angle_rot = random(-1,1);
	m.vx = random(-5,5);
	m.vy = random(-5,5);

	float x_t = random(0, WIDTH);
	float y_t = random(0, HEIGHT);

	list.push_back(Point(0.5,0));
	list.push_back(Point(0.5+SQRT2_SUR_2/2,0.5-SQRT2_SUR_2/2));
	list.push_back(Point(1,0.5));
	list.push_back(Point(0.5+SQRT2_SUR_2/2,0.5+SQRT2_SUR_2/2));
	list.push_back(Point(0.5,1));
	list.push_back(Point(0.5-SQRT2_SUR_2/2,0.5+SQRT2_SUR_2/2));
	list.push_back(Point(0,0.5));
	list.push_back(Point(0.5-SQRT2_SUR_2/2,0.5-SQRT2_SUR_2/2));

	for (int i(0) ; i < size_list ; i++) {
		list[i].setX(list[i].getX() * size + x_t);
		list[i].setY(list[i].getY() * size + y_t);
	}
};


unsigned int Asteroid::getPoint(){
	return point;
};


void Asteroid::move() {
	float x, y, x_c, y_c, x_p, y_p;
	Point p_c = getCenter();
	float angle = m.angle_rot * PI / 180;

	x_c = p_c.getX();
	y_c = p_c.getY();

	for (int i(0) ; i < size_list ; i++) {
		x_p = list[i].getX();
		y_p = list[i].getY();
		x = x_p-x_c;
		y = y_p-y_c;

		list[i].setX(x*cos(angle)+y*sin(angle)+x_c);
		list[i].setY(-x*sin(angle)+y*cos(angle)+y_c);

		list[i].setX(list[i].getX() + m.vx);
		list[i].setY(list[i].getY() + m.vy);

	}
};

Point Asteroid::getCenter() {
	Point p1,p2;
	p1 = list[0];
	p2 = list[4];
	return middle(p1,p2);
}

int Asteroid::getLevel() {
	return size;
};
