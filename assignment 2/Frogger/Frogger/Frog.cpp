#include "Frog.h"

Frog::Frog(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* calc)
	: DynamicObject(mySurf, shader, calc)
{
	id[0] = 8;
	id[1] = 9;

	amb[0] = .0f; amb[1] = .7f; amb[2] = 0.0f; amb[3] = 1.f;
	diff[0] = .1f; diff[1] = .9f; diff[2] = .1f; diff[3] = 1.f;
	spec[0] = .04f; spec[1] = .9f; spec[2] = .04f; spec[3] = 1.f;
	emissive[0] = 0.f;  emissive[1] = 0.f; emissive[2] = 0.f; emissive[3] = 1.f;
	shininess = new float(180.f);
}

Frog::~Frog() {}

void
Frog::create()
{
	_mySurf->setObjId(id[0]);
	_mySurf->createCylinder(.5f, .75f, 4);
	applyColor();

	_mySurf->setObjId(id[1]);
	_mySurf->createSphere(.25f, 64);
	applyColor();
}

void
Frog::draw()
{
	_mySurf->setObjMaterials(id[0], _shader);
	_calc->translation(_position.getX(), _position.getY(), _position.getZ());
	_calc->scale(1.5f, .9f, .6f);
	_calc->rotate(90.0f, 0.0f, 0.0f, 1.0f);
	_calc->rotate(45.0f, 0.0f, 1.0f, 0.0f);
	sendToGL(id[0]);

	_mySurf->setObjMaterials(id[1], _shader);
	_calc->translation(_position.getX(), _position.getY(), _position.getZ() - .45f);
	sendToGL(id[1]);
}

void
Frog::update(float delta_t)
{

}
