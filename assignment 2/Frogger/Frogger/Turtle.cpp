#include "Turtle.h"


Turtle::Turtle(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* calc)
	: DynamicObject(mySurf, shader, calc)
{
	id[0] = 11;
	id[1] = 12;

	amb[0] = 0.2f; amb[1] = 0.1f; amb[2] = 0.1f; amb[3] = 1.f;
	diff[0] = .6f; diff[1] = .4f; diff[2] = .1f; diff[3] = 1.f;
	spec[0] = .6f; spec[1] = .3f; spec[2] = .05f; spec[3] = 1.f;
	emissive[0] = 0.f;  emissive[1] = 0.f; emissive[2] = 0.f; emissive[3] = 1.f;
	shininess = new float(100.f);

	_obj_length = 2.2f * 1.1f;
	_obj_width = 2.2f * .9f;
	_boundingBox->setParams(_obj_length, _obj_width);
}


Turtle::~Turtle()
{
}

void
Turtle::create()
{
	// body and paws
	_mySurf->setObjId(id[1]);
	_mySurf->createSphere(1.1f, 64);
	applyColor();

	setHead();

	// head
	_mySurf->setObjId(id[0]);
	_mySurf->createSphere(.6f, 64);
	applyColor();

}

void
Turtle::draw()
{
	_mySurf->setObjMaterials(id[1], _shader);
	_calc->translation(_position.getX(), _position.getY(), _position.getZ());
	_calc->scale(1.1f, 1.0f, .9f);
	sendToGL(id[1]);

	_mySurf->setObjMaterials(id[0], _shader);
	_calc->translation(_position.getX() - 1.55f, _position.getY(), _position.getZ());
	sendToGL(id[0]);

}

void
Turtle::update(float delta_t)
{
	float deltaX = _position.getX() - delta_t;

	if (deltaX <= -16.5f)
		setPosition(17.f, _position.getY(), _position.getZ());
	else
		setPosition(deltaX, _position.getY(), _position.getZ());
}
