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
	void setTires();
	void setHead();
	
	virtual void create() = 0;
	enum TEX_TYPE tex_needed() { return TEX_NONE; } // TODO: Will change in the future

protected:
	Vector3 _speed;
};
