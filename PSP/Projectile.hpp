#include "Vector3d.hpp"

class Projectile
{
private:
	int grav = 9.81;

public:
	Vector3d vector;
 	Projectile(Vector3d _vector) {
		 vector = _vector;
	}
	Projectile() {}
};
