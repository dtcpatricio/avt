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

#include "OurMathLib.h"
#include "vsShaderLib.h"
#include "vsResSurfRevLib.h"
#include "ourScene.h"

#ifdef _WIN32
#define M_PI       3.14159265358979323846f
#endif

#define CAPTION "Frogger (Assignment 1)"

OurMathLib calc;
VSShaderLib shader;
VSResSurfRevLib mySurf;
OurScene scene;


int WinX = 640, WinY = 480;
int WindowHandle = 0;
int faceCount = 12;

const GLfloat FPS = 1000 / 60;
unsigned int FrameCount = 0;

GLfloat projMatrix[16], viewMatrix[16], model[16];

GLuint viewMatrixId, projId, modelId;

GLfloat* modelRet;

// Camera Coordinates
float camX, camY, camZ;

// Camera Spherical Coordinates
float alpha = -43.0f, beta = 48.0f;
float r = 50.0f;

// Mouse Tracking Variables
int startX, startY, tracking = 0;

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


/////////////////////////////////////////////////////////////////////// SCENE

void createRiver(){

	mySurf.createCylinder(4.0f, 10.0f, 4);
	calc.setIdentityMatrix(model, 4);
	translation(0.0f, 0.0f, -10.0f);
	glUniformMatrix4fv(modelId, 1, false, model);
	mySurf.simpleRender();

	mySurf.createCylinder(4.0f, 10.0f, 4);
	calc.setIdentityMatrix(model, 4);
	translation(10.0f, 0.0f, 0.0f);
	glUniformMatrix4fv(modelId, 1, false, model);
	mySurf.simpleRender();

	
}

void createRoad(){
	mySurf.createCylinder(4.0f, 10.0f, 4);
	calc.setIdentityMatrix(model, 4);
	translation(-10.0f, 0.0f, 0.0f);
	glUniformMatrix4fv(modelId, 1, false, model);
	mySurf.simpleRender();

	mySurf.createCylinder(4.0f, 10.0f, 4);
	calc.setIdentityMatrix(model, 4);
	translation(0.0f, 0.0f, 10.0f);
	glUniformMatrix4fv(modelId, 1, false, model);
	mySurf.simpleRender();
}

void createTopLogs(){

	//left 
	mySurf.createCylinder(6.0f, 1.0f, 8);
	calc.setIdentityMatrix(model, 4);
	translation(0.0f, 3.0f, -15.0f);
	rotate(90.0f, 1.0f, 0.0f, -1.0f);
	glUniformMatrix4fv(modelId, 1, false, model);
	mySurf.simpleRender();

	//middle 
	mySurf.createCylinder(7.0f, 1.0f, 8);
	calc.setIdentityMatrix(model, 4);
	translation(7.5f, 3.0f, -7.5f);
	rotate(90.0f, 1.0f, 0.0f, -1.0f);
	glUniformMatrix4fv(modelId, 1, false, model);
	mySurf.simpleRender();

	//right
	mySurf.createCylinder(4.0f, 1.0f, 8);
	calc.setIdentityMatrix(model, 4);
	translation(15.0f, 3.0f, 0.0f);
	rotate(90.0f, 1.0f, 0.0f, -1.0f);
	glUniformMatrix4fv(modelId, 1, false, model);
	mySurf.simpleRender();

}

void middleRowLogs(){

	//left
	mySurf.createCylinder(7.0f, 1.0f, 8);
	calc.setIdentityMatrix(model, 4);
	translation(-2.5f, 3.0f, -12.5f);
	rotate(90.0f, 1.0f, 0.0f, -1.0f);
	glUniformMatrix4fv(modelId, 1, false, model);
	mySurf.simpleRender();

	//middle
	mySurf.createCylinder(4.0f, 1.0f, 8);
	calc.setIdentityMatrix(model, 4);
	translation(5.0f, 3.0f, -5.0f);
	rotate(90.0f, 1.0f, 0.0f, -1.0f);
	glUniformMatrix4fv(modelId, 1, false, model);
	mySurf.simpleRender();

	//right
	mySurf.createCylinder(6.0f, 1.0f, 8);
	calc.setIdentityMatrix(model, 4);
	translation(11.5f, 3.0f, 1.5f);
	rotate(90.0f, 1.0f, 0.0f, -1.0f);
	glUniformMatrix4fv(modelId, 1, false, model);
	mySurf.simpleRender();
}

void bottomRowLogs(){

	//left done
	mySurf.createCylinder(4.0f, 1.0f, 8);
	calc.setIdentityMatrix(model, 4);
	translation(-5.0f, 3.0f, -10.0f);
	rotate(90.0f, 1.0f, 0.0f, -1.0f);
	glUniformMatrix4fv(modelId, 1, false, model);
	mySurf.simpleRender();

	//middle
	mySurf.createCube(1.0f);
	calc.setIdentityMatrix(model, 4);
	translation(2.5f, 2.5f, -2.5f);
	glUniformMatrix4fv(modelId, 1, false, model);
	mySurf.simpleRender();

	//right
	mySurf.createCube(1.0f);
	calc.setIdentityMatrix(model, 4);
	translation(5.0f, 2.5f, 0.0f);
	glUniformMatrix4fv(modelId, 1, false, model);
	mySurf.simpleRender();
}

void createLogs(){

	createTopLogs();
	middleRowLogs();
	bottomRowLogs();
}

void renderScene()
{

	lookAt(camX, camY, camZ, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	glUseProgram(shader.getProgramIndex());

	glUniformMatrix4fv(viewMatrixId, 1, false, viewMatrix);
	glUniformMatrix4fv(projId, 1, false, projMatrix);

	createRiver();

	createLogs();

	createRoad();

	mySurf.createCube(1.0f);
	calc.setIdentityMatrix(model, 4);
	translation(0.0f, 2.5f, 0.0f);
	glUniformMatrix4fv(modelId, 1, false, model);
	mySurf.simpleRender();
	
	checkOpenGLError("ERROR: Could not draw scene.");
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
	perspective(53.13f, ratio, .1f, -5.0f);

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

	deltaX =  - xx + startX;
	deltaY =    yy - startY;

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
		else if (button == GLUT_RIGHT_BUTTON)
			tracking = 2;
	}

	//stop tracking the mouse
	else if (state == GLUT_UP) {
		if (tracking == 1) {
			alpha -= (xx - startX);
			beta += (yy - startY);
		}
		else if (tracking == 2) {
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

	//createScene();

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

