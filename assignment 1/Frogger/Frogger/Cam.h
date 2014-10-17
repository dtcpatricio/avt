#ifndef __CAM_H__
#define __CAM_H__

#include "misc.h"

class Scene;

class Cam {
public:
	Cam(Scene *scene);
	~Cam();

	float getX() { return x; }
	float getY() { return y; }
	float getZ() { return z; }

	float getR() { return r;     }
	float getA() { return alpha; }
	float getB() { return beta;  }

	void setR(float value) { r     = value; }
	void setA(float value) { alpha = value; }
	void setB(float value) { beta  = value; }

	void setCamType(CamType value) { camType = value; }

	// Set camera position based on its spherical coordinates
	void update();

	void lookAt();
	void reshape(float ratio);

private:
	Scene *_scene;

	CamType camType = ORTHOGONAL;
	float r, alpha, beta;
	float x, y, z;
};

#endif