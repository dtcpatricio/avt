#include "Riverside.h"


Riverside::Riverside(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* calc)
	: StaticObject(mySurf, shader, calc)
{
}


Riverside::~Riverside()
{
}

void
Riverside::create()
{
	_mySurf->setObjId(id);
	_mySurf->createRectangle(30.0f, 2.0f, 4.0f);
}

void 
Riverside::draw()
{
	applyColor(0.0f, 1.0f, 0.0f);
	_calc->translation(_position.getX(), _position.getY(), _position.getZ());
	sendToGL(id);
}
