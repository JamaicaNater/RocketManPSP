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
private:
	int grav = 9.81;

public:
	Image image = Image();
	Image weapon = Image();
	Vector2d vector;
 	Projectile(Vector2d _vector) {
		 vector = _vector;
	}
	Projectile() {}
};



