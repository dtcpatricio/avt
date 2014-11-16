#pragma once
#include "DynamicObject.h"

#define frand()			((float)rand()/RAND_MAX)
#define M_PI			3.14159265

class Firework :
	public DynamicObject
{
public:
	Firework(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* calc);
	~Firework();

	void create();
	void draw();
	void update(float delta_t);

	float getLife(){ return _life;}
	float getFade(){ return _fade;}
	Vector3* getAcel(){ return new Vector3(ax, ay, az); }
	void setLife(float l){ _life = l; }
	void setFade(float f){ _fade = f; }
	void setColors(Vector3* color){ 
		_r = color->getX();
		_g = color->getY();
		_b = color->getZ();
	}
	void setAcel(float x, float y, float z){
		ax = x;
		ay = y;
		az = z;
	}

private:
	int id = 23;
	float	_life;		// vida
	float	_fade;		// fade
	float _r, _g, _b;		// color
	float ax, ay, az;		// aceleração
};

