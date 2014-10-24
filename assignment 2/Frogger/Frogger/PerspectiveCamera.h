#pragma once
#include "Camera.h"

class PerspectiveCamera :
	public Camera
{
public:
	PerspectiveCamera(float fovy, float aspect, float zNear, float zFar, MathLib* calc);
	~PerspectiveCamera();

	void updateUp();
	void updateEye(float rAux, float alphaAux, float betaAux, Vector3 *frogp);
	void updateAt(Vector3 *v);
	void computeProjectionMatrix();

private:
	float _fovy;
	float _aspect;
};

