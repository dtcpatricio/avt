#pragma once
#include "GameObject.h"

class DynamicObject :
	public GameObject
{
public:
	DynamicObject(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* calc);
	~DynamicObject();

	void update(float delta_t);
	void setSpeed(Vector3* speed);
	void setSpeed(float x, float y, float z);
	Vector3 getSpeed();
	
	virtual void create() = 0;

protected:
	Vector3 _speed;
};
