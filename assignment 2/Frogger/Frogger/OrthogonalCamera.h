#pragma once
#include "Camera.h"

class OrthogonalCamera :
	public Camera
{
public:
	OrthogonalCamera(float left, float right, float top, float bottom, float near, float far, MathLib* calc);
	~OrthogonalCamera();

	void computeProjectionMatrix();

private:
	float _left;
	float _right;
	float _top;
	float _bottom;
};

