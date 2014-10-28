#include "Lamp.h"


Lamp::Lamp(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* calc, LightSource* l)
: StaticObject(mySurf, shader, calc), lamp(l)
{
	id[0] = 13;
	id[1] = 14;

	amb[0] = .19255f; amb[1] = .19255f; amb[2] = .19255f; amb[3] = 1.f;
	diff[0] = 0.50754f; diff[1] = 0.50754f; diff[2] = 0.50754f; diff[3] = 1.f;
	spec[0] = 0.508273f; spec[1] = 0.508273f; spec[2] = 0.508273; spec[3] = 1.f;
	emissive[0] = 0.f;  emissive[1] = 0.f; emissive[2] = 0.f; emissive[3] = 1.f;
	shininess = new float(0.4f * 128.f);

	float x = l->getPosition()->getX();
	float y = l->getPosition()->getY();
	float z = l->getPosition()->getZ();

	_position.set(x, y, z);
}


Lamp::~Lamp()
{
}

void
Lamp::create()
{
	_mySurf->setObjId(id[0]);
	_mySurf->createSphere(1.f, 20);
	applyColor();

	_mySurf->setObjId(id[1]);
	_mySurf->createCylinder(4.0f, 1.f, 20);
	applyColor();

}

void
Lamp::draw()
{
	_mySurf->setObjMaterials(id[0], _shader);
	_calc->setIdentityMatrix(_calc->getModel(), 4);
	_calc->translation(_position.getX(), _position.getY(), _position.getZ());
	sendToGL(id[0]);

	_mySurf->setObjMaterials(id[1], _shader);
	_calc->setIdentityMatrix(_calc->getModel(), 4);
	_calc->translation(_position.getX(), 1.5f, _position.getZ());
	sendToGL(id[1]);

	
}
