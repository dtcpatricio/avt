#pragma once
#include "StaticObject.h"
class Tunnel :
	public StaticObject
{
public:
	Tunnel(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* calc);
	~Tunnel();

	void create();
	void draw();

private:
	int id = 15;
	
};

