#pragma once
#include "Camera.h"

class PerspectiveCamera :
	public Camera
{
public:
	PerspectiveCamera(float fovy, float aspect, float zNear, float zFar, MathLib* calc);
	~PerspectiveCamera();

	void update(float rAux, float alphaAux, float betaAux);
	void updateAt(float x, float y, float z);
	void computeProjectionMatrix();

private:
	float _fovy;
	float _aspect;
};

