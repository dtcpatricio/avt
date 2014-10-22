#pragma once
#include "DynamicObject.h"

class Frog :
	public DynamicObject
{
public:
	Frog(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* calc);
	~Frog();

	void draw();
	void create();
	void update(float delta_t);

private:
	int id[2];
};
