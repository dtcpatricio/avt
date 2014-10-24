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
	_mySurf->createSphere(.3f, 16);

	// body and paws
	_mySurf->setObjId(id[1]);
	_mySurf->createRectangle(.7f, .5f, 1.0f);
}

void
Turtle::draw()
{
	_calc->translation(_position.getX() - .3f, _position.getY(), _position.getZ()+0.4f);
	applyColor(0.0f, 1.0f, .0f);
	sendToGL(id[0]);
	_calc->translation(_position.getX(), _position.getY(), _position.getZ());
	applyColor(0.6f, .4f, .0f);
	sendToGL(id[1]);
}

void
Turtle::update(float delta_t)
{
	float deltaX = _position.getX() - delta_t;

	if (deltaX <= -15.0f)
		setPosition(14.5f, _position.getY(), _position.getZ());
	else
		setPosition(deltaX, _position.getY(), _position.getZ());
}
