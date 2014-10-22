#include "Road.h"

Road::Road(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* calc)
	: StaticObject(mySurf, shader, calc)
{
}


Road::~Road()
{
}

void
Road::create()
{
	_mySurf->setObjId(id);
	_mySurf->createRectangle(30.0f, 2.0f, 15.0f);
}

void 
Road::draw()
{
	_calc->translation(_position.getX(), _position.getY(), _position.getZ());
	applyColor(.0f, 0.0f, 0.11f);
	sendToGL(id);
}
