#pragma once
#include "Vector3.h"

class BoundingBox
{
public:
	BoundingBox();
	~BoundingBox();

	void setLimits(Vector3 *pos);
	void setParams(float length, float width);
	
	bool is_colliding(BoundingBox *b);
	
	float getUpX() { return _upX; };
	float getBottomX() { return _bottomX; };
	float getUpZ() { return _upZ; };
	float getBottomZ() { return _bottomZ; };

private:
	float _upX, _upZ, _bottomX, _bottomZ;
	float _length, _width;
};

