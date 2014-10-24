#pragma once
#include "Vector3.h"
class Vector4 :
	public Vector3
{
public:
	Vector4();
	Vector4(float x, float y, float z, float w);
	~Vector4();

	float getW() { return _w; };
	void set(float x, float y, float z, float w) {
		_x = x; _y = y; _z = z; _w = w;
	};

	float* Vec4ToFloat();
	bool operator=(Vector4 *vec);
	Vector4* operator*(float scalar);
	Vector4* operator+(Vector4 *vec);
	Vector4* operator-(Vector4 *vec);

private:
	float _w;
};

