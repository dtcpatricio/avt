#include "Turtle.h"


Turtle::Turtle(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* calc)
	: DynamicObject(mySurf, shader, calc)
{
	id[0] = 12;
	id[1] = 13;
}


Turtle::~Turtle()
{
}

void
Turtle::create()
{
	// head
	_mySurf->setObjId(id[0]);
	_mySurf->createSphere(.5f, 32);

	// body and paws
	_mySurf->setObjId(id[1]);
	_mySurf->createRectangle(.7f, .3f, .5f);
}

void
Turtle::draw()
{

}

void
Turtle::update(float delta_t)
{

}
