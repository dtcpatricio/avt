
#include <vector>
#include <string>
#include <GL/glew.h>

#ifndef __OurMathLib__
#define __OurMathLib__

class  OurMathLib
{
public:
	OurMathLib();
	~OurMathLib();

	// Normalize a vec3
	void normalize(float *a);

	// res = a cross b;
	void crossProduct(GLfloat *a, GLfloat *b, GLfloat *res);

	// returns a . b
	float dotProduct(GLfloat *a, GLfloat *b);

	void setIdentityMatrix(float *mat, int size);

	void matrixMultiplication(GLfloat * m, GLfloat * aux);


};


#endif