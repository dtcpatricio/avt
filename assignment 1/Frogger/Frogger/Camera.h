#pragma once
#include "Entity.h"


class Camera :
	public Entity
{
public:
	Camera(float near, float far, MathLib* calc) : _near(near), _far(far), _calc(calc)
	{	};

	virtual ~Camera(){};

	void update();
	virtual void computeProjectionMatrix() = 0;
	void computeVisualizationMatrix();

protected:
	float _near;
	float _far;
	Vector3 _up;
	Vector3 _at;
	MathLib* _calc;
};

