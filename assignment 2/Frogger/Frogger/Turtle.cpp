#include "Turtle.h"


Turtle::Turtle(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* calc)
	: DynamicObject(mySurf, shader, calc)
{
	id[0] = 12;
	id[1] = 13;

	diff[0] = .5f;        diff[1] = .3f;        diff[2] = .1f;        diff[3] = 1.f;
	amb[0]  = .5f * .25f; amb[1]  = .3f * .25f; amb[2]  = .1f * .25f; amb[3]  = 1.f;
	spec[0] = .1f;        spec[1] = .1f;        spec[2] = .05f;       spec[3] = 1.f;
	emissive[0] = 0.f;  emissive[1] = 0.f; emissive[2] = 0.f; emissive[3] = 1.f;
	shininess = new float(100.f);
}


Turtle::~Turtle()
{
}

void
Turtle::create()
{
	// body and paws
	_mySurf->setObjId(id[1]);
	_mySurf->createRectangle(.7f, .5f, 1.0f);
	applyColor();

	setHead();

	// head
	_mySurf->setObjId(id[0]);
	_mySurf->createSphere(.3f, 16);
	applyColor();

}

void
Turtle::draw()
{
	_mySurf->setObjMaterials(id[1], _shader);
	_calc->translation(_position.getX(), _position.getY(), _position.getZ());
	sendToGL(id[1]);

	_mySurf->setObjMaterials(id[0], _shader);
	_calc->translation(_position.getX() - .3f, _position.getY(), _position.getZ()+0.4f);
	sendToGL(id[0]);

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