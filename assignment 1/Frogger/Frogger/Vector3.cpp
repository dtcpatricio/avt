#include "Vector3.h"


Vector3::Vector3()
{}

Vector3::Vector3(float x, float y, float z) 
	: _x(x), _y(y), _z(z)
{}

Vector3::~Vector3()
{}

float
Vector3::getX()
{
	return _x;
}

float
Vector3::getY()
{
	return _y;
}

float
Vector3::getZ()
{
	return _z;
}

void
Vector3::set(float x, float y, float z)
{
	_x = x;
	_y = y;
	_z = z;
}

bool
Vector3::operator=(Vector3 vec)
{
	if (vec.getX() == _x && vec.getY() == _y && vec.getZ() == _z)
		return true;
	
	return false;
}

Vector3*
Vector3::operator*(float scalar)
{
	_x *= scalar;
	_y *= scalar;
	_z *= scalar;
	return this;
}

Vector3*
Vector3::operator+(Vector3 vec)
{
	_x += vec.getX();
	_y += vec.getY();
	_z += vec.getZ();
	return this;
}

Vector3*
Vector3::operator-(Vector3 vec)
{
	_x -= vec.getX();
	_y -= vec.getY();
	_z -= vec.getZ();
	return this;
}

