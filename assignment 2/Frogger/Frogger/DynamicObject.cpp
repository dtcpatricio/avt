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
DynamicObject::setTires(){
	amb[0] = .0f; amb[1] = .0f; amb[2] = .0f;
	diff[0] = 0.0f; diff[1] = .0f; diff[2] = .0f;
	spec[0] = .0f; spec[1] = .0f; spec[2] = .0f;
}

void
DynamicObject::setHead(){
	amb[0] = .1f; amb[1] = .5f; amb[2] = .1f;
	diff[0] = .2f; diff[1] = .9f; diff[2] = .2f;
	spec[0] = .2f; spec[1] = .8f; spec[2] = .2f;
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
