#include "Scene.h"
#include "IO.h"
#include "Cam.h"
#include "Frog.h"
#include "OurMathLib.h"

Scene::Scene(VSShaderLib &shader) : _shader(shader) {
	_io   = new IO(this);
	_cam  = new Cam(this);
	_frog = new Frog(this);
	_calc = new OurMathLib(this);
}

IO              *Scene::getIO()     { return _io;     }
Cam             *Scene::getCam()    { return _cam;    }
Frog            *Scene::getFrog()   { return _frog;   }
OurMathLib      *Scene::getCalc()   { return _calc;   }
VSShaderLib      Scene::getShader() { return _shader; }
VSResSurfRevLib  Scene::getSurf()   { return _mySurf; }

void Scene::initIDs() {
	colorId      = glGetUniformLocation(_shader.getProgramIndex(), "in_color");
	modelId      = glGetUniformLocation(_shader.getProgramIndex(), "model");
	viewMatrixId = glGetUniformLocation(_shader.getProgramIndex(), "viewMatrix");
	projId       = glGetUniformLocation(_shader.getProgramIndex(), "projMatrix");
}

void Scene::create() {
	initIDs();

	_cam->update();

	_frog->setX(-12.5f);
	_frog->setY(2.5f);
	_frog->setZ(12.5f);

	createEdges();
	createRiver();
	createLogs();
	createRoad();
	createCar();
	createFrog(*_frog);
}

void Scene::createRiver(){
	_mySurf.setObjId(0);
	_mySurf.createCylinder(4.0f, 10.0f, 4);
}

void Scene::createRoad(){
	_mySurf.setObjId(1);
	_mySurf.createCylinder(4.0f, 10.0f, 4);
}

void Scene::createTopLogs(){
	_mySurf.setObjId(2);
	_mySurf.createCylinder(6.0f, 0.75f, 8);
}

void Scene::middleRowLogs(){
	_mySurf.setObjId(3);
	_mySurf.createCylinder(5.0f, 0.75f, 8);
}

void Scene::bottomRowLogs(){
	_mySurf.setObjId(4);
	_mySurf.createCylinder(5.0f, 0.75f, 8);
}

void Scene::createLogs(){
	createTopLogs();
	middleRowLogs();
	bottomRowLogs();
}

void Scene::createGrassEdge()
{
	_mySurf.setObjId(5);
	_mySurf.createCylinder(29.0f, 2.0f, 4);
}

void Scene::createStreetEdge()
{
	_mySurf.setObjId(6);
	_mySurf.createCylinder(29.0f, 2.0f, 4);
}

void Scene::createSidewalkEdge()
{
	_mySurf.setObjId(7);
	_mySurf.createCylinder(29.0f, 2.0f, 4);
}

void Scene::createEdges()
{
	createGrassEdge();
	createStreetEdge();
	createSidewalkEdge();
}

void Scene::createFrog(Frog frog)
{
	float x = frog.getX();
	float y = frog.getY();
	float z = frog.getZ();
	_mySurf.setObjId(8);
	_mySurf.createCube(1.0f);

	_mySurf.setObjId(9);
	_mySurf.createSphere(.5f, 16);
}

void Scene::createCar()
{
	_mySurf.setObjId(10);

	// front left wheel
	_mySurf.createTorus(0.1f, 0.5f, 16, 16);
	
	// body shop
	_mySurf.setObjId(11);
	_mySurf.createCylinder(2.0f, 1.0f, 4);
}

void Scene::update() {
	_frog->draw();
	_cam->lookAt();

	applySurfaceTransformations();
}

void Scene::applySurfaceTransformations() {
	glUniformMatrix4fv(viewMatrixId, 1, false, _calc->getViewMatrix());
	glUniformMatrix4fv(projId,       1, false, _calc->getProjMatrix());
	
	for (int i = 0; i < _mySurf.getMyMeshLength(); i++) {
		applyTransformations(i);
	}
}

void Scene::sendToGL(int objId) {
	GLuint modelId = glGetUniformLocation(_shader.getProgramIndex(), "model");
	glUniformMatrix4fv(modelId, 1, false, _calc->getModel());
	_mySurf.simpleRender(objId);
	_calc->setIdentityMatrix(_calc->getModel(), 4);
}


void Scene::applyColor(float r, float g, float b){

	GLuint colorId = glGetUniformLocation(_shader.getProgramIndex(), "in_color");
	glUniform4f(colorId, r, g, b, 1.0);
}

void Scene::applyTransformations(int objId) {
	switch (objId) {
	//River
	case 0:
		_calc->translation(0.0f, 0.0f, -10.0f);
		applyColor(0.0f, 0.0f, 1.0f);
		sendToGL(objId);
		_calc->translation(10.0f, 0.0f, 0.0f);
		applyColor(0.0f, 0.0f, 1.0f);
		sendToGL(objId);
		break;

		//Road
	case 1:
		_calc->translation(-12.0f, 0.0f, 2.0f);
		applyColor(0.0f, 0.0f, 0.0f);
		sendToGL(objId);
		_calc->translation(-2.0f, 0.0f, 12.0f);
		applyColor(0.0f, 0.0f, 0.0f);
		sendToGL(objId);
		break;

		//Top Logs
	case 2:
		for (int j = 0; j < 3; j++){
			_calc->translation(2.0f + 6.5f*j, 2.75f, -15.0f + 6.5f*j);
			_calc->rotate(90.0f, 1.0f, 0.0f, -1.0f);
			applyColor(0.6f, 0.4f, 0.12f);
			sendToGL(objId);
		}
		break;

		//Middle Logs
	case 3:
		for (int j = 0; j < 3; j++){
			_calc->translation(-2.5f + 6.5f*j, 2.75f, -12.5f + 6.5f*j);
			_calc->rotate(90.0f, 1.0f, 0.0f, -1.0f);
			applyColor(0.6f, 0.4f, 0.12f);
			sendToGL(objId);
		}
		break;

		//Bottom Logs
	case 4:
		for (int j = 0; j < 3; j++){
			_calc->translation(-2.5f + 6.0f*j, 2.75f, -9.5f + 6.0f*j);
			_calc->rotate(90.0f, 1.0f, 0.0f, -1.0f);
			applyColor(0.6f, 0.4f, 0.12f);
			sendToGL(objId);
		}
		break;

		// Grass Edge 
	case 5:
		_calc->translation(11.0f, 0.5f, -11.0f);
		_calc->rotate(90.0f, 1.0f, 0.0f, -1.0f);
		applyColor(0.0f, 1.0f, 0.0f);
		sendToGL(objId);
		break;

		//Street Edge
	case 6:
		_calc->translation(-1.0f, .5f, 1.0f);
		_calc->rotate(90.0f, 1.0f, 0.0f, -1.0f);
		applyColor(0.25f, 0.25f, 0.25f);
		sendToGL(objId);
		break;

		// Sidewalk Edge
	case 7:
		_calc->translation(-13.0f, .5f, 13.0f);
		_calc->rotate(90.0f, 1.0f, 0.0f, -1.0f);
		applyColor(0.98f, 0.625f, 0.12f);
		sendToGL(objId);
		break;

		//Frog' Body
	case 8:
		_calc->translation(_frog->getX(), _frog->getY(), _frog->getZ());
		applyColor(1.0f, 0.0f, 1.0f);
		sendToGL(objId);
		break;

		// Frog's Head
	case 9:
		_calc->translation(_frog->getX(), _frog->getY() + 1.0f, _frog->getZ());
		applyColor(1.0f, 0.0f, 0.5f);
		sendToGL(objId);
		break;

		//Car's wheels  Note: Bozo Programming
	case 10:
		for (int j = 0; j < 4; j++){
			switch (j){
			case 0:
				_calc->translation(-7.3f, 2.5f, 6.0f);
				break;

			case 1:
				_calc->translation(-6.0f, 2.5f, 4.7f);
				break;

			case 2:
				_calc->translation(-4.7f, 2.5f, 6.0f);
				break;

			case 3:
				_calc->translation(-6.0f, 2.5f, 7.3f);
			}
			_calc->rotate(90.0f, 1.0f, 0.0f, 1.0f);
			applyColor(0.1f, 0.1f, 0.1f);
			sendToGL(objId);
		}

		//Car's Body
	case 11:
		_calc->translation(-6.0f, 3.0f, 6.0f);
		_calc->rotate(90.0f, 1.0f, 0.0f, -1.0f);
		applyColor(1.0f, 0.0f, 0.0f);
		sendToGL(objId);
		break;
	}
}