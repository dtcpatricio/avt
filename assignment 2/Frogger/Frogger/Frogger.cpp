// Frogger.cpp : Defines the entry point for the console application.

#include "Frogger.h"

#define CAPTION "Frogger (Assignment 1)"

struct MyWrapper {
	static void display(void) {
		manager->display();
	}
	static void renderScene(void) {
		manager->renderScene();
	}
	static void reshape(int x, int y) {
		manager->reshape(x, y);
	}
	static void keyPressed(unsigned char key, int xx, int yy) {
		manager->keyPressed(key, xx, yy);
	}
	static void keyUp(unsigned char key, int xx, int yy) {
		manager->keyUp(key, xx, yy);
	}
	static void mouseMotion(int xx, int yy) {
		manager->mouseMotion(xx, yy);
	}
	static void mouseWheel(int wheel, int direction, int x, int y) {
		manager->mouseWheel(wheel, direction, x, y);
	}
	static void mouseButtons(int button, int state, int xx, int yy) {
		manager->mouseButtons(button, state, xx, yy);
	}
	static void iterate(int value){
		manager->iterate(value);
	}
	static GameManager *manager;
};

GameManager* MyWrapper::manager = 0;

GLErrors error;

int WindowHandle = 0;

GLfloat color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

/////////////////////////////////////////////////////////////////////// VAOs & VBOs

void destroyBufferObjects()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	error.checkOpenGLError("ERROR: Could not destroy VAOs and VBOs.");
}

/////////////////////////////////////////////////////////////////////// CALLBACKS

void cleanup()
{
	MyWrapper::manager->destroyShaderProgram();
	destroyBufferObjects();
}

void
refresh(int value)
{
	glutPostRedisplay();
	glutTimerFunc(FPS, refresh, 0);
}

void onTimer(int value)
{
	std::ostringstream oss;
	oss << CAPTION << ": " << MyWrapper::manager->FrameCount << " FPS @ (" << WinX << "x" << WinY << ")";
	std::string s = oss.str();
	glutSetWindow(WindowHandle);
	glutSetWindowTitle(s.c_str());
	MyWrapper::manager->FrameCount = 0;
	glutTimerFunc(1000, onTimer, 0);
}

/////////////////////////////////////////////////////////////////////// SETUP

void setupCallbacks()
{
	glutCloseFunc  (cleanup);
	glutDisplayFunc(MyWrapper::display);
	glutReshapeFunc(MyWrapper::reshape);
	
	glutTimerFunc  (0, onTimer, 0);
	glutTimerFunc  (0, refresh, 0);
	glutTimerFunc(0, MyWrapper::iterate, 0);

	// Mouse and Keyboard
	glutKeyboardFunc  (MyWrapper::keyPressed);
	glutKeyboardUpFunc(MyWrapper::keyUp);
	glutMouseFunc(MyWrapper::mouseButtons);
	glutMotionFunc(MyWrapper::mouseMotion);
	glutMouseWheelFunc(MyWrapper::mouseWheel);
}

void setupOpenGL()
{
	std::cerr << "CONTEXT: OpenGL v" << glGetString(GL_VERSION) << std::endl;
	glClearColor(0.5f, 0.6f, 0.7f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glDepthRange(0.0, 1.0);
	glClearDepth(1.0);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_MULTISAMPLE);
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

	glutInitContextVersion(3, 2);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutInitWindowSize(WinX, WinY);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
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
}

/////////////////////////////////////////////////////////////////////// Main

int main(int argc, char* argv[])
{
	init(argc, argv);
	MyWrapper::manager = new GameManager();
	MyWrapper::manager->init();
	
}
