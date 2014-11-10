#include "Tree.h"


Tree::Tree(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* calc)
	: StaticObject(mySurf, shader, calc)
{
	amb[0] = 0.1f; amb[1] = 0.5f; amb[2] = 0.1f; amb[3] = 1.f;
	diff[0] = .1f; diff[1] = .1f; diff[2] = .1f; diff[3] = 1.f;
	spec[0] = .1f; spec[1] = .1f; spec[2] = .2f; spec[3] = 1.f;
	emissive[0] = 0.f;  emissive[1] = 0.f; emissive[2] = 0.f; emissive[3] = 1.f;
	shininess = new float(100.f);
}


Tree::~Tree()
{
}

void
Tree::create()
{
	_mySurf->setObjId(id);
	_mySurf->createRectangle(10.0f, 10.0f);
	applyColor();
	
}

void
Tree::draw()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);

	_mySurf->setObjMaterials(id, _shader);
	_calc->setIdentityMatrix(_calc->getModel(), 4);
	_calc->translation(_position.getX(), _position.getY(), _position.getZ());
	_calc->rotate(45, 0.0f, 1.0f, 0.0f);
	sendToGL(id);

	glDisable(GL_BLEND);

}

