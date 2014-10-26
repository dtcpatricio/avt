#pragma once
#include "Vector4.h"
#include <GL/glut.h>

enum LIGHT_TYPE { POINT_LIGHT, DIR_LIGHT, SPOT_LIGHT };

class LightSource
{
public:
	LightSource(enum LIGHT_TYPE number);
	~LightSource();
	
	bool getState() { return _state; }
	enum LIGHT_TYPE getNum() { return _num; }

	Vector4  getAmbient()   { return _amb; }
	Vector4  getDiffuse()   { return _diff; }
	Vector4  getSpecular()  { return _spec; }
	float    getShininess() { return _exponent; }
	Vector4* getPosition()  { return &_position; }
	Vector4  getDirection() { return _dir; }
	float    getCutOff()    { return _cut_off; }

	bool setState(bool state);

	void setAmbient  (Vector4 *amb);
	void setDiffuse  (Vector4 *diff);
	void setSpecular (Vector4 *spec);
	void setPosition (Vector4 *pos);
	void setDirection(Vector4 *dir);
	void setExponent (float exp)     { _exponent = exp; }
	void setCutOff   (float cut_off) { _cut_off  = cut_off; }

	void draw();

private:
	Vector4 _amb, _diff, _spec, _position;
	Vector4 _dir;   // Used in spotlight
	float _cut_off; // Used in spotlight
	float _exponent;
	enum LIGHT_TYPE _num;
	bool _state;
};

