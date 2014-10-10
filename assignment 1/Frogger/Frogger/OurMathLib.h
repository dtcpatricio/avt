
#include <vector>
#include <string>
#include <GL/glew.h>

#ifndef __OurMathLib__
#define __OurMathLib__



class  OurMathLib
{
public:

	#define COUNT_MATRICES 2
	#define MOD 0
	#define V 1
	#define PROJ 2


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

	void pushMatrix(GLfloat * m, int type);

	GLfloat* popMatrix(int type);

protected:

	/// Matrix stacks for all matrix types
	std::vector<GLfloat *> mMatrixStack[COUNT_MATRICES];


};


#endif