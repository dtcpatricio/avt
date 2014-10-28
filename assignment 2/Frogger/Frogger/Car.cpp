#include "Car.h"


Car::Car(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* calc)
	: DynamicObject(mySurf, shader, calc)
{
	id[0] = 6;
	id[1] = 7;
	amb[0] = 0.1745f; amb[1] = 0.01175f; amb[2] = 0.01175f; amb[3] = 1.f;
	diff[0] = 0.61424f; diff[1] = 0.04136f; diff[2] = 0.04136f; diff[3] = 1.f;
	spec[0] = 0.727811f; spec[1] = 0.626959f; spec[2] = 0.626959f; spec[3] = 1.f;
	emissive[0] = 0.f;  emissive[1] = 0.f; emissive[2] = 0.f; emissive[3] = 1.f;
	shininess = new float(80.0f);

	_obj_length = 2.f;
	_obj_width = 1.f;
	_boundingBox->setParams(_obj_length, _obj_width);
}


Car::~Car()
{
}

void
Car::create()
{
	// body shop
	_mySurf->setObjId(id[1]);
	_mySurf->createCylinder(2.f, 1.f, 4);
	applyColor();

	setTires();

	// wheels
	_mySurf->setObjId(id[0]);
	_mySurf->createTorus(0.1f, 0.3f, 16, 16);
	applyColor();
}

void
Car::draw()
{
	_mySurf->setObjMaterials(id[1], _shader);
	_calc->translation(_position.getX(), _position.getY() + 0.3f, _position.getZ());
	_calc->scale(1.f, 1.0f, 1.f);
	_calc->rotate(90.0f, 0.0f, 0.0f, 1.0f);
	_calc->rotate(45.0f, 0.0f, 1.0f, 0.0f);
	sendToGL(id[1]);

	for (size_t i = 0; i < 4; i++)
	{
		switch (i)
		{
			// front left wheel
		case 0:
			_calc->translation(_position.getX() - .5f, _position.getY() - .2f, _position.getZ() + .8f);
			break;

			// rear left wheel
		case 1:
			_calc->translation(_position.getX() + .5f, _position.getY() - 0.2f, _position.getZ() + .8f);
			break;

			// front right wheel
		case 2:
			_calc->translation(_position.getX() - .5f, _position.getY() - 0.2f, _position.getZ() - .8f);
			break;

			// rar right wheel
		case 3:
			_calc->translation(_position.getX() + .5f, _position.getY() - .2f, _position.getZ() - .8f);
			break;
		}
		
		_mySurf->setObjMaterials(id[0], _shader);
		_calc->rotate(90.0f, 1.0f, 0.0f, 0.0f);
		sendToGL(id[0]);
	}
}

void
Car::update(float delta_t)
{
	float deltaX = _position.getX() - delta_t;

	if (deltaX <= -15.0f)
		setPosition(13.0f, _position.getY(), _position.getZ());
	else
		setPosition(deltaX, _position.getY(), _position.getZ());
}
