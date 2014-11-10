#pragma once
#include "StaticObject.h"
class Tree :
	public StaticObject
{
public:
	Tree(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* calc);
	~Tree();

	void create();
	void draw();

private:
	int id=16;
};

