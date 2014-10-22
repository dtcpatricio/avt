#include "Roadside.h"


Roadside::Roadside(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* calc)
	: StaticObject(mySurf, shader, calc)
{
}


Roadside::~Roadside()
{
}

void
Roadside::create()
{
	_mySurf->setObjId(id);
	_mySurf->createRectangle(30.0f, 2.0f, 4.0f);
}

void 
Roadside::draw(){
	_calc->translation(_position.getX(), _position.getY(), _position.getZ());
	applyColor(0.98f, 0.625f, 0.12f);
	sendToGL(id);
}
