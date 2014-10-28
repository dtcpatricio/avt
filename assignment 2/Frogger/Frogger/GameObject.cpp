#include "GameObject.h"


GameObject::GameObject(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib *calc)
: _mySurf(mySurf), _shader(shader), _calc(calc)
{
	_boundingBox = new BoundingBox();
}


GameObject::~GameObject()
{
}

void
GameObject::sendToGL(int id)
{
	GLuint modelId = glGetUniformLocation(_shader->getProgramIndex(), "model");
	glUniformMatrix4fv(modelId, 1, false, _calc->getModel());
	GLuint normal_uniformId = glGetUniformLocation(_shader->getProgramIndex(), "m_normal");
	glUniformMatrix3fv(normal_uniformId, 1, false, _calc->computeNormalMatrix3x3());
	_mySurf->simpleRender(id);
	_calc->setIdentityMatrix(_calc->getModel(), 4);
}

//void
//GameObject::applyColor(float r, float g, float b){
//	GLuint colorId = glGetUniformLocation(_shader->getProgramIndex(), "in_color");
//	glUniform4f(colorId, r, g, b, 1.0);
//}

void
GameObject::applyColor()
{
	_mySurf->setColor(VSResourceLib::MaterialSemantics::AMBIENT, amb);
	_mySurf->setColor(VSResourceLib::MaterialSemantics::DIFFUSE, diff);
	_mySurf->setColor(VSResourceLib::MaterialSemantics::SPECULAR, spec);
	_mySurf->setColor(VSResourceLib::MaterialSemantics::EMISSIVE, emissive);
	_mySurf->setColor(VSResourceLib::MaterialSemantics::SHININESS, shininess);
}
