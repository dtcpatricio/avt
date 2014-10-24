#include "Turtle.h"


Turtle::Turtle(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* calc)
	: DynamicObject(mySurf, shader, calc)
{
	id[0] = 12;
	id[1] = 13;
	amb[0] = .2f; amb[1] = .15f; amb[2] = .1f; amb[3] = 1.f;
	diff[0] = .3f; diff[1] = .3f; diff[2] = .3f; diff[3] = 1.f;
	spec[0] = .1f; spec[1] = .1f; spec[2] = .1f; spec[3] = 1.f;
	emissive[0] = 0.f;  emissive[1] = 0.f; emissive[2] = 0.f; emissive[3] = 1.f;
	shininess = new float(180.f);
}


Turtle::~Turtle()
{
}

void
Turtle::create()
{
	// head
	_mySurf->setObjId(id[0]);
	applyColor();
	_mySurf->createSphere(.3f, 16);

	// body and paws
	_mySurf->setObjId(id[1]);
	applyColor();
	_mySurf->createRectangle(.7f, .5f, 1.0f);
}

void
Turtle::draw()
{
	_calc->translation(_position.getX() - .3f, _position.getY(), _position.getZ()+0.4f);
	sendToGL(id[0]);
	_calc->translation(_position.getX(), _position.getY(), _position.getZ());
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
