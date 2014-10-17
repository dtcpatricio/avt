#include "Cam.h"
#include "Scene.h"
#include "io.h"
#include "Frog.h"
#include "OurMathLib.h"

Cam::Cam(Scene *scene) : _scene(scene), r(5.0f), alpha(-43.0f), beta(48.0f) {}
Cam::~Cam() {}

void Cam::update() {
	x = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	z = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	y = r *   						      sin(beta * 3.14f / 180.0f);
}

void Cam::lookAt() {
	float camAdjust = -.5f;
	switch (camType) {
	case ORTHOGONAL:
		_scene->getCalc()->lookAt(0.0f, 15.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f);
		break;

	case PERSPECTIVETOP:
		_scene->getCalc()->lookAt(-5.0f, 40.0f, 5.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f);
		break;

	case PERSPECTIVE3RD:
		Frog *frog = _scene->getFrog();
		_scene->getCalc()->lookAt(
			frog->getX() + x, frog->getY() + y - camAdjust, frog->getZ() + z,
			frog->getX(), frog->getY(), frog->getZ(),
			0.0f, 1.0f, 0.0f);
		break;
	}
}

void Cam::reshape(float ratio)
{
	switch (camType) {
	case ORTHOGONAL:
		_scene->getCalc()->ortho(-30.0f, 30.0f, -20.0f, 20.0f, -30.0f, 30.0f);
		break;
	default:
		_scene->getCalc()->perspective(53.13f, ratio, .1f, -5.0f);
		break;
	}
}