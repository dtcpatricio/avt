#include "Bus.h"


Bus::Bus(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* calc)
	: DynamicObject(mySurf, shader, calc)
{
	id[0] = 5;
	id[1] = 6;
}


Bus::~Bus()
{
}

void
Bus::create()
{
	// wheels
	_mySurf->setObjId(id[0]);
	_mySurf->createTorus(0.1f, 0.5f, 16, 16);

	// body shop
	_mySurf->setObjId(id[1]);
	_mySurf->createRectangle(3.0f, 2.0f, 2.0f);
}

void
Bus::draw()
{
	_calc->translation(_position.getX(), _position.getY(), _position.getZ());
	applyColor(1.f, 1.f, 0.0f);
	sendToGL(id[1]);

	for (size_t i = 0; i < 4; i++)
	{
		switch (i)
		{
			// front left wheel
		case 0:
			_calc->translation(_position.getX() + .8f, _position.getY(), _position.getZ() + 2.2f);
			break;

			// rear left wheel
		case 1:
			_calc->translation(_position.getX() + 2.2f, _position.getY(), _position.getZ() + 2.2f);
			break;

			// front right wheel
		case 2:
			_calc->translation(_position.getX() + .8f, _position.getY(), _position.getZ() - .2f);
			break;

			// rar right wheel
		case 3:
			_calc->translation(_position.getX() + 2.2f, _position.getY(), _position.getZ() - .2f);
			break;
		}
		_calc->rotate(90.0f, 1.0f, 0.0f, 0.0f);
		applyColor(1.f, 0.01f, 0.01f);
		sendToGL(id[0]);
	}
}

void
Bus::update(float delta_t)
{

}
