#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <math.h>
#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "cube.h"

typedef GLfloat Matrix[16];

/////////////////////////////////////////////////////////////////////// Auxiliary Math Functions
void normalize(float *a);

void crossProduct(GLfloat *a, GLfloat *b, GLfloat *res);

float dotProduct(GLfloat *a, GLfloat *b);

/////////////////////////////////////////////////////////////////////// TRANSFORMATIONS

void setIdentityMatrix(float *mat, int size);

void matrixMultiplication(Matrix m, Matrix aux);

void translation(float x, float y, float z);

void scale(float x, float y, float z);

void rotate(float angle, float x, float y, float z);

void lookAt(float xPos, float yPos, float zPos,
	float xLook, float yLook, float zLook,
	float xUp, float yUp, float zUp);

void perspective(float fov, float ratio, float nearp, float farp);

