#pragma once

#include "vsResSurfRevLib.h"
#include "vsShaderLib.h"

// Include game objects
#include "Frog.h"
#include "Road.h"
#include "Bus.h"
#include "Car.h"
#include "Roadside.h"
#include "River.h"
#include "Riverside.h"
#include "TimberLog.h"
#include "Turtle.h"

#include "PerspectiveCamera.h"
#include "OrthogonalCamera.h"
#include "GLErrors.h"
#include <GL/glew.h>
#include <GL/freeglut.h>

#define WinX 640
#define WinY 480

class GameManager
{
public:
	GameManager();
	~GameManager();

	// Callbacks - Input
	void keyPressed(unsigned char key, int xx, int yy);
	void keyUp(unsigned char key, int xx, int yy);
	void mouseButtons(int button, int state, int xx, int yy);
	void mouseMotion(int xx, int yy);
	void mouseWheel(int wheel, int direction, int xx, int yy);

	// Callbacks - rendering
	void display();
	void reshape(GLsizei w, GLsizei h);
	void renderScene();
	void idle();
	void update();
	void init();

	// Callbacks - timers
	void cleanup();

	//Setup Cameras
	void createCameras();

	// Enumerator for camera type
	enum CamType { ORTHOGONAL, PERSPECTIVETOP, PERSPECTIVE3RD };

	// Camera type
	CamType camType = ORTHOGONAL;

	// Setup functions
	GLuint setupShaders();
	void destroyShaderProgram(); // TODO: to remove to another file
	void destroyBufferObjects(); // TODO: to remove to another file

	// Aux functions
	void applySurfaceTransformations();

	//libs

	VSShaderLib * _shader;
	VSResSurfRevLib * _mySurf;
	MathLib * _ml;

	// Tracking Variables
	int startX, startY, tracking = 0;
	// Coefficient constants
	float r, alpha, beta;

	Frog * _frog;
	std::vector<GameObject*> * _game_objects;

	unsigned int FrameCount = 0;

private:
	OrthogonalCamera *_orthoCam;
	PerspectiveCamera *_perspCam;
	PerspectiveCamera *_tpCam;
	float _frogX;
	float _frogY;
	float _frogZ;
	GLErrors _gl_errors;
	GLuint viewMatrixId, projId, modelId, colorId;

	void createScene();
	void createFrog();
	void createRiver();
	void createRoad();
	void createRoadside();
	void createRiverside();
	void createCar();
	void createTimberlog();
	void createBus();
	void createTurtle();
};

