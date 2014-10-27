#include "Riverside.h"


Riverside::Riverside(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* calc)
	: StaticObject(mySurf, shader, calc)
{
	amb[0] = .0215f; amb[1] = .1745f; amb[2] = .0215f; amb[3] = 1.f;
	diff[0] = .07568f; diff[1] = .61424f; diff[2] = .07568f; diff[3] = 1.f;
	spec[0] = .633f; spec[1] = .727811f; spec[2] = .633f; spec[3] = 1.f;
	emissive[0] = 0.f;  emissive[1] = 0.f; emissive[2] = 0.f; emissive[3] = 1.f;
	shininess = new float(0.6f * 128.f);
}


Riverside::~Riverside()
{
}

void
Riverside::create()
{
	_mySurf->setObjId(id);
	_mySurf->createCylinder(30.0f, 1.f, 4);
	applyColor();
}

void 
Riverside::draw()
{
	_mySurf->setObjMaterials(id, _shader);
	_calc->translation(_position.getX(), _position.getY(), _position.getZ());
	_calc->scale(1.0f, 1.0f, 2.f);
	_calc->rotate(90.0f, 0.0f, 0.0f, 1.0f);
	_calc->rotate(45.0f, 0.0f, 1.0f, 0.0f);
	sendToGL(id);
}
