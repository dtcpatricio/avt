#pragma once
#include "StaticObject.h"

class Road :
	public StaticObject
{
public:
	Road(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* calc);
	~Road();

	void create();
	void draw();

private:
	int id = 2;
	int texCount = 0;
};

