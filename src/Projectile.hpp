#include "Vector2d.hpp"

class Projectile
{
private:
	int grav = 9.81;

public:
	unsigned int * image = NULL;
	unsigned int * weapon = NULL;
	Vector2d vector;
 	Projectile(Vector2d _vector) {
		 vector = _vector;
	}
	Projectile() {}
};

struct Image
{
	unsigned int * img_matrix;
	unsigned int width;
	unsigned int height;
};

