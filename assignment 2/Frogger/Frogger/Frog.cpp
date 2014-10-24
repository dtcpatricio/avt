#include "Frog.h"

Frog::Frog(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* calc)
	: DynamicObject(mySurf, shader, calc)
{
	id[0] = 9;
	id[1] = 10;
	amb[0] = .2f; amb[1] = .15f; amb[2] = .1f; amb[3] = 1.f;
	diff[0] = .3f; diff[1] = .3f; diff[2] = .3f; diff[3] = 1.f;
	spec[0] = .1f; spec[1] = .1f; spec[2] = .1f; spec[3] = 1.f;
	emissive[0] = 0.f;  emissive[1] = 0.f; emissive[2] = 0.f; emissive[3] = 1.f;
	shininess = new float(180.f);
}

Frog::~Frog() {}

void
Frog::create()
{
	_mySurf->setObjId(id[0]);
	applyColor();
	_mySurf->createCube(1.0f);

	_mySurf->setObjId(id[1]);
	applyColor();
	_mySurf->createSphere(.5f, 16);
}

void
Frog::draw()
{
	_calc->translation(_position.getX(), _position.getY(), _position.getZ());
	sendToGL(id[0]);
	_calc->translation(_position.getX() + .5f, _position.getY() + 1.5f, _position.getZ() + .5f);
	sendToGL(id[1]);
}

void
Frog::update(float delta_t)
{

}
