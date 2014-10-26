#include "Frog.h"

Frog::Frog(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* calc)
	: DynamicObject(mySurf, shader, calc)
{
	id[0] = 9;
	id[1] = 10;
	diff[0] = .1f;         diff[1] = 1.f;          diff[2] = .3f;         diff[3] = 1.f;
	amb[0]  = .1f * 0.25f; amb[1]  = 1.f * 0.25f;  amb[2]  = .3f * 0.25f; amb[3]  = 1.f;
	spec[0] = .1f;         spec[1] = .1f;          spec[2] = .1f;         spec[3] = 1.f;
	emissive[0] = 0.f;  emissive[1] = 0.f; emissive[2] = 0.f; emissive[3] = 1.f;
	shininess = new float(100.f);
}

Frog::~Frog() {}

void
Frog::create()
{
	_mySurf->setObjId(id[0]);
	applyColor();
	_mySurf->createCube(1.0f);

	_mySurf->setObjId(id[1]);
	_mySurf->createSphere(.5f, 20);
	applyColor();
}

void
Frog::draw()
{
	applyColor();
	_mySurf->setObjMaterials(id[0], _shader);
	_calc->translation(_position.getX(), _position.getY(), _position.getZ());
	sendToGL(id[0]);

	_mySurf->setObjMaterials(id[1], _shader);
	_calc->translation(_position.getX() + .5f, _position.getY() + 1.5f, _position.getZ() + .5f);
	sendToGL(id[1]);
}

void
Frog::update(float delta_t)
{

}
