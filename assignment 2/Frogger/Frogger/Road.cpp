#include "Road.h"

Road::Road(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* calc)
	: StaticObject(mySurf, shader, calc)
{
	amb[0] = .2f; amb[1] = .15f; amb[2] = .1f; amb[3] = 1.f;
	diff[0] = .8f; diff[1] = .6f; diff[2] = .4f; diff[3] = 1.f;
	spec[0] = .8f; spec[1] = .8f; spec[2] = .8f; spec[3] = 1.f;
	emissive[0] = 0.f;  emissive[1] = 0.f; emissive[2] = 0.f; emissive[3] = 1.f;
	shininess = new float(100.f);
}


Road::~Road()
{
}

void
Road::create()
{
	applyColor();
	_mySurf->setObjId(id);
	_mySurf->createRectangle(30.0f, 2.0f, 15.0f);
}

void 
Road::draw()
{
	_mySurf->setObjMaterials(id, _shader);
	_calc->translation(_position.getX(), _position.getY(), _position.getZ());
	sendToGL(id);
}
