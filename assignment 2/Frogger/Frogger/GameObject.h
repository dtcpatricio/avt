#pragma once
#include "Entity.h"
#include "vsResSurfRevLib.h"

class GameObject :
	public Entity
{
public:
	GameObject(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* _calc);
	~GameObject();

	void sendToGL(int id);
	void applyColor(float r, float g, float b);
	void applyColor();

	virtual void draw() = 0;
	virtual void create() = 0;

protected:
	VSResSurfRevLib * _mySurf;
	VSShaderLib * _shader;
	MathLib *_calc;

	float amb[4];
	float diff[4];
	float spec[4];
	float emissive[4];
	float *shininess;
};

