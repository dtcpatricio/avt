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
	enum TEX_TYPE tex_needed() { return TEX_RIVER; }

private:
	int id = 0;
	int texCount = 0;
};

