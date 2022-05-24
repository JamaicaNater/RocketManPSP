#pragma once
#include "Vector2d.hpp"

struct Image
{
	unsigned int * img_matrix;
	unsigned int width;
	unsigned int height;

	Image() {
		img_matrix = NULL;
		width = 0;
		height = 0;
	}
};

class Projectile
{
	public:
	float grav = 110.00f;
	Image image = Image();
	//Image weapon = Image();
	Vector2d vector;
 	Projectile(Vector2d _vector) {
		 vector = _vector;
	}
	Projectile() {}
};

class Person: public Projectile 
{
	public:
	Image weapon = Image();
	
	bool jumping = false;
	unsigned int jump_time;
	int jump_height = 0;
	int starting_jump_height;

	unsigned short jump_height_at(int microseconds) {
		

		return 1 ;
	}

} ;



