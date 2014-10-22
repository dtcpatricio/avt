#include "Entity.h"


Entity::Entity()
{}


Entity::~Entity()
{}

Vector3*
Entity::getPosition()
{
	return &_position;
}

Vector3*
Entity::setPosition(float x, float y, float z)
{
	_position.set(x, y, z);
	return &_position;
}

Vector3*
Entity::setPosition(Vector3 vec)
{
	_position.set(vec.getX(), vec.getY(), vec.getZ());
	return &_position;
}
