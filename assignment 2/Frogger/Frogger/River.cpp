#include "River.h"

River::River(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* calc)
	: StaticObject(mySurf, shader, calc)
{
	amb[0] = 0.0f; amb[1] = 0.0f; amb[2] = .8f; amb[3] = 1.f;
	diff[0] = .1f; diff[1] = 0.1f; diff[2] = .9f; diff[3] = 1.f;
	spec[0] = 0.04f; spec[1] = 0.04f; spec[2] = .9f; spec[3] = 1.f;
	emissive[0] = 0.f;  emissive[1] = 0.f; emissive[2] = 0.f; emissive[3] = 1.f;
	shininess = new float(100.f);
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

