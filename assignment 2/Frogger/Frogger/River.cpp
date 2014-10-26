#include "River.h"

River::River(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* calc)
	: StaticObject(mySurf, shader, calc)
{
	diff[0] = .1f;        diff[1] = .1f;        diff[2] = .9f;        diff[3] = 1.f;
	amb[0]  = .1f * .25f; amb[1]  = .1f * .25f; amb[2]  = .9f * .25f; amb[3]  = 1.f;
	spec[0] = .1f;        spec[1] = .1f;        spec[2] = .9f;        spec[3] = 1.f;
	emissive[0] = 0.f;  emissive[1] = 0.f; emissive[2] = 0.f; emissive[3] = 1.f;
	shininess = new float(180.f);
}


River::~River()
{
}

void
River::create()
{
	_mySurf->setObjId(id);
	applyColor();
	_mySurf->createRectangle(30.0f, 2.0f, 15.0f);
}

void
River::draw()
{
	_mySurf->setObjMaterials(id, _shader);
	_calc->setIdentityMatrix(_calc->getModel(), 4);
	_calc->translation(_position.getX(), _position.getY(), _position.getZ());
	sendToGL(id);
}

