#include "Camera.h"

void
Camera::update()
{
	_up.set(0.0f, 0.0f, -1.0f);
	_at.set(0.0f, 0.0f, 0.0f);
}

void
Camera::computeVisualizationMatrix()
{
	_calc->lookAt(_position.getX(), _position.getY(), _position.getZ(),
		_at.getX(), _at.getY(), _at.getZ(),
		_up.getX(), _up.getY(), _up.getZ());
}
