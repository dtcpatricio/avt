#include "Bus.h"


Bus::Bus(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* calc)
	: DynamicObject(mySurf, shader, calc)
{					
	id[0] = 4;	id[1] = 5;
	amb[0] = 0.24725f; amb[1] = 0.1995f; amb[2] = 0.0745f; amb[3] = 1.f;
	diff[0] = 0.75164f; diff[1] = 0.60648; diff[2] = 0.22648f; diff[3] = 1.f;
	spec[0] = 0.628281f; spec[1] = 0.555802f; spec[2] = 0.366065f; spec[3] = 1.f;
	emissive[0] = 0.f;  emissive[1] = 0.f; emissive[2] = 0.f; emissive[3] = 1.f;
	shininess = new float(52.f);
}


Bus::~Bus()
{
}

void
Bus::create()
{
	// body shop
	_mySurf->setObjId(id[1]);
	_mySurf->createRectangle(3.0f, 2.0f, 2.0f);
	applyColor();

	setTires();

	// wheels
	_mySurf->setObjId(id[0]);
	_mySurf->createTorus(0.1f, 0.5f, 16, 16);
	applyColor();

	
}

void
Bus::draw()
{
	_mySurf->setObjMaterials(id[1], _shader);
	_calc->translation(_position.getX(), _position.getY() + 0.3f, _position.getZ());
	sendToGL(id[1]);

	for (size_t i = 0; i < 4; i++)
	{
		switch (i)
		{
			// front left wheel
		case 0:
			_calc->translation(_position.getX() + .8f, _position.getY() + 0.5f, _position.getZ() + 2.2f);
			break;

			// rear left wheel
		case 1:
			_calc->translation(_position.getX() + 2.2f, _position.getY() + 0.5f, _position.getZ() + 2.2f);
			break;

			// front right wheel
		case 2:
			_calc->translation(_position.getX() + .8f, _position.getY() + 0.5f, _position.getZ() - .2f);
			break;

			// rar right wheel
		case 3:
			_calc->translation(_position.getX() + 2.2f, _position.getY() + 0.5f, _position.getZ() - .2f);
			break;
		}

		_mySurf->setObjMaterials(id[0], _shader);
		_calc->rotate(90.0f, 1.0f, 0.0f, 0.0f);
		sendToGL(id[0]);
	}
}

void
Bus::update(float delta_t)
{
	float deltaX = _position.getX() - delta_t;

	if (deltaX <= -15.0f)
		setPosition(12.0f, _position.getY(), _position.getZ());
	else
		setPosition(deltaX, _position.getY(), _position.getZ());
}
