#include "GameObject.h"


GameObject::GameObject(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib *calc)
: _mySurf(mySurf), _shader(shader), _calc(calc)
{}


GameObject::~GameObject()
{
}

void
GameObject::sendToGL(int id)
{
	GLuint modelId = glGetUniformLocation(_shader->getProgramIndex(), "model");
	glUniformMatrix4fv(modelId, 1, false, _calc->getModel());
	_mySurf->simpleRender(id);
	_calc->setIdentityMatrix(_calc->getModel(), 4);
}

void
GameObject::applyColor(float r, float g, float b){
	GLuint colorId = glGetUniformLocation(_shader->getProgramIndex(), "in_color");
	glUniform4f(colorId, r, g, b, 1.0);
}
