#include "BoundingBox.h"


BoundingBox::BoundingBox()
{
}

BoundingBox::~BoundingBox()
{
}

void
BoundingBox::setLimits(Vector3* pos)
{
	_upX = pos->getX() - _length / 2;
	_bottomX = pos->getX() + _length / 2;
	_upZ = pos->getZ() - _width / 2;
	_bottomZ = pos->getZ() + _width / 2;
}

void
BoundingBox::setParams(float len, float wid)
{
	_length = len;
	_width = wid;
}

bool
BoundingBox::is_colliding(BoundingBox *bb)
{
	float BupX = bb->getUpX();
	float BbotX = bb->getBottomX();
	float BupZ = bb->getUpZ();
	float BbotZ = bb->getBottomZ();
	return !(_bottomX < BupX 
		|| BbotX < _upX
		|| _bottomZ < BupZ 
		|| BbotZ < _upZ);
}
