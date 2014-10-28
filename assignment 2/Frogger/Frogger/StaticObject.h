#pragma once
#include "GameObject.h"

class StaticObject :
	public GameObject
{
public:
	StaticObject(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* calc);
	~StaticObject();

	virtual void create() = 0;
	virtual void draw() = 0;
	virtual enum TEX_TYPE tex_needed() = 0;
};