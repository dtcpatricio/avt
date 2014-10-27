#include "Roadside.h"


Roadside::Roadside(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* calc)
	: StaticObject(mySurf, shader, calc)
{
	amb[0] = .25f; amb[1] = .25f; amb[2] = .25f; amb[3] = 1.f;
	diff[0] = .4f; diff[1] = .4f; diff[2] = .4f; diff[3] = 1.f;
	spec[0] = .774597f; spec[1] = .774597f; spec[2] = .774597f; spec[3] = 1.f;
	emissive[0] = 0.f;  emissive[1] = 0.f; emissive[2] = 0.f; emissive[3] = 1.f;
	shininess = new float(0.6f * 128.f);
}


Roadside::~Roadside()
{
}

void
Roadside::create()
{
	_mySurf->setObjId(id);
	_mySurf->createCylinder(30.0f, 1.f, 4);
	applyColor();
}

void 
Roadside::draw()
{
	_mySurf->setObjMaterials(id, _shader);
	_calc->translation(_position.getX(), _position.getY(), _position.getZ());
	_calc->scale(1.0f, 1.0f, 3.f);
	_calc->rotate(90.0f, 0.0f, 0.0f, 1.0f);
	_calc->rotate(45.0f, 0.0f, 1.0f, 0.0f);
	sendToGL(id);
}
