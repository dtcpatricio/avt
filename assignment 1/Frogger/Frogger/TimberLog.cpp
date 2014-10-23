#include "TimberLog.h"


TimberLog::TimberLog(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* calc)
	: DynamicObject(mySurf, shader, calc)
{
}


TimberLog::~TimberLog()
{
}

void
TimberLog::create()
{
	_mySurf->setObjId(id);
	_mySurf->createCylinder(4.0f, 0.75f, 16);
}

void
TimberLog::create(float length)
{
	_mySurf->setObjId(id);
	_mySurf->createCylinder(length, 0.75f, 16);
}

void
TimberLog::draw()
{
	_calc->translation(_position.getX(), _position.getY(), _position.getZ());
	_calc->rotate(90.0f, 0.0f, 0.0f, 1.0f);
	applyColor(0.6f, 0.4f, 0.12f);
	sendToGL(id);
}

void
TimberLog::update(float delta_t)
{
	float deltaX = _position.getX() - delta_t;

	if (deltaX <= -12.75f)
		setPosition(12.75f, _position.getY(), _position.getZ());
	else
		setPosition(deltaX , _position.getY(), _position.getZ());
}
