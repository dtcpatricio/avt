#include "River.h"

River::River(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* calc)
	: StaticObject(mySurf, shader, calc)
{
	//0.1	0.18725	0.1745	0.396	0.74151	0.69102	0.297254	0.30829	0.306678	0.1
	amb[0] = 0.0f; amb[1] = 0.0f; amb[2] = .5f; amb[3] = 1.f;
	diff[0] = .1f; diff[1] = 0.1f; diff[2] = .9f; diff[3] = 1.f;
	spec[0] = 0.04f; spec[1] = 0.04f; spec[2] = .9f; spec[3] = 1.f;
	emissive[0] = 0.f;  emissive[1] = 0.f; emissive[2] = 0.f; emissive[3] = 1.f;
	shininess = new float(12.f);
}


River::~River()
{
}

void
River::create()
{
	_mySurf->setObjId(id);
	_mySurf->createCylinder(30.0f, 5.f, 4);
	applyColor();
}

void
River::draw()
{
	_mySurf->setObjMaterials(id, _shader);
	_calc->setIdentityMatrix(_calc->getModel(), 4);
	_calc->translation(_position.getX(), _position.getY(), _position.getZ());
	_calc->scale(1.0f, .2f, 2.1f);
	_calc->rotate(90.0f, 0.0f, 0.0f, 1.0f);
	_calc->rotate(45.0f, 0.0f, 1.0f, 0.0f);
	sendToGL(id);
}

