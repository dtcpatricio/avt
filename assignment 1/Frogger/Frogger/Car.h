#pragma once
#include "DynamicObject.h"

class Car :
	public DynamicObject
{
public:
	Car(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* calc);
	~Car();

	void draw();
	void create();
	void update(float delta_t);

private:
	int id[2];
};

