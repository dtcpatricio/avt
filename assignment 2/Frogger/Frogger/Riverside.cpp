#include "Riverside.h"


Riverside::Riverside(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* calc)
	: StaticObject(mySurf, shader, calc)
{
	amb[0] = .1f; amb[1] = .8f; amb[2] = .1f; amb[3] = 1.f;
	diff[0] = .2f; diff[1] = .6f; diff[2] = .2f; diff[3] = 1.f;
	spec[0] = .2f; spec[1] = .8f; spec[2] = .2f; spec[3] = 1.f;
	emissive[0] = 0.f;  emissive[1] = 0.f; emissive[2] = 0.f; emissive[3] = 1.f;
	shininess = new float(100.f);
}


Riverside::~Riverside()
{
}

void
Riverside::create()
{
	_mySurf->setObjId(id);
	applyColor();
	_mySurf->createRectangle(30.0f, 2.0f, 4.0f);
}

void 
Riverside::draw()
{
	_mySurf->setObjMaterials(id, _shader);
	_calc->translation(_position.getX(), _position.getY(), _position.getZ());
	sendToGL(id);
}
