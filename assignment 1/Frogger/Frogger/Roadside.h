#pragma once
#include "StaticObject.h"

class Roadside :
	public StaticObject
{
public:
	Roadside(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* calc);
	~Roadside();

	void create();
	void draw();

private:
	int id = 3;
};

