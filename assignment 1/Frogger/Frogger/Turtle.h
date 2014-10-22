#pragma once
#include "DynamicObject.h"

class Turtle :
	public DynamicObject
{
public:
	Turtle(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* calc);
	~Turtle();

	void draw();
	void create();
	void update(float delta_t);

private:
	int id[2];
};

