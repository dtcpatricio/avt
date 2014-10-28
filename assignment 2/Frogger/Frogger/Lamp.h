#pragma once
#include "StaticObject.h"
#include "LightSource.h"
class Lamp :
	public StaticObject
{
public:
	Lamp(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* calc, LightSource* l);
	~Lamp();

	void create();
	void draw();

private:
	int id[2];
	LightSource* lamp;
};

