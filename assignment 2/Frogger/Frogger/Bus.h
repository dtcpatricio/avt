#pragma once
#include "DynamicObject.h"

class Bus :
	public DynamicObject
{
public:
	Bus(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* calc);
	~Bus();

	void draw();
	void create();
	void update(float delta_t);

private:
	int id[2];
	float shininess = 100.0f;
	int texcount = 0;
};

