#include "DynamicObject.h"


DynamicObject::DynamicObject(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* calc)
	: GameObject(mySurf, shader, calc)
{
}


DynamicObject::~DynamicObject()
{
}

void
DynamicObject::update(float delta_t)
{

}

void
DynamicObject::setSpeed(Vector3* speed)
{
	_speed.set(speed->getX(), speed->getY(), speed->getZ());
}

void
DynamicObject::setSpeed(float x, float y, float z)
{
	_speed.set(x, y, z);
}

Vector3
DynamicObject::getSpeed()
{
	return _speed;
}
