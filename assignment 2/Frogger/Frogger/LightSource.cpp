#include "LightSource.h"


LightSource::LightSource(enum LIGHT_TYPE number)
	: _num(number), _state(true)
{
}

LightSource::~LightSource()
{
}

void
LightSource::setAmbient(Vector4 *amb)
{
	memcpy(&_amb, amb, sizeof(Vector4));
}

void
LightSource::setDiffuse(Vector4 *diff)
{
	memcpy(&_diff, diff, sizeof(Vector4));
}

void
LightSource::setSpecular(Vector4 *spec)
{
	memcpy(&_spec, spec, sizeof(Vector4));
}

void
LightSource::setPosition(Vector4* v)
{
	_position.set(v->getX(), v->getY(), v->getZ(), v->getW());
}

void
LightSource::setDirection(Vector4 *dir)
{
	memcpy(&_dir, dir, sizeof(Vector4));
}

void
LightSource::draw()
{
}

bool
LightSource::setState(bool state){
	_state = state;
	return state;
}