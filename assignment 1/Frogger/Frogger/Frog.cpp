#include "Frog.h"

Frog::Frog(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* calc)
	: DynamicObject(mySurf, shader, calc)
{
	id[0] = 9;
	id[1] = 10;
}

Frog::~Frog() {}

void
Frog::create()
{
	_mySurf->setObjId(id[0]);
	_mySurf->createCube(1.0f);

	_mySurf->setObjId(id[1]);
	_mySurf->createSphere(.5f, 16);
}

void
Frog::draw()
{
	_calc->translation(_position.getX(), _position.getY(), _position.getZ());
	applyColor(0.0f, 1.0f, .3f);
	sendToGL(id[0]);
	_calc->translation(_position.getX() + .5f, _position.getY() + 1.5f, _position.getZ() + .5f);
	applyColor(0.0f, .0f, 1.0f);
	sendToGL(id[1]);
}

void
Frog::update(float delta_t)
{

}
