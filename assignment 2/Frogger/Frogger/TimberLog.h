#pragma once
#include "DynamicObject.h"

class TimberLog :
	public DynamicObject
{
public:
	TimberLog(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* calc);
	~TimberLog();

	void draw();
	void create();
	void update(float delta_t);

private:
	int id = 11;
};

