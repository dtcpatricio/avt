#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <GL/glew.h>
#include "Vector3.h"

#ifdef _WIN32
#define M_PI       3.14159265358979323846f
#endif

#define COUNT_MATRICES 2
#define MOD 0
#define V 1
#define PROJ 2


class MathLib {

	GLfloat model[16], projMatrix[16], viewMatrix[16];

public:
	MathLib();

	void print();

	// Normalize a vec3
	void normalize(float *a);

	// res = a cross b;
	void crossProduct(GLfloat *a, GLfloat *b, GLfloat *res);

	// returns a . b
	float dotProduct(GLfloat *a, GLfloat *b);

	void setIdentity3x3(float *mat);

	void setIdentityMatrix(float *mat, int size);

	void matrixMultiplication(GLfloat * m, GLfloat * aux);

	void pushMatrix(GLfloat * m, int type);

	void matrixByVector(GLfloat* vector, float* res);

	GLfloat* popMatrix(int type);

	void MathLib::translation(float x, float y, float z);
	void MathLib::scale(float x, float y, float z);
	void MathLib::rotate(float angle, float x, float y, float z);
	void MathLib::lookAt(
		float xPos, float yPos, float zPos,
		float xLook, float yLook, float zLook,
		float xUp, float yUp, float zUp);
	void MathLib::perspective(float fov, float ratio, float nearp, float farp);
	void MathLib::ortho(
		float left, float right,
		float bottom, float top,
		float nearp, float farp);
	void MathLib::MultiplyMatrixByVector4by4OpenGL_FLOAT(
			float *resultvector,
			const float *matrix,
			const float *pvector);
	int MathLib::glhInvertMatrixf2(float *m, float *out);
	int MathLib::glhUnProjectf(
			float winx, float winy, float winz,
			float *modelView, float *projection, int *viewport,
			float *posx, float *posy, float *posz);

	Vector3 * MathLib::GetGLPos(int x, int y);
	GLfloat * MathLib::getModel();
	GLfloat * MathLib::getViewMatrix();
	GLfloat * MathLib::getProjMatrix();
	void MathLib::setProjMatrix(GLfloat * m);
	GLfloat * MathLib::computeNormalMatrix3x3();

protected:

	/// Matrix stacks for all matrix types
	std::vector<GLfloat *> mMatrixStack[COUNT_MATRICES];

};