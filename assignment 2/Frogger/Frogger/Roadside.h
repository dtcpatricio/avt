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
	enum TEX_TYPE tex_needed() { return TEX_NONE; }

private:
	int id = 3;
	int texCount = 0;
};

