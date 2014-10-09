#include "SimpleGeometryLib.h"
#include "vsResSurfRevLib.h"

VSResSurfRevLib mySurface;

SimpleGeometryLib::SimpleGeometryLib() {}

SimpleGeometryLib::~SimpleGeometryLib() {}

void
SimpleGeometryLib::createCube(float size) {
		mySurface.createCylinder(size, size, 4);
}