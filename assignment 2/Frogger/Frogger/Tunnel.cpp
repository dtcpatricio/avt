#include "Tunnel.h"


Tunnel::Tunnel(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* calc)
	: StaticObject(mySurf, shader, calc)
{
	amb[0] = .0f; amb[1] = .0f; amb[2] = .0f; amb[3] = 1.0f;
	diff[0] = .1f; diff[1] = .2f; diff[2] = .2f; diff[3] = 1.0f;
	spec[0] = .1f; spec[1] = .2f; spec[2] = .1f; spec[3] = 1.0f;
	emissive[0] = 0.f;  emissive[1] = 0.f; emissive[2] = 0.f; emissive[3] = 1.f;
	shininess = new float(12.f * 128.f);
}


Tunnel::~Tunnel()
{
}

void
Tunnel::create()
{
	_mySurf->setObjId(id);
	_mySurf->createCylinder(6.f, 7.5f, 20);
	applyColor();
}

void
Tunnel::draw()
{

	_mySurf->setObjMaterials(id, _shader);
	_calc->setIdentityMatrix(_calc->getModel(), 4);
	_calc->translation(_position.getX(), _position.getY(), _position.getZ());
	_calc->rotate(90.0f, 0.0f, 0.0f, 1.0f);
	_calc->rotate(45.0f, 0.0f, 1.0f, 0.0f);
	sendToGL(id);

}
