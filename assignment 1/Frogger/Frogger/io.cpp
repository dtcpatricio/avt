#include "io.h"
#include "Scene.h"
#include "Cam.h"
#include "Frog.h"
#include "OurMathLib.h"

IO::IO(Scene *scene) : _scene(scene) {}

void IO::mouseWheel(int wheel, int direction, int x, int y) {
	Cam *cam = _scene->getCam();
	
	float r = cam->getR();
	r += direction * 0.1f;
	if (r < 0.1f) {
		r = 0.1f;
	}

	cam->setR(r);
	cam->update();
}

void IO::processMouseMotion(int xx, int yy) {
	Cam *cam = _scene->getCam();

	float r     = cam->getR();
	float alpha = cam->getA();
	float beta  = cam->getB();

	int   deltaX = -xx + startX, deltaY = yy - startY;
	float alphaAux, betaAux;
	float rAux;

	// left mouse button: move camera
	if (tracking == 1) {
		alphaAux = alpha + deltaX;
		betaAux = beta + deltaY;

		if (betaAux > 85.0f)
			betaAux = 85.0f;
		else if (betaAux < -85.0f)
			betaAux = -85.0f;
		rAux = r;
	}
	// right mouse button: zoom
	else if (tracking == 2) {
		alphaAux = alpha;
		betaAux = beta;
		rAux = r + (deltaY * 0.01f);
		if (rAux < 0.1f)
			rAux = 0.1f;
	}

	cam->setR(rAux);
	cam->setA(alphaAux);
	cam->setB(betaAux);
	cam->update();
}

void IO::processMouseButtons(int button, int state, int xx, int yy)
{
	// start tracking the mouse
	if (state == GLUT_DOWN)  {
		startX = xx;
		startY = yy;
		if (button == GLUT_LEFT_BUTTON)
			tracking = 1;
		else if (button == GLUT_RIGHT_BUTTON) {
			tracking = 2;
			_scene->getCalc()->GetOGLPos(xx, yy);
			float xcomp = getMouseWorldX() - _scene->getFrog()->getX();
			float zcomp = getMouseWorldZ() - _scene->getFrog()->getZ();
			float xabs = abs(xcomp);
			float zabs = abs(zcomp);
			std::cout << "X: " << xcomp << std::endl << "Z: " << zcomp << std::endl;
			if (xcomp >= 0 && zcomp <= 0) {
				_scene->getFrog()->setUp(true);
				if (xabs >= zabs)
					_scene->getFrog()->setRight(true);
				else
					_scene->getFrog()->setLeft(true);
			}
			else {
				_scene->getFrog()->setDown(true);
				if (xabs >= zabs)
					_scene->getFrog()->setLeft(true);
				else
					_scene->getFrog()->setRight(true);
			}
		}
	}

	//stop tracking the mouse
	else if (state == GLUT_UP) {
		if (tracking == 1) {
			float alpha = _scene->getCam()->getA() - (xx - startX);
			_scene->getCam()->setA(alpha);
			float beta  = _scene->getCam()->getB() + (yy - startY);
			_scene->getCam()->setB(beta);
		}
		else if (tracking == 2) {
			_scene->getFrog()->setUp   (false);
			_scene->getFrog()->setDown (false);
			_scene->getFrog()->setLeft (false);
			_scene->getFrog()->setRight(false);

			float r = _scene->getCam()->getR();
			r += (yy - startY) * 0.01f;
			if (r < 0.1f)
				r = 0.1f;
			_scene->getCam()->setR(r);
		}
		tracking = 0;
	}
}

void IO::reshape(int w, int h) {
	// Look at this lovely hack
	int WindowX = w;
	int WindowY = h;
	float ratio = (1.0f * WindowX) / WindowY;

	glViewport(0, 0, WindowX, WindowY);
	_scene->getCam()->reshape(ratio);
}

void IO::processKeys(unsigned char key, int xx, int yy)
{
	switch (key) {

	case '1':
		_scene->getCam()->setCamType(ORTHOGONAL);
		reshape(WinX, WinY);
		break;

	case '2':
		_scene->getCam()->setCamType(PERSPECTIVETOP);
		reshape(WinX, WinY);
		break;

	case '3':
		_scene->getCam()->setCamType(PERSPECTIVE3RD);
		reshape(WinX, WinY);
		break;

	case 'o':
		_scene->getFrog()->setLeft(true);
		break;

	case 'p':
		_scene->getFrog()->setRight(true);
		break;

	case 'q':
		_scene->getFrog()->setUp(true);
		break;

	case 'a':
		_scene->getFrog()->setDown(true);
		break;

	case 27:
		glutLeaveMainLoop();
		break;

	case 'c':
		printf("Camera Spherical Coordinates (%f, %f, %f)\n"
			, _scene->getCam()->getA(), _scene->getCam()->getB(), _scene->getCam()->getR());
		break;

	}
}

void IO::processUpKeys(unsigned char key, int x, int y) {
	switch (key) {
	case 'o':
		_scene->getFrog()->setLeft(false);
		break;

	case 'p':
		_scene->getFrog()->setRight(false);
		break;

	case 'q':
		_scene->getFrog()->setUp(false);
		break;

	case 'a':
		_scene->getFrog()->setDown(false);
		break;
	}
}