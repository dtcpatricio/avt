#include "Frog.h"


Frog::Frog()
{
}


Frog::~Frog()
{
}

void
Frog::setX(float x) {
	xPos = x;
}

void
Frog::setY(float y) {
	yPos = y;
}

void
Frog::setZ(float z) {
	zPos = z;
}

float
Frog::getX() {
	return xPos;
}

float
Frog::getY() {
	return yPos;
}

float
Frog::getZ() {
	return zPos;
}
