struct Vector3d
{
	int x, y;
	float angle;

	Vector3d(int _x, int _y, int _z, float _angle) {
		x = _x;
		y = _y;
		angle = _angle; 
	}

	Vector3d(int _x, int _y) {
		x = _x;
		y = _y;
	}

	Vector3d(){}
};
