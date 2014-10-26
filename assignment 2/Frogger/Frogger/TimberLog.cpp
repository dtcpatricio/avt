#include "TimberLog.h"


TimberLog::TimberLog(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* calc)
	: DynamicObject(mySurf, shader, calc)
{
	diff[0] = .5f;        diff[1] = .3f;        diff[2] = .1f;        diff[3] = 1.f;
	amb[0]  = .5f * .25f; amb[1]  = .3f * .25f; amb[2]  = .1f * .25f; amb[3]  = 1.f;
	spec[0] = .1f;        spec[1] = .1f;        spec[2] = .05f;       spec[3] = 1.f;
	emissive[0] = 0.f;  emissive[1] = 0.f; emissive[2] = 0.f; emissive[3] = 1.f;
	shininess = new float(100.f);
}


TimberLog::~TimberLog()
{
}

void
TimberLog::create()
{
	_mySurf->setObjId(id);
	_mySurf->createCylinder(4.0f, 0.75f, 32);
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
