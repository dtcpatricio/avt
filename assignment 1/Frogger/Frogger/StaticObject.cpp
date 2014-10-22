#include "StaticObject.h"


StaticObject::StaticObject(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* calc)
	: GameObject(mySurf, shader, calc)
{}


StaticObject::~StaticObject()
{
}
