#include "OrthogonalCamera.h"


OrthogonalCamera::OrthogonalCamera(float left, float right, float bottom, float top, 
			float near, float far, MathLib* calc)
	: Camera(near, far, calc), _left(left), _right(right), _top(top), _bottom(bottom)
{}


OrthogonalCamera::~OrthogonalCamera()
{}

void
OrthogonalCamera::computeProjectionMatrix()
{
	_calc->ortho(_left, _right, _bottom, _top, _near, _far);
}



