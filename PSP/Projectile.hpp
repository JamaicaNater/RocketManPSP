#include "Vector2d.hpp"

class Projectile
{
private:
	int grav = 9.81;

public:
	unsigned char * image = NULL;
	Vector2d vector;
 	Projectile(Vector2d _vector) {
		 vector = _vector;
	}
	Projectile() {}
};
