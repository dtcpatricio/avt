#pragma once
#include "StaticObject.h"

class River :
	public StaticObject
{
public:
	River(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* calc);
	~River();

	void create();
	void draw();

private:
	int id = 0;
};

