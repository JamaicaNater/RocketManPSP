#include "Vector2d.hpp"

class Projectile
{
private:
	int grav = 9.81;

public:
	Vector2d vector;
 	Projectile(Vector2d _vector) {
		 vector = _vector;
	}
	Projectile() {}
};
