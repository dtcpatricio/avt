#include "TimberLog.h"


TimberLog::TimberLog(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* calc)
	: DynamicObject(mySurf, shader, calc)
{
	amb[0] = .2125f; amb[1] = .1275f; amb[2] = .054f; amb[3] = 1.f;
	diff[0] = .714f; diff[1] = .4284f; diff[2] = .18144f; diff[3] = 1.f;
	spec[0] = .393548f; spec[1] = .271906f; spec[2] = .166721f; spec[3] = 1.f;
	emissive[0] = 0.f;  emissive[1] = 0.f; emissive[2] = 0.f; emissive[3] = 1.f;
	shininess = new float(0.2f * 128.f);
}


TimberLog::~TimberLog()
{
}

void
TimberLog::create()
{
	_mySurf->setObjId(id);
	_mySurf->createCylinder(5.0f, 1.2f, 32);
	applyColor();
}


void
TimberLog::draw()
{
	_mySurf->setObjMaterials(id, _shader);
	_calc->translation(_position.getX(), _position.getY(), _position.getZ());
	_calc->rotate(90.0f, 0.0f, 0.0f, 1.0f);
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
