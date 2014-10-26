#include "Road.h"

Road::Road(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* calc)
	: StaticObject(mySurf, shader, calc)
{
	diff[0] = .2f;        diff[1] = .2f;        diff[2] = .2f;        diff[3] = 1.f;
	amb[0]  = .2f * .25f; amb[1]  = .2f * .25f; amb[2]  = .2f * .25f; amb[3]  = 1.f;
	spec[0] = .0f;        spec[1] = .0f;        spec[2] = .0f;        spec[3] = 1.f;
	emissive[0] = 0.f;  emissive[1] = 0.f; emissive[2] = 0.f; emissive[3] = 1.f;
	shininess = new float(100.f);
}


Road::~Road()
{
}

void
Road::create()
{
	_mySurf->setObjId(id);
	applyColor();
	_mySurf->createRectangle(30.0f, 2.0f, 15.0f);
}

void 
Road::draw()
{
	_mySurf->setObjMaterials(id, _shader);
	_calc->translation(_position.getX(), _position.getY(), _position.getZ());
	sendToGL(id);
}
