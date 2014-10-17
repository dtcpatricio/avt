// Frogger.cpp : Defines the entry point for the console application.

#include "Frogger.h"
#include "Scene.h"
#include "io.h"
#include "Cam.h"
#include "Frog.h"
#include "OurMathLib.h"

#define CAPTION "Frogger (Assignment 1)"

VSShaderLib shader;
Scene scene(shader);

int WindowHandle = 0;

const GLfloat FPS = 1000 / 60;
unsigned int FrameCount = 0;

GLfloat color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

/////////////////////////////////////////////////////////////////////// ERRORS

bool isOpenGLError()
{
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

/////////////////////////////////////////////////////////////////////// SHADERS

GLuint setupShaders()
{
	shader.init();
	shader.loadShader(VSShaderLib::VERTEX_SHADER,   "assign1.vert");
	shader.loadShader(VSShaderLib::FRAGMENT_SHADER, "assign1.frag");

	shader.setProgramOutput(0, "outFrag");
	shader.setVertexAttribName(VSShaderLib::VERTEX_COORD_ATTRIB,  "in_pos");
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

/////////////////////////////////////////////////////////////////////// SCENE

void renderScene()
{
	glUseProgram(shader.getProgramIndex());

	scene.update();
	
	//checkOpenGLError("ERROR: Could not draw scene.");
}

/////////////////////////////////////////////////////////////////////// CALLBACKS

void mouseWheel(int wheel, int direction, int x, int y) {
	scene.getIO()->mouseWheel(wheel, direction, x, y);
}
void processMouseMotion(int xx, int yy) {
	scene.getIO()->processMouseMotion(xx, yy);
}
void processMouseButtons(int button, int state, int xx, int yy) {
	scene.getIO()->processMouseButtons(button, state, xx, yy);
}
void processKeys(unsigned char key, int xx, int yy) {
	scene.getIO()->processKeys(key, xx, yy);
}
void processUpKeys(unsigned char key, int x, int y) {
	scene.getIO()->processUpKeys(key, x, y);
}

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

void reshape(int w, int h) {
	scene.getIO()->reshape(w, h);
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

/////////////////////////////////////////////////////////////////////// SETUP

void setupCallbacks()
{
	glutCloseFunc  (cleanup);
	glutIdleFunc   (renderScene);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	
	glutTimerFunc  (0, timer, 0);
	glutTimerFunc  (0, refresh, 0);

	// Mouse and Keyboard
	glutKeyboardFunc  (processKeys);
	glutKeyboardUpFunc(processUpKeys);
	glutMouseFunc     (processMouseButtons);
	glutMotionFunc    (processMouseMotion);
	glutMouseWheelFunc(mouseWheel);
}

void setupOpenGL()
{
	/*	cam.update() */

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

void setupGLEW()
{
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

	scene.create();
}

/////////////////////////////////////////////////////////////////////// Main

int main(int argc, char* argv[])
{
	init(argc, argv);
	glutMainLoop();
	exit(EXIT_SUCCESS);
}