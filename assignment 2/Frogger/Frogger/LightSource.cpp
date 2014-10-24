#include "LightSource.h"


LightSource::LightSource(GLenum number)
	: _num(number)
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
LightSource::draw()
{

}

void
LightSource::setPosition(Vector4* v)
{
	_position.set(v->getX(), v->getY(), v->getZ(), v->getW());
}
