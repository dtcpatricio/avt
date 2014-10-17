#include <vector>
#include <string>
#include <iostream>
#include <GL/glew.h>

#include "Scene.h"

#ifndef __OurMathLib__
#define __OurMathLib__

#ifdef _WIN32
#define M_PI       3.14159265358979323846f
#endif

#define COUNT_MATRICES 2
#define MOD 0
#define V 1
#define PROJ 2

class Scene;

class OurMathLib {
	Scene *_scene;

	GLfloat model[16], projMatrix[16], viewMatrix[16];

public:
	OurMathLib(Scene *scene);

	// Normalize a vec3
	void normalize(float *a);

	// res = a cross b;
	void crossProduct(GLfloat *a, GLfloat *b, GLfloat *res);

	// returns a . b
	float dotProduct(GLfloat *a, GLfloat *b);

	void setIdentityMatrix(float *mat, int size);

	void matrixMultiplication(GLfloat * m, GLfloat * aux);

	void pushMatrix(GLfloat * m, int type);

	GLfloat* popMatrix(int type);

	void OurMathLib::translation(float x, float y, float z);
	void OurMathLib::scale(float x, float y, float z);
	void OurMathLib::rotate(float angle, float x, float y, float z);
	void OurMathLib::lookAt(
		float xPos, float yPos, float zPos,
		float xLook, float yLook, float zLook,
		float xUp, float yUp, float zUp);
	void OurMathLib::perspective(float fov, float ratio, float nearp, float farp);
	void OurMathLib::ortho(
		float left, float right,
		float bottom, float top,
		float nearp, float farp);
	void OurMathLib::MultiplyMatrixByVector4by4OpenGL_FLOAT(
			float *resultvector,
			const float *matrix,
			const float *pvector);
	int OurMathLib::glhInvertMatrixf2(float *m, float *out);
	int OurMathLib::glhUnProjectf(
			float winx, float winy, float winz,
			float *modelView, float *projection, int *viewport,
			float *posx, float *posy, float *posz);
	void OurMathLib::GetOGLPos(int x, int y);

	GLfloat * OurMathLib::getModel();
	GLfloat * OurMathLib::getViewMatrix();
	GLfloat * OurMathLib::getProjMatrix();

protected:

	/// Matrix stacks for all matrix types
	std::vector<GLfloat *> mMatrixStack[COUNT_MATRICES];

};

#endif