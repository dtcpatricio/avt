#include "PerspectiveCamera.h"
#include <math.h>

PerspectiveCamera::PerspectiveCamera(float fovy, float aspect, float zNear, float zFar, MathLib* calc)
	: Camera(zNear, zFar, calc), _fovy(fovy), _aspect(aspect)
{}


PerspectiveCamera::~PerspectiveCamera()
{}

void
PerspectiveCamera::updateUp(){
	_up.set(0.0f, 1.0f, 0.0f);
}

void
PerspectiveCamera::updateEye(float rAux, float alphaAux, float betaAux, Vector3 *frogp)
{
	float x = rAux * sin(alphaAux * 3.14f / 180.0f) * cos(betaAux * 3.14f / 180.0f);
	float z = rAux * cos(alphaAux * 3.14f / 180.0f) * cos(betaAux * 3.14f / 180.0f);
	float y = rAux *   						      sin(betaAux * 3.14f / 180.0f);
	setPosition(frogp->getX() + x, frogp->getY() + y, frogp->getZ() + z);
}

void
PerspectiveCamera::updateAt(Vector3 *v)
{
	_at.set(v->getX(), v->getY(), v->getZ());
}

void
PerspectiveCamera::computeProjectionMatrix()
{
	_calc->perspective(_fovy, _aspect, _near, _far);
}


