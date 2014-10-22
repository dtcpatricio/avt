#include "River.h"

River::River(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* calc)
	: StaticObject(mySurf, shader, calc)
{
}


River::~River()
{
}

void
River::create()
{
	_mySurf->setObjId(id);
	_mySurf->createRectangle(30.0f, 2.0f, 15.0f);
}

void
River::draw(){
	_calc->setIdentityMatrix(_calc->getModel(), 4);
	_calc->translation(_position.getX(), _position.getY(), _position.getZ());
	applyColor(0.0f, 0.0f, 1.0f);
	sendToGL(id);
}

