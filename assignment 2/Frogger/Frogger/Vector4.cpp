#include "Vector4.h"


Vector4::Vector4()
{
}

Vector4::Vector4(float x, float y, float z, float w)
	: Vector3(x, y, z), _w(w)
{}

Vector4::~Vector4()
{
}

bool
Vector4::operator=(Vector4 *vec)
{
	if (vec->getX() == _x && vec->getY() == _y && vec->getZ() == _z && vec->getW())
		return true;

	return false;
}

Vector4*
Vector4::operator*(float scalar)
{
	_x *= scalar;
	_y *= scalar;
	_z *= scalar;
	_w *= scalar;
	return this;
}

Vector4*
Vector4::operator+(Vector4 *vec)
{
	_x += vec->getX();
	_y += vec->getY();
	_z += vec->getZ();
	_w += vec->getW();
	return this;
}

Vector4*
Vector4::operator-(Vector4 *vec)
{
	_x -= vec->getX();
	_y -= vec->getY();
	_z -= vec->getZ();
	_w -= vec->getW();
	return this;
}

float*
Vector4::Vec4ToFloat()
{
	float lpos[] = { _x, _y, _z, _w };
	return lpos;
}
