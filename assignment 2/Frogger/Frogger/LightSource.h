#pragma once
#include "Vector4.h"
#include <GL/glut.h>

class LightSource
{
public:
	LightSource(GLenum number);
	~LightSource();

	bool getState() { return _state; };
	bool setState(bool state);
	GLenum getNum() { return _num; };
	void setPosition(Vector4 *pos);
	Vector4* getPosition() { return &_position; };
	void setDirection(Vector3 *dir);
	void setCutOff(float cut_off) { _cut_off = cut_off;	};
	void setExponent(float exp) { _exponent = exp; };
	void setAmbient(Vector4 *amb);
	void setDiffuse(Vector4 *diff);
	void setSpecular(Vector4 *spec);
	Vector4 getAmbient() { return _amb; };
	Vector4 getDiffuse() { return _diff; };
	Vector4 getSpecular() { return _spec; };
	float getShininess() { return _exponent; };
	void draw();

private:
	Vector4 _amb;
	Vector4 _diff;
	Vector4 _spec;
	Vector4 _position;
	Vector3 _direction;
	float _cut_off;
	float _exponent;
	GLenum _num;
	bool _state;
};

