#include "OurMathLib.h"

OurMathLib::OurMathLib() {}

OurMathLib::~OurMathLib() {}

// Normalize a vec3
void OurMathLib::normalize(float *a) {
	const GLfloat mag = sqrt(pow(a[0], 2) + pow(a[1], 2) + pow(a[2], 2));
	a[0] /= mag; a[1] /= mag; a[2] /= mag;
}

// res = a cross b;
void OurMathLib::crossProduct(GLfloat *a, GLfloat *b, GLfloat *res) {
	res[0] = a[1] * b[2] - b[1] * a[2];
	res[1] = a[2] * b[0] - b[2] * a[0];
	res[2] = a[0] * b[1] - b[0] * a[1];
}

// returns a . b
float OurMathLib::dotProduct(GLfloat *a, GLfloat *b) {
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

void OurMathLib::setIdentityMatrix(float *mat, int size) {

	// fill matrix with 0s
	for (int i = 0; i < size * size; ++i)
		mat[i] = 0.0f;

	// fill diagonal with 1s
	for (int i = 0; i < size; ++i)
		mat[i + i * size] = 1.0f;
}

void OurMathLib::matrixMultiplication(GLfloat * m, GLfloat * aux)
{
	GLfloat res[16];
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			res[j * 4 + i] = 0.0f;
			for (int k = 0; k < 4; k++)
			{
				res[j * 4 + i] += m[k * 4 + i] * aux[j * 4 + k];
			}
		}
	}
	memcpy(m, res, 16 * sizeof(GLfloat));
}

void OurMathLib::pushMatrix(GLfloat * m, int type){
	mMatrixStack[type].push_back(m);
}

GLfloat * OurMathLib::popMatrix(int type){
	GLfloat m[16];
	memcpy(m, mMatrixStack[type][0], sizeof(GLfloat)* 16);
	mMatrixStack[type].pop_back();
	return m;

}