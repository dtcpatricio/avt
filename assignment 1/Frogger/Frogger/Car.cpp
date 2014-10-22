#include "Car.h"


Car::Car(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* calc)
	: DynamicObject(mySurf, shader, calc)
{
	id[0] = 7;
	id[1] = 8;
}


Car::~Car()
{
}

void
Car::create()
{
	// wheels
	_mySurf->setObjId(id[0]);
	_mySurf->createTorus(0.1f, 0.3f, 16, 16);

	// body shop
	_mySurf->setObjId(id[1]);
	_mySurf->createRectangle(2.0f, 1.0f, 1.0f);
}

void
Car::draw()
{
	_calc->translation(_position.getX(), _position.getY(), _position.getZ());
	applyColor(1.f, 0.0f, 0.0f);
	sendToGL(id[1]);

	for (size_t i = 0; i < 4; i++)
	{
		switch (i)
		{
			// front left wheel
		case 0:
			_calc->translation(_position.getX() + .5f, _position.getY(), _position.getZ() + 1.1f);
			break;

			// rear left wheel
		case 1:
			_calc->translation(_position.getX() + 1.5f, _position.getY(), _position.getZ() + 1.1f);
			break;

			// front right wheel
		case 2:
			_calc->translation(_position.getX() + .5f, _position.getY(), _position.getZ() - .1f);
			break;

			// rar right wheel
		case 3:
			_calc->translation(_position.getX() + 1.5f, _position.getY(), _position.getZ() - .1f);
			break;
		}
		_calc->rotate(90.0f, 1.0f, 0.0f, 0.0f);
		applyColor(0.0f, 0.0f, 0.0f);
		sendToGL(id[0]);
	}
}

void
Car::update(float delta_t)
{

}
