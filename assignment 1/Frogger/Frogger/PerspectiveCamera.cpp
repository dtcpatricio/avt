#include "PerspectiveCamera.h"
#include <math.h>

PerspectiveCamera::PerspectiveCamera(float fovy, float aspect, float zNear, float zFar, MathLib* calc)
	: Camera(zNear, zFar, calc), _fovy(fovy), _aspect(aspect)
{}


PerspectiveCamera::~PerspectiveCamera()
{}

void
PerspectiveCamera::update(float rAux, float alphaAux, float betaAux)
{
	float x = rAux * sin(alphaAux * 3.14f / 180.0f) * cos(betaAux * 3.14f / 180.0f);
	float z = rAux * cos(alphaAux * 3.14f / 180.0f) * cos(betaAux * 3.14f / 180.0f);
	float y = rAux *   						      sin(betaAux * 3.14f / 180.0f);
	setPosition(x, y, z);
}

void
PerspectiveCamera::updateAt(float x, float y, float z)
{
	_at.set(x, y, z);
}

void
PerspectiveCamera::computeProjectionMatrix()
{
	_calc->perspective(_fovy, _aspect, _near, _far);
}


