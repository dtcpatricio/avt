#pragma once

#include "Vector3.h"
#include "MathLib.h"

class Entity
{
public:
	Entity();
	~Entity();

	Vector3* getPosition();
	Vector3* setPosition(float x, float y, float z);
	Vector3* setPosition(Vector3 *p);

protected:
	Vector3 _position;
};

