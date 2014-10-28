#pragma once
#include "StaticObject.h"

class Riverside :
	public StaticObject
{
public:
	Riverside(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* calc);
	~Riverside();

	void create();
	void draw();
	enum TEX_TYPE tex_needed() { return TEX_NONE; }

private:
	int id = 1;
	int texCount = 0;
};

