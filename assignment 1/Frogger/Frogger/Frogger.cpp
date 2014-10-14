// Frogger.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <math.h>
#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "Frogger.h"
#include "Frog.h"
#include "OurMathLib.h"
#include "vsShaderLib.h"
#include "vsResSurfRevLib.h"

#ifdef _WIN32
#define M_PI       3.14159265358979323846f
#endif

#define CAPTION "Frogger (Assignment 1)"
#define ORTHOGONAL 0
#define PERSPECTIVETOP 1
#define PERSPECTIVE3RD 2

#define SWAP_ROWS_DOUBLE(a, b) { double *_tmp = a; (a) = (b); (b) = _tmp; }
#define SWAP_ROWS_FLOAT(a, b) { float *_tmp = a; (a) = (b); (b) = _tmp; }
#define MAT(m, r, c) (m)[(c)* 4 + (r)]

OurMathLib calc;
VSShaderLib shader;
VSResSurfRevLib mySurf;

// Mesh array definition

int WinX = 640, WinY = 480;
int WindowHandle = 0;
int faceCount = 12;

// camera type
int camType = ORTHOGONAL;
GLboolean leftPressed = false, rightPressed = false, upPressed = false, downPressed = false;
const GLfloat FPS = 1000 / 60;
unsigned int FrameCount = 0;

GLfloat projMatrix[16], viewMatrix[16], model[16];
GLfloat color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat mouseWorldP[3];

GLuint viewMatrixId, projId, modelId, colorId;

GLfloat* modelRet;

Frog frog;

// Camera Coordinates
float camX, camY, camZ;

// Camera Spherical Coordinates
float alpha = -43.0f, beta = 48.0f;
float r = 5.0f;

// Mouse Tracking Variables
int startX, startY, tracking = 0;
float eyeX = 0, eyeY = 0, eyeZ = 0;

static inline float
DegToRad(float degrees)
{
	return (float)(degrees * (M_PI / 180.0f));
};

/////////////////////////////////////////////////////////////////////// ERRORS

bool isOpenGLError() {
	bool isError = false;
	GLenum errCode;
	const GLubyte *errString;
	while ((errCode = glGetError()) != GL_NO_ERROR) {
		isError = true;
		errString = gluErrorString(errCode);
		std::cerr << "OpenGL ERROR [" << errString << "]." << std::endl;
	}
	return isError;
}

void checkOpenGLError(std::string error)
{
	if (isOpenGLError()) {
		std::cerr << error << std::endl;
		exit(EXIT_FAILURE);
	}
}

/////////////////////////////////////////////////////////////////////// SHADERs

GLuint setupShaders()
{
	shader.init();
	shader.loadShader(VSShaderLib::VERTEX_SHADER, "assign1.vert");
	shader.loadShader(VSShaderLib::FRAGMENT_SHADER, "assign1.frag");

	shader.setProgramOutput(0, "outFrag");
	shader.setVertexAttribName(VSShaderLib::VERTEX_COORD_ATTRIB, "in_pos");
	shader.setVertexAttribName(VSShaderLib::TEXTURE_COORD_ATTRIB, "in_color");
	shader.prepareProgram();

	return(shader.isProgramValid());
}

void destroyShaderProgram()
{
	glUseProgram(0);
	glDetachShader(shader.getProgramIndex(), shader.getShaderIndex(shader.VERTEX_SHADER));
	glDetachShader(shader.getProgramIndex(), shader.getShaderIndex(shader.FRAGMENT_SHADER));

	glDeleteShader(shader.getShaderIndex(shader.FRAGMENT_SHADER));
	glDeleteShader(shader.getShaderIndex(shader.VERTEX_SHADER));
	glDeleteProgram(shader.getProgramIndex());

	checkOpenGLError("ERROR: Could not destroy shaders.");
}

/////////////////////////////////////////////////////////////////////// VAOs & VBOs

void destroyBufferObjects()
{

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	checkOpenGLError("ERROR: Could not destroy VAOs and VBOs.");
}

/////////////////////////////////////////////////////////////////////// TRANSFORMATIONS

void translation(float x, float y, float z)
{
	GLfloat aux[16];
	calc.setIdentityMatrix(aux, 4);
	aux[12] = x; aux[13] = y; aux[14] = z;

	calc.matrixMultiplication(model, aux);
}

void scale(float x, float y, float z){

	GLfloat aux[] = {
		x, 0.0f, 0.0f, 0.0f,
		0.0f, y, 0.0f, 0.0f,
		0.0f, 0.0f, z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	calc.matrixMultiplication(model, aux);
}

void rotate(float angle, float x, float y, float z)
{
	GLfloat mat[16];
	float v[3];

	v[0] = x; v[1] = y;	v[2] = z;

	float radAngle = DegToRad(angle);
	float co = cos(radAngle);
	float si = sin(radAngle);

	calc.normalize(v);
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

	calc.matrixMultiplication(model, mat);
}

void lookAt(float xPos, float yPos, float zPos,
	float xLook, float yLook, float zLook,
	float xUp, float yUp, float zUp)
{
	float dir[3], right[3], up[3];

	up[0] = xUp; up[1] = yUp; up[2] = zUp;

	dir[0] = (xLook - xPos); dir[1] = (yLook - yPos); dir[2] = (zLook - zPos);
	calc.normalize(dir);

	calc.crossProduct(dir, up, right);
	calc.normalize(right);

	calc.crossProduct(right, dir, up);
	calc.normalize(up);

	for (size_t i = 0; i < 3; i++) {
		viewMatrix[i * 4] = right[i];
		viewMatrix[i * 4 + 1] = up[i];
		viewMatrix[i * 4 + 2] = -dir[i];
		viewMatrix[i * 4 + 3] = 0.0f;
	}

	viewMatrix[12] = viewMatrix[13] = viewMatrix[14] = 0.0f;
	viewMatrix[15] = 1.0f;

	GLfloat m2[16];
	calc.setIdentityMatrix(m2, 4);
	m2[12] = -xPos;	m2[13] = -yPos;	m2[14] = -zPos;

	calc.matrixMultiplication(viewMatrix, m2);
}

void perspective(float fov, float ratio, float nearp, float farp)
{
	float f = 1.0f / tan(fov * (M_PI / 360.0f));

	calc.setIdentityMatrix(projMatrix, 4);

	projMatrix[0] = f / ratio;
	projMatrix[1 * 4 + 1] = f;
	projMatrix[2 * 4 + 2] = (farp + nearp) / (nearp - farp);
	projMatrix[3 * 4 + 2] = (2.0f * farp * nearp) / (nearp - farp);
	projMatrix[2 * 4 + 3] = -1.0f;
	projMatrix[3 * 4 + 3] = 0.0f;
}

void ortho(float left, float right,
	float bottom, float top,
	float nearp, float farp)
{
	calc.setIdentityMatrix(projMatrix, 4);

	projMatrix[0 * 4 + 0] = 2 / (right - left);
	projMatrix[1 * 4 + 1] = 2 / (top - bottom);
	projMatrix[2 * 4 + 2] = -2 / (farp - nearp);
	projMatrix[3 * 4 + 0] = -(right + left) / (right - left);
	projMatrix[3 * 4 + 1] = -(top + bottom) / (top - bottom);
	projMatrix[3 * 4 + 2] = -(farp + nearp) / (farp - nearp);
}

void MultiplyMatrixByVector4by4OpenGL_FLOAT(float *resultvector, const float *matrix, const float *pvector)
{
	resultvector[0] = matrix[0] * pvector[0] + matrix[4] * pvector[1] + matrix[8] * pvector[2] + matrix[12] * pvector[3];
	resultvector[1] = matrix[1] * pvector[0] + matrix[5] * pvector[1] + matrix[9] * pvector[2] + matrix[13] * pvector[3];
	resultvector[2] = matrix[2] * pvector[0] + matrix[6] * pvector[1] + matrix[10] * pvector[2] + matrix[14] * pvector[3];
	resultvector[3] = matrix[3] * pvector[0] + matrix[7] * pvector[1] + matrix[11] * pvector[2] + matrix[15] * pvector[3];
}

//This code comes directly from GLU except that it is for float
int glhInvertMatrixf2(float *m, float *out)
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

int glhUnProjectf(float winx, float winy, float winz, float *modelView, float *projection, int *viewport,
	float *posx, float *posy, float *posz)
{
	//Transformation matrices
	float m[16], A[16];
	float in[4], out[4];
	//Calculation for inverting a matrix, compute projection x modelview
	//and store in A[16]
	calc.setIdentityMatrix(A, 4);
	calc.matrixMultiplication(A, modelView);
	calc.matrixMultiplication(A, projection);
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
	*posz = - out[2] * out[3];
	return 1;
}
void GetOGLPos(int x, int y)
{
	GLint viewport[4];
	GLfloat winX, winY, winZ;
	GLfloat posX, posY, posZ;

	glGetIntegerv(GL_VIEWPORT, viewport);

	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

	glhUnProjectf(winX, winY, winZ, viewMatrix, projMatrix, viewport, &posX, &posY, &posZ);
	mouseWorldP[0] = posX;
	mouseWorldP[1] = posY;
	mouseWorldP[2] = posZ;
	std::cout << "Pos: " << posX << " " << posY << " " << posZ << std::endl;
}
/////////////////////////////////////////////////////////////////////// SCENE

void createRiver(){
	mySurf.setObjId(0);
	mySurf.createCylinder(4.0f, 10.0f, 4);
	
	//translation(0.0f, 0.0f, -10.0f); 1st river
	//translation(10.0f, 0.0f, 0.0f); 2nd river
}

void createRoad(){
	mySurf.setObjId(1);
	mySurf.createCylinder(4.0f, 10.0f, 4);
	//translation(-12.0f, 0.0f, 2.0f);
	//translation(-2.0f, 0.0f, 12.0f);
}

void createTopLogs(){
	mySurf.setObjId(2);
	mySurf.createCylinder(6.0f, 0.75f, 8);
	//translation(2.0f, 2.75f, -15.0f);
	
	
	//translation(8.5f, 2.75f, -8.5f);
	//rotate(90.0f, 1.0f, 0.0f, -1.0f);
	
	//translation(16.0f, 2.75f, -1.0f);
	//rotate(90.0f, 1.0f, 0.0f, -1.0f);
}

void middleRowLogs(){
	mySurf.setObjId(3);
	mySurf.createCylinder(5.0f, 0.75f, 8);
	//translation(-2.5f, 2.75f, -12.5f);
	//rotate(90.0f, 1.0f, 0.0f, -1.0f);
	
	//translation(4.0f, 2.75f, -6.0f);
	//rotate(90.0f, 1.0f, 0.0f, -1.0f);
	
	//translation(10.5f, 2.75f, 0.5f);
	//rotate(90.0f, 1.0f, 0.0f, -1.0f);
}

void bottomRowLogs(){
	mySurf.setObjId(4);
	mySurf.createCylinder(5.0f, 0.75f, 8);
	//translation(-2.5f, 2.75f, -9.5f);
	//rotate(90.0f, 1.0f, 0.0f, -1.0f);
	
	//translation(3.5f, 2.75f, -3.5f);
	//rotate(90.0f, 1.0f, 0.0f, -1.0f);
	
	//translation(9.5f, 2.75f, 2.5f);
	//rotate(90.0f, 1.0f, 0.0f, -1.0f);
}

void createLogs(){

	createTopLogs();
	middleRowLogs();
	bottomRowLogs();
}

void createGrassEdge()
{
	mySurf.setObjId(5);
	mySurf.createCylinder(29.0f, 2.0f, 4);
	//translation(11.0f, .5f, -11.0f);
	//rotate(90.0f, 1.0f, 0.0f, -1.0f);
}

void createStreetEdge()
{
	mySurf.setObjId(6);
	mySurf.createCylinder(29.0f, 2.0f, 4);
	//translation(-1.0f, .5f, 1.0f);
	//rotate(90.0f, 1.0f, 0.0f, -1.0f);
}

void createSidewalkEdge()
{
	mySurf.setObjId(7);
	mySurf.createCylinder(29.0f, 2.0f, 4);
	//translation(-13.0f, .5f, 13.0f);
	//rotate(90.0f, 1.0f, 0.0f, -1.0f);
}

void createEdges()
{
	createGrassEdge();
	createStreetEdge();
	createSidewalkEdge();
}

void createFrog(Frog frog)
{
	float x = frog.getX();
	float y = frog.getY();
	float z = frog.getZ();
	mySurf.setObjId(8);
	mySurf.createCube(1.0f);
	//translation(x, y, z);
	
	mySurf.setObjId(9);
	mySurf.createSphere(.5f, 16);
	//translation(x, y + 1.0f, z);
}

void createCar()
{
	mySurf.setObjId(10);
	// front left wheel
	mySurf.createTorus(0.1f, 0.5f, 16, 16);
	//translation(x-1.3f, 2.5f, z);
	//rotate(90.0f, 1.0f, 0.0f, 1.0f);
	
	// front right wheel
	//translation(x, y-0.5f, z-1.3f);
	//rotate(90.0f, 1.0f, 0.0f, 1.0f);
	
	// rear right wheel
	//translation(x+1.3f, y-0.5, z);
	//rotate(90.0f, 1.0f, 0.0f, 1.0f);
	
	// rear left wheel
	//translation(x, y-0.5f, z+1.3f);
	//rotate(90.0f, 1.0f, 0.0f, 1.0f);
	
	// body shop
	mySurf.setObjId(11);
	mySurf.createCylinder(2.0f, 1.0f, 4);
	//translation(x, y, z);
	//rotate(90.0f, 1.0f, 0.0f, -1.0f);
}

void createScene() {
	createEdges();
	createRiver();
	createLogs();
	createRoad();
	createCar();
	createFrog(frog);
}

void sendToGL(int objId) {
	glUniformMatrix4fv(modelId, 1, false, model);
	mySurf.simpleRender(objId);
	calc.setIdentityMatrix(model, 4);
}

void applyColor(float r, float g, float b){
	glUniform4f(colorId, r, g, b, 1.0);
}

void applyTransformations(int objId) {
	switch (objId)
	{
		//River
		case 0:
			translation(0.0f, 0.0f, -10.0f);
			applyColor(0.0f, 0.0f, 1.0f);
			sendToGL(objId);
			translation(10.0f, 0.0f, 0.0f);
			applyColor(0.0f, 0.0f, 1.0f);
			sendToGL(objId);
			break;

		//Road
		case 1:
			translation(-12.0f, 0.0f, 2.0f);
			applyColor(0.0f, 0.0f, 0.0f);
			sendToGL(objId);
			translation(-2.0f, 0.0f, 12.0f);
			applyColor(0.0f, 0.0f, 0.0f);
			sendToGL(objId);
			break;

		//Top Logs
		case 2:
			for (int j = 0; j < 3; j++){
				translation(2.0f + 6.5f*j, 2.75f, -15.0f + 6.5f*j);
				rotate(90.0f, 1.0f, 0.0f, -1.0f);
				applyColor(0.6f, 0.4f, 0.12f);
				sendToGL(objId);
			}
			break;

		//Middle Logs
		case 3:
			for (int j = 0; j < 3; j++){
				translation(-2.5f + 6.5f*j, 2.75f, -12.5f + 6.5f*j);
				rotate(90.0f, 1.0f, 0.0f, -1.0f);
				applyColor(0.6f, 0.4f, 0.12f);
				sendToGL(objId);
			}
			break;

		//Bottom Logs
		case 4:
			for (int j = 0; j < 3; j++){
				translation(-2.5f + 6.0f*j, 2.75f, -9.5f + 6.0f*j);
				rotate(90.0f, 1.0f, 0.0f, -1.0f);
				applyColor(0.6f, 0.4f, 0.12f);
				sendToGL(objId);
			}
			break;
		
		// Grass Edge 
		case 5:
			translation(11.0f, 0.5f, -11.0f);
			rotate(90.0f, 1.0f, 0.0f, -1.0f);
			applyColor(0.0f, 1.0f, 0.0f);
			sendToGL(objId);
			break;

		//Street Edge
		case 6:
			translation(-1.0f, .5f, 1.0f);
			rotate(90.0f, 1.0f, 0.0f, -1.0f);
			applyColor(0.25f, 0.25f, 0.25f);
			sendToGL(objId);
			break;

		// Sidewalk Edge
		case 7:
			translation(-13.0f, .5f, 13.0f);
			rotate(90.0f, 1.0f, 0.0f, -1.0f);
			applyColor(0.98f, 0.625f, 0.12f);
			sendToGL(objId);
			break;

		//Frog' Body
		case 8:
			translation(frog.getX(), frog.getY(), frog.getZ());
			applyColor(1.0f, 0.0f, 1.0f);
			sendToGL(objId);
			break;

		// Frog's Head
		case 9:
			translation(frog.getX(), frog.getY() + 1.0f, frog.getZ());
			applyColor(1.0f, 0.0f, 0.5f);
			sendToGL(objId);
			break;

			//Car's wheels  Note: Bozo Programming
		case 10:
			for (int j = 0; j < 4; j++){
				switch (j){
				case 0:
					translation(-7.3f, 2.5f, 6.0f);
					break;

				case 1:
					translation(-6.0f, 2.5f, 4.7f);
					break;

				case 2:
					translation(-4.7f, 2.5f, 6.0f);
					break;

				case 3:
					translation(-6.0f, 2.5f, 7.3f);
				}
				rotate(90.0f, 1.0f, 0.0f, 1.0f);
				applyColor(0.1f, 0.1f, 0.1f);
				sendToGL(objId);
			}
		
		//Car's Body
		case 11:
			translation(-6.0f, 3.0f, 6.0f);
			rotate(90.0f, 1.0f, 0.0f, -1.0f);
			applyColor(1.0f, 0.0f, 0.0f);
			sendToGL(objId);
			break;

		
	}
}

void renderScene()
{
	float camAdjust = -.5f;
	switch (camType)
	{
	case ORTHOGONAL:
		lookAt(0.0f, 15.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f);
		break;

	case PERSPECTIVETOP:
		lookAt(-5.0f, 40.0f, 5.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f);
		break;

	case PERSPECTIVE3RD:
		lookAt(
				frog.getX() + camX, frog.getY() + camY - camAdjust, frog.getZ() + camZ,
				frog.getX(), frog.getY(), frog.getZ(),
				0.0f, 1.0f, 0.0f);
		break;

	default:
		break;
	}
	
	updateFrog();
	glUseProgram(shader.getProgramIndex());

	glUniformMatrix4fv(viewMatrixId, 1, false, viewMatrix);
	glUniformMatrix4fv(projId, 1, false, projMatrix);

	for (int i = 0; i < mySurf.getMyMeshLength(); i++) {
		applyTransformations(i);
	}
	//Routine to create Cars
	//for (int i = 2; i < 6; i++){
	//	for (int j = -1; j < 2; j++){
	//		createCar(-2.0f*i+ j*6, 3.0f, 2.0f*i+j*6);
	//	}
	//}

	/*mySurf.createCube(1.0f);
	calc.setIdentityMatrix(model, 4);
	translation(0.0f, 2.5f, 0.0f);
	glUniformMatrix4fv(modelId, 1, false, model);
	mySurf.simpleRender();*/
	
	checkOpenGLError("ERROR: Could not draw scene.");
}

/////////////////////////////////////////////////////////////////////// Frog Movement Routines

float frogSpeed = 0.05 * 1 / 60;

void frogLeft(){
	float x = frog.getX();
	float z = frog.getZ();

	frog.setX(x-frogSpeed);
	frog.setZ(z - frogSpeed);
}

void frogRight(){
	float x = frog.getX();
	float z = frog.getZ();

	frog.setX(x + frogSpeed);
	frog.setZ(z + frogSpeed);
}

void frogForward(){
	float x = frog.getX();
	float z = frog.getZ();

	frog.setX(x + frogSpeed);
	frog.setZ(z - frogSpeed);
}

void frogBackwards(){
	float x = frog.getX();
	float z = frog.getZ();

	frog.setX(x - frogSpeed);
	frog.setZ(z + frogSpeed);
}

void updateFrog() {
	if (leftPressed == true)
		frogLeft();
	if (rightPressed == true)
		frogRight();
	if (upPressed == true)
		frogForward();
	if (downPressed == true)
		frogBackwards();
}
/////////////////////////////////////////////////////////////////////// CALLBACKS

void cleanup()
{
	destroyShaderProgram();
	destroyBufferObjects();
}

void display()
{
	++FrameCount;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderScene();
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	float ratio;
	WinX = w;
	WinY = h;
	glViewport(0, 0, WinX, WinY);

	ratio = (1.0f * WinX) / WinY;
	
	switch (camType) {
	case ORTHOGONAL:
		ortho(-30.0f, 30.0f, -20.0f, 20.0f, -30.0f, 30.0f);
		break;

	case PERSPECTIVETOP:
		perspective(53.13f, ratio, .1f, -5.0f);
		break;

	default:
		perspective(53.13f, ratio, .1f, -5.0f);
		break;
	}

}

void refresh(int value)
{
	glutPostRedisplay();
	glutTimerFunc(FPS, refresh, 0);
}

void timer(int value)
{
	std::ostringstream oss;
	oss << CAPTION << ": " << FrameCount << " FPS @ (" << WinX << "x" << WinY << ")";
	std::string s = oss.str();
	glutSetWindow(WindowHandle);
	glutSetWindowTitle(s.c_str());
	FrameCount = 0;
	glutTimerFunc(1000, timer, 0);
}

void mouseWheel(int wheel, int direction, int x, int y) {

	r += direction * 0.1f;
	if (r < 0.1f)
		r = 0.1f;

	camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camY = r *   						     sin(beta * 3.14f / 180.0f);

//  uncomment this if not using an idle func
//	glutPostRedisplay();
}

void processMouseMotion(int xx, int yy)
{
	int deltaX, deltaY;
	float alphaAux, betaAux;
	float rAux;

	deltaX =  -xx + startX;
	deltaY =   yy - startY;

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

	camX = rAux * sin(alphaAux * 3.14f / 180.0f) * cos(betaAux * 3.14f / 180.0f);
	camZ = rAux * cos(alphaAux * 3.14f / 180.0f) * cos(betaAux * 3.14f / 180.0f);
	camY = rAux *   						       sin(betaAux * 3.14f / 180.0f);

//  uncomment this if not using an idle func
//	glutPostRedisplay();
}

void processMouseButtons(int button, int state, int xx, int yy)
{
	// start tracking the mouse
	if (state == GLUT_DOWN)  {
		startX = xx;
		startY = yy;
		if (button == GLUT_LEFT_BUTTON)
			tracking = 1;
		else if (button == GLUT_RIGHT_BUTTON) {
			tracking = 2;
			GetOGLPos(xx, yy);
			float xcomp = mouseWorldP[0] - frog.getX();
			float zcomp = mouseWorldP[2] - frog.getZ();
			float xabs = abs(xcomp);
			float zabs = abs(zcomp);
			std::cout << "X: " << xcomp << std::endl << "Z: " << zcomp << std::endl;
			if (xcomp >= 0 && zcomp <= 0) {
				upPressed = true;
				if (xabs >= zabs)
					rightPressed = true;
				else
					leftPressed = true;
			}
			else {
				downPressed = true;
				if (xabs >= zabs)
					leftPressed = true;
				else
					rightPressed = true;
			}
				

			
		}
	}

	//stop tracking the mouse
	else if (state == GLUT_UP) {
		if (tracking == 1) {
			alpha -= (xx - startX);
			beta += (yy - startY);
		}
		else if (tracking == 2) {
			upPressed = false; downPressed = false; leftPressed = false; rightPressed = false;
			r += (yy - startY) * 0.01f;
			if (r < 0.1f)
				r = 0.1f;
		}
		tracking = 0;
	}
}

void processKeys(unsigned char key, int xx, int yy)
{	
	switch(key) {

	case '1':
		camType = ORTHOGONAL;
		reshape(WinX, WinY);
		break;

	case '2':
		camType = PERSPECTIVETOP;
		reshape(WinX, WinY);
		break;

	case '3':
		camType = PERSPECTIVE3RD;
		reshape(WinX, WinY);
		break;

	case 'o':
		//frogLeft();
		leftPressed = true;
		break;

	case 'p':
		//frogRight();
		rightPressed = true;
		break;

	case 'q':
		//frogForward();
		upPressed = true;
		break;

	case 'a':
		//frogBackwards();
		downPressed = true;
		break;

	case 27:
		glutLeaveMainLoop();
		break;

	case 'c': 
		printf("Camera Spherical Coordinates (%f, %f, %f)\n", alpha, beta, r);
		break;

	}
//  uncomment this if not using an idle func
//	glutPostRedisplay();
}

void processUpKeys(unsigned char key, int x, int y) {
	switch (key) {
	case 'o':
		leftPressed = false;
		break;

	case 'p':
		rightPressed = false;
		break;

	case 'q':
		upPressed = false;
		break;

	case 'a':
		downPressed = false;
		break;
	}
}

/////////////////////////////////////////////////////////////////////// SETUP

void setupCallbacks()
{
	glutCloseFunc(cleanup);
	glutIdleFunc(renderScene);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(0, timer, 0);
	glutTimerFunc(0, refresh, 0);

	//	Mouse and Keyboard Callbacks
	glutKeyboardFunc(processKeys);
	glutKeyboardUpFunc(processUpKeys);
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);
	glutMouseWheelFunc ( mouseWheel ) ;
}

void setupOpenGL() {

	// set the camera position based on its spherical coordinates
	camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camY = r *   						     sin(beta * 3.14f / 180.0f);

	std::cerr << "CONTEXT: OpenGL v" << glGetString(GL_VERSION) << std::endl;
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glDepthRange(0.0, 1.0);
	glClearDepth(1.0);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
}

void setupGLEW() {
	glewExperimental = GL_TRUE;
	GLenum result = glewInit();
	if (result != GLEW_OK) {
		std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
		exit(EXIT_FAILURE);
	}
	GLenum err_code = glGetError();
	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));
	printf("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

}

void setupGLUT(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitContextVersion(3, 1);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutInitWindowSize(WinX, WinY);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	WindowHandle = glutCreateWindow(CAPTION);
	if (WindowHandle < 1) {
		std::cerr << "ERROR: Could not create a new rendering window." << std::endl;
		exit(EXIT_FAILURE);
	}
}

void init(int argc, char* argv[])
{
	setupGLUT(argc, argv);
	setupGLEW();
	setupOpenGL();
	setupCallbacks();

	if (!setupShaders())
		exit(1);

	frog.setX(-12.5f);
	frog.setY(2.5f);
	frog.setZ(12.5f);
	createScene();

	colorId = glGetUniformLocation(shader.getProgramIndex(), "in_color");
	viewMatrixId = glGetUniformLocation(shader.getProgramIndex(), "viewMatrix");
	projId = glGetUniformLocation(shader.getProgramIndex(), "projMatrix");
	modelId = glGetUniformLocation(shader.getProgramIndex(), "model");
	
}

/////////////////////////////////////////////////////////////////////// Main

int main(int argc, char* argv[])
{
	init(argc, argv);
	glutMainLoop();
	exit(EXIT_SUCCESS);
}

