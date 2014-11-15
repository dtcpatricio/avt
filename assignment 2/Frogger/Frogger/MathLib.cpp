#include "MathLib.h"

#define SWAP_ROWS_DOUBLE(a, b) { double *_tmp = a; (a) = (b); (b) = _tmp; }
#define SWAP_ROWS_FLOAT(a, b)  { float  *_tmp = a; (a) = (b); (b) = _tmp; }

#define MAT(m, r, c) (m)[(c)* 4 + (r)]

MathLib::MathLib() {}

void
MathLib::print()
{
	for (int i = 0; i < 16; i++){
		std::cout << model[i] << " ";
	}
	std::cout << std::endl;

	for (int i = 0; i < 16; i++){
		std::cout << viewMatrix[i] << " ";
	}
	std::cout << std::endl;

	//for (int i = 0; i < 16; i++){
	//	std::cout << projMatrix[i] << " ";
	//}
	std::cout << std::endl;
}

// Normalize a vec3
void MathLib::normalize(float *a) {
	const GLfloat mag = sqrt(pow(a[0], 2) + pow(a[1], 2) + pow(a[2], 2));
	a[0] /= mag; a[1] /= mag; a[2] /= mag;
}

// res = a cross b;
void MathLib::crossProduct(GLfloat *a, GLfloat *b, GLfloat *res) {
	res[0] = a[1] * b[2] - b[1] * a[2];
	res[1] = a[2] * b[0] - b[2] * a[0];
	res[2] = a[0] * b[1] - b[0] * a[1];
}

// returns a . b
float MathLib::dotProduct(GLfloat *a, GLfloat *b) {
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

void MathLib::setIdentity3x3(float *mat) {
	// fill 3x3 matrix with 0s and 1s
	int size = 3;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (i == j)
				mat[i + 4 * j] = 1.0f;
			else
				mat[i + 4 * j] = 0.0f;
		}
	}
}
void MathLib::setIdentityMatrix(float *mat, int size) {

	// fill matrix with 0s
	for (int i = 0; i < size * size; ++i)
		mat[i] = 0.0f;

	// fill diagonal with 1s
	for (int i = 0; i < size; ++i)
		mat[i + i * size] = 1.0f;
}

void MathLib::matrixMultiplication(GLfloat * m, GLfloat * aux)
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

void MathLib::pushMatrix(GLfloat * m, int type){
	mMatrixStack[type].push_back(m);
}

GLfloat * MathLib::popMatrix(int type){
	GLfloat m[16];
	memcpy(m, mMatrixStack[type][0], sizeof(GLfloat)* 16);
	mMatrixStack[type].pop_back();
	return m;

}

void MathLib::translation(float x, float y, float z)
{
	GLfloat aux[16];
	setIdentityMatrix(aux, 4);
	aux[12] = x; aux[13] = y; aux[14] = z;

	matrixMultiplication(model, aux);
}

void MathLib::scale(float x, float y, float z){

	GLfloat aux[] = {
		x, 0.0f, 0.0f, 0.0f,
		0.0f, y, 0.0f, 0.0f,
		0.0f, 0.0f, z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	matrixMultiplication(model, aux);
}

static inline float
DegToRad(float degrees)
{
	return (float)(degrees * (M_PI / 180.0f));
};

void MathLib::rotate(float angle, float x, float y, float z)
{
	GLfloat mat[16];
	float v[3];

	v[0] = x; v[1] = y;	v[2] = z;

	float radAngle = DegToRad(angle);
	float co = cos(radAngle);
	float si = sin(radAngle);

	normalize(v);
	float x2 = v[0] * v[0];
	float y2 = v[1] * v[1];
	float z2 = v[2] * v[2];

	mat[0] = co + x2 * (1 - co);
	mat[4] = v[0] * v[1] * (1 - co) - v[2] * si;
	mat[8] = v[0] * v[2] * (1 - co) + v[1] * si;
	mat[12] = 0.0f;

	mat[1] = v[0] * v[1] * (1 - co) + v[2] * si;
	mat[5] = co + y2 * (1 - co);
	mat[9] = v[1] * v[2] * (1 - co) - v[0] * si;
	mat[13] = 0.0f;

	mat[2] = v[0] * v[2] * (1 - co) - v[1] * si;
	mat[6] = v[1] * v[2] * (1 - co) + v[0] * si;
	mat[10] = co + z2 * (1 - co);
	mat[14] = 0.0f;

	mat[3] = 0.0f;
	mat[7] = 0.0f;
	mat[11] = 0.0f;
	mat[15] = 1.0f;

	matrixMultiplication(model, mat);
}

void MathLib::lookAt(float xPos, float yPos, float zPos,
	float xLook, float yLook, float zLook,
	float xUp, float yUp, float zUp)
{
	float dir[3], right[3], up[3];

	up[0] = xUp; up[1] = yUp; up[2] = zUp;

	setIdentityMatrix(viewMatrix, 4);
	dir[0] = (xLook - xPos); dir[1] = (yLook - yPos); dir[2] = (zLook - zPos);
	normalize(dir);

	crossProduct(dir, up, right);
	normalize(right);

	crossProduct(right, dir, up);
	normalize(up);

	for (size_t i = 0; i < 3; i++) {
		viewMatrix[i * 4] = right[i];
		viewMatrix[i * 4 + 1] = up[i];
		viewMatrix[i * 4 + 2] = -dir[i];
		viewMatrix[i * 4 + 3] = 0.0f;
	}

	viewMatrix[12] = viewMatrix[13] = viewMatrix[14] = 0.0f;
	viewMatrix[15] = 1.0f;

	GLfloat m2[16];
	setIdentityMatrix(m2, 4);
	m2[12] = -xPos;	m2[13] = -yPos;	m2[14] = -zPos;

	matrixMultiplication(viewMatrix, m2);
}

void MathLib::perspective(float fov, float ratio, float nearp, float farp)
{
	float f = 1.0f / tan(fov * (M_PI / 360.0f));

	setIdentityMatrix(projMatrix, 4);

	projMatrix[0] = f / ratio;
	projMatrix[1 * 4 + 1] = f;
	projMatrix[2 * 4 + 2] = (farp + nearp) / (nearp - farp);
	projMatrix[3 * 4 + 2] = (2.0f * farp * nearp) / (nearp - farp);
	projMatrix[2 * 4 + 3] = -1.0f;
	projMatrix[3 * 4 + 3] = 0.0f;
}

void MathLib::ortho(
	float left,   float right,
	float bottom, float top,
	float nearp,  float farp)
{
	setIdentityMatrix(projMatrix, 4);

	projMatrix[0 * 4 + 0] = 2 / (right - left);
	projMatrix[1 * 4 + 1] = 2 / (top - bottom);
	projMatrix[2 * 4 + 2] = -2 / (farp - nearp);
	projMatrix[3 * 4 + 0] = -(right + left) / (right - left);
	projMatrix[3 * 4 + 1] = -(top + bottom) / (top - bottom);
	projMatrix[3 * 4 + 2] = -(farp + nearp) / (farp - nearp);
}

void MathLib::MultiplyMatrixByVector4by4OpenGL_FLOAT(
		float *resultvector, const float *matrix, const float *pvector)
{
	resultvector[0] = matrix[0] * pvector[0]
			+ matrix[4]  * pvector[1]
			+ matrix[8]  * pvector[2]
			+ matrix[12] * pvector[3];
	resultvector[1] = matrix[1] * pvector[0]
			+ matrix[5]  * pvector[1]
			+ matrix[9]  * pvector[2]
			+ matrix[13] * pvector[3];
	resultvector[2] = matrix[2] * pvector[0]
			+ matrix[6]  * pvector[1]
			+ matrix[10] * pvector[2]
			+ matrix[14] * pvector[3];
	resultvector[3] = matrix[3] * pvector[0]
			+ matrix[7]  * pvector[1]
			+ matrix[11] * pvector[2]
			+ matrix[15] * pvector[3];
}

// Compute res = M * point
void 
MathLib::matrixByVector(GLfloat *vector, float *res) {

	for (int i = 0; i < 4; ++i) {

		res[i] = 0.0f;

		for (int j = 0; j < 4; j++) {

			res[i] += vector[j] * viewMatrix[j * 4 + i];
		}
	}
}

//This code comes directly from GLU except that it is for float
int MathLib::glhInvertMatrixf2(float *m, float *out)
{
	float wtmp[4][8];
	float m0, m1, m2, m3, s;
	float *r0, *r1, *r2, *r3;
	r0 = wtmp[0], r1 = wtmp[1], r2 = wtmp[2], r3 = wtmp[3];
	r0[0] = MAT(m, 0, 0), r0[1] = MAT(m, 0, 1),
		r0[2] = MAT(m, 0, 2), r0[3] = MAT(m, 0, 3),
		r0[4] = 1.0, r0[5] = r0[6] = r0[7] = 0.0,
		r1[0] = MAT(m, 1, 0), r1[1] = MAT(m, 1, 1),
		r1[2] = MAT(m, 1, 2), r1[3] = MAT(m, 1, 3),
		r1[5] = 1.0, r1[4] = r1[6] = r1[7] = 0.0,
		r2[0] = MAT(m, 2, 0), r2[1] = MAT(m, 2, 1),
		r2[2] = MAT(m, 2, 2), r2[3] = MAT(m, 2, 3),
		r2[6] = 1.0, r2[4] = r2[5] = r2[7] = 0.0,
		r3[0] = MAT(m, 3, 0), r3[1] = MAT(m, 3, 1),
		r3[2] = MAT(m, 3, 2), r3[3] = MAT(m, 3, 3),
		r3[7] = 1.0, r3[4] = r3[5] = r3[6] = 0.0;
	/* choose pivot - or die */
	if (fabsf(r3[0]) > fabsf(r2[0]))
		SWAP_ROWS_FLOAT(r3, r2);
	if (fabsf(r2[0]) > fabsf(r1[0]))
		SWAP_ROWS_FLOAT(r2, r1);
	if (fabsf(r1[0]) > fabsf(r0[0]))
		SWAP_ROWS_FLOAT(r1, r0);
	if (0.0 == r0[0])
		return 0;
	/* eliminate first variable     */
	m1 = r1[0] / r0[0];
	m2 = r2[0] / r0[0];
	m3 = r3[0] / r0[0];
	s = r0[1];
	r1[1] -= m1 * s;
	r2[1] -= m2 * s;
	r3[1] -= m3 * s;
	s = r0[2];
	r1[2] -= m1 * s;
	r2[2] -= m2 * s;
	r3[2] -= m3 * s;
	s = r0[3];
	r1[3] -= m1 * s;
	r2[3] -= m2 * s;
	r3[3] -= m3 * s;
	s = r0[4];
	if (s != 0.0) {
		r1[4] -= m1 * s;
		r2[4] -= m2 * s;
		r3[4] -= m3 * s;
	}
	s = r0[5];
	if (s != 0.0) {
		r1[5] -= m1 * s;
		r2[5] -= m2 * s;
		r3[5] -= m3 * s;
	}
	s = r0[6];
	if (s != 0.0) {
		r1[6] -= m1 * s;
		r2[6] -= m2 * s;
		r3[6] -= m3 * s;
	}
	s = r0[7];
	if (s != 0.0) {
		r1[7] -= m1 * s;
		r2[7] -= m2 * s;
		r3[7] -= m3 * s;
	}
	/* choose pivot - or die */
	if (fabsf(r3[1]) > fabsf(r2[1]))
		SWAP_ROWS_FLOAT(r3, r2);
	if (fabsf(r2[1]) > fabsf(r1[1]))
		SWAP_ROWS_FLOAT(r2, r1);
	if (0.0 == r1[1])
		return 0;
	/* eliminate second variable */
	m2 = r2[1] / r1[1];
	m3 = r3[1] / r1[1];
	r2[2] -= m2 * r1[2];
	r3[2] -= m3 * r1[2];
	r2[3] -= m2 * r1[3];
	r3[3] -= m3 * r1[3];
	s = r1[4];
	if (0.0 != s) {
		r2[4] -= m2 * s;
		r3[4] -= m3 * s;
	}
	s = r1[5];
	if (0.0 != s) {
		r2[5] -= m2 * s;
		r3[5] -= m3 * s;
	}
	s = r1[6];
	if (0.0 != s) {
		r2[6] -= m2 * s;
		r3[6] -= m3 * s;
	}
	s = r1[7];
	if (0.0 != s) {
		r2[7] -= m2 * s;
		r3[7] -= m3 * s;
	}
	/* choose pivot - or die */
	if (fabsf(r3[2]) > fabsf(r2[2]))
		SWAP_ROWS_FLOAT(r3, r2);
	if (0.0 == r2[2])
		return 0;
	/* eliminate third variable */
	m3 = r3[2] / r2[2];
	r3[3] -= m3 * r2[3], r3[4] -= m3 * r2[4],
		r3[5] -= m3 * r2[5], r3[6] -= m3 * r2[6], r3[7] -= m3 * r2[7];
	/* last check */
	if (0.0 == r3[3])
		return 0;
	s = 1.0 / r3[3];		/* now back substitute row 3 */
	r3[4] *= s;
	r3[5] *= s;
	r3[6] *= s;
	r3[7] *= s;
	m2 = r2[3];			/* now back substitute row 2 */
	s = 1.0 / r2[2];
	r2[4] = s * (r2[4] - r3[4] * m2), r2[5] = s * (r2[5] - r3[5] * m2),
		r2[6] = s * (r2[6] - r3[6] * m2), r2[7] = s * (r2[7] - r3[7] * m2);
	m1 = r1[3];
	r1[4] -= r3[4] * m1, r1[5] -= r3[5] * m1,
		r1[6] -= r3[6] * m1, r1[7] -= r3[7] * m1;
	m0 = r0[3];
	r0[4] -= r3[4] * m0, r0[5] -= r3[5] * m0,
		r0[6] -= r3[6] * m0, r0[7] -= r3[7] * m0;
	m1 = r1[2];			/* now back substitute row 1 */
	s = 1.0 / r1[1];
	r1[4] = s * (r1[4] - r2[4] * m1), r1[5] = s * (r1[5] - r2[5] * m1),
		r1[6] = s * (r1[6] - r2[6] * m1), r1[7] = s * (r1[7] - r2[7] * m1);
	m0 = r0[2];
	r0[4] -= r2[4] * m0, r0[5] -= r2[5] * m0,
		r0[6] -= r2[6] * m0, r0[7] -= r2[7] * m0;
	m0 = r0[1];			/* now back substitute row 0 */
	s = 1.0 / r0[0];
	r0[4] = s * (r0[4] - r1[4] * m0), r0[5] = s * (r0[5] - r1[5] * m0),
		r0[6] = s * (r0[6] - r1[6] * m0), r0[7] = s * (r0[7] - r1[7] * m0);
	MAT(out, 0, 0) = r0[4];
	MAT(out, 0, 1) = r0[5], MAT(out, 0, 2) = r0[6];
	MAT(out, 0, 3) = r0[7], MAT(out, 1, 0) = r1[4];
	MAT(out, 1, 1) = r1[5], MAT(out, 1, 2) = r1[6];
	MAT(out, 1, 3) = r1[7], MAT(out, 2, 0) = r2[4];
	MAT(out, 2, 1) = r2[5], MAT(out, 2, 2) = r2[6];
	MAT(out, 2, 3) = r2[7], MAT(out, 3, 0) = r3[4];
	MAT(out, 3, 1) = r3[5], MAT(out, 3, 2) = r3[6];
	MAT(out, 3, 3) = r3[7];
	return 1;
}

int MathLib::glhUnProjectf(float winx, float winy, float winz, float *modelView, float *projection, int *viewport,
	float *posx, float *posy, float *posz)
{
	//Transformation matrices
	float m[16], A[16];
	float in[4], out[4];
	//Calculation for inverting a matrix, compute projection x modelview
	//and store in A[16]
	setIdentityMatrix(A, 4);
	matrixMultiplication(A, modelView);
	matrixMultiplication(A, projection);
	//Now compute the inverse of matrix A
	if (glhInvertMatrixf2(A, m) == 0)
		return 0;
	//Transformation of normalized coordinates between -1 and 1
	in[0] = (winx - (float)viewport[0]) / (float)viewport[2] * 2.0 - 1.0;
	in[1] = (winy - (float)viewport[1]) / (float)viewport[3] * 2.0 - 1.0;
	in[2] = 2.0*winz - 1.0;
	in[3] = 1.0;
	//Objects coordinates
	MultiplyMatrixByVector4by4OpenGL_FLOAT(out, m, in);
	if (out[3] == 0.0)
		return 0;
	out[3] = 1.0 / out[3];
	*posx = out[0] * out[3];
	*posy = out[1] * out[3];
	*posz = -out[2] * out[3];
	return 1;
}

Vector3*
MathLib::GetGLPos(int x, int y)
{
	GLint viewport[4];
	GLfloat winX, winY, winZ;
	GLfloat posX, posY, posZ;

	glGetIntegerv(GL_VIEWPORT, viewport);

	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

	glhUnProjectf(winX, winY, winZ, viewMatrix, projMatrix, viewport, &posX, &posY, &posZ);
	Vector3* positions = new Vector3(posX, posY, posZ);
	std::cout << "Pos: " << posX << " " << posY << " " << posZ << std::endl;
	return positions;
}

GLfloat * MathLib::getViewMatrix() { return viewMatrix; }
GLfloat * MathLib::getProjMatrix() { return projMatrix; }
GLfloat * MathLib::getModel()      { return model; }

// computes the derived normal matrix
GLfloat*
MathLib::computeNormalMatrix3x3() {

	float mMat3x3[9];
	float viewModel[16];
	float mNormal3x3[9];
	memcpy(viewModel, viewMatrix, sizeof(float) * 16);
	matrixMultiplication(viewModel, model);
	mMat3x3[0] = viewModel[0];
	mMat3x3[1] = viewModel[1];
	mMat3x3[2] = viewModel[2];

	mMat3x3[3] = viewModel[4];
	mMat3x3[4] = viewModel[5];
	mMat3x3[5] = viewModel[6];

	mMat3x3[6] = viewModel[8];
	mMat3x3[7] = viewModel[9];
	mMat3x3[8] = viewModel[10];

	float det, invDet;

	det = mMat3x3[0] * (mMat3x3[4] * mMat3x3[8] - mMat3x3[5] * mMat3x3[7]) +
		mMat3x3[1] * (mMat3x3[5] * mMat3x3[6] - mMat3x3[8] * mMat3x3[3]) +
		mMat3x3[2] * (mMat3x3[3] * mMat3x3[7] - mMat3x3[4] * mMat3x3[6]);

	invDet = 1.0f / det;

	mNormal3x3[0] = (mMat3x3[4] * mMat3x3[8] - mMat3x3[5] * mMat3x3[7]) * invDet;
	mNormal3x3[1] = (mMat3x3[5] * mMat3x3[6] - mMat3x3[8] * mMat3x3[3]) * invDet;
	mNormal3x3[2] = (mMat3x3[3] * mMat3x3[7] - mMat3x3[4] * mMat3x3[6]) * invDet;
	mNormal3x3[3] = (mMat3x3[2] * mMat3x3[7] - mMat3x3[1] * mMat3x3[8]) * invDet;
	mNormal3x3[4] = (mMat3x3[0] * mMat3x3[8] - mMat3x3[2] * mMat3x3[6]) * invDet;
	mNormal3x3[5] = (mMat3x3[1] * mMat3x3[6] - mMat3x3[7] * mMat3x3[0]) * invDet;
	mNormal3x3[6] = (mMat3x3[1] * mMat3x3[5] - mMat3x3[4] * mMat3x3[2]) * invDet;
	mNormal3x3[7] = (mMat3x3[2] * mMat3x3[3] - mMat3x3[0] * mMat3x3[5]) * invDet;
	mNormal3x3[8] = (mMat3x3[0] * mMat3x3[4] - mMat3x3[3] * mMat3x3[1]) * invDet;
	
	return mNormal3x3;
}
