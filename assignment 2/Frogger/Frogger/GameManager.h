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


// Include LightSource object
#include "LightSource.h"

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
	void incrementSpeed();

	//libs

	VSShaderLib * _shader;
	VSResSurfRevLib * _mySurf;
	MathLib * _ml;

	// Tracking Variables
	int startX, startY, tracking;
	// Coefficient constants
	float alpha = 0.f, alphaAux = 0.f, betaAux = 20.f, beta = 20.f;
	float r = 5.25f, rAux = 5.25f;

	Frog * _frog;

	std::vector<GameObject*> * _game_objects;
	std::vector<LightSource*> *_light_sources;
	std::vector<TimberLog*> * _logs;
	std::vector<Bus*> * _bus;
	std::vector<Car*> * _cars;
	std::vector<Turtle*> * _turtles;

	float speedIncr = 0.005f;
	unsigned int FrameCount = 0;

private:
	OrthogonalCamera *_orthoCam;
	PerspectiveCamera *_perspCam;
	PerspectiveCamera *_tpCam;
	float _frogX;
	float _frogY;
	float _frogZ;
	GLErrors _gl_errors;
	GLuint viewMatrixId, projId, modelId, lightId, normal_uniformId;

	// Create Scene methods
	void createScene();
	void createFrog();
	void createRiver();
	void createRoad();
	void createRoadside();
	void createRiverside();
	void createCar();
	void createTopTimberLog();
	void createMiddleTimberLog();
	void createBottomTimberLog();
	void createBus();
	void createTopTurtles();
	void createBottomTurtles();

	void createLightsources();

	//Dynamic Objects methods
	void updateDynamicObj();
	void updateLogs();
	void updateBus();
	void updateCars();
	void updateTurtles();
	void updateFrog();

	// frog movement bools
	bool frogLeft = false;
	bool frogRight = false;
	bool frogUp = false;
	bool frogDown = false;

	// frog movement speed
	float _frog_speed = 0.005f;
};

