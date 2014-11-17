#pragma once
#include "Tree.h"
class Stencil :
	public StaticObject
{
public:
	Stencil(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* calc);
	~Stencil();

	void create();
	void draw();

private:
	int id = 111;
};