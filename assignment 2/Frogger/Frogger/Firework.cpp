#include "Firework.h"


Firework::Firework(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* calc)
: DynamicObject(mySurf, shader, calc)
{
	amb[0] = 0.0f; amb[1] = 0.0f; amb[2] = 1.f; amb[3] = 1.f;
	diff[0] = .1f; diff[1] = 0.1f; diff[2] = .9f; diff[3] = .4f;
	spec[0] = 0.04f; spec[1] = 0.04f; spec[2] = .9f; spec[3] = 1.f;
	emissive[0] = 0.f;  emissive[1] = 0.f; emissive[2] = 0.f; emissive[3] = 1.f;
	shininess = new float(12.f * 128.f);
}


Firework::~Firework()
{
}

void
Firework::create()
{
	_mySurf->setObjId(id);
	_mySurf->createRectangle(1.0f, 1.0f);
	applyColor();
}

void
Firework::draw()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	_mySurf->setObjMaterials(id, _shader);
	_calc->translation(_position.getX(), _position.getY(), _position.getZ());
	sendToGL(id);

	glDisable(GL_BLEND);
}

