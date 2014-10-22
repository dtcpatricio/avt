#include "GameManager.h"


GameManager::GameManager()
{}


GameManager::~GameManager()
{}

/////////////////////////////////////////////////////////////////////// CALLBACKS

/////////////////////////////////////////////////////////////////////// INPUT
void
GameManager::mouseWheel(int wheel, int direction, int x, int y) {
	//Cam *cam = _scene->getCam();

	//float r = cam->getR();
	//r += direction * 0.1f;
	//if (r < 0.1f) {
	//	r = 0.1f;
	//}

	//cam->setR(r);
	//cam->update(cam->getR(), cam->getA(), cam->getB());
}

void
GameManager::mouseMotion(int xx, int yy) {
	int   deltaX = -xx + startX, deltaY = yy - startY;
	float alphaAux, betaAux;
	float rAux;

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

	//cam->update(rAux, alphaAux, betaAux);
}
void
GameManager::mouseButtons(int button, int state, int xx, int yy)
{
	//// start tracking the mouse
	//if (state == GLUT_DOWN)  {
	//	startX = xx;
	//	startY = yy;
	//	if (button == GLUT_LEFT_BUTTON)
	//		tracking = 1;
	//	else if (button == GLUT_RIGHT_BUTTON) {
	//		tracking = 2;
	//		Vector3 mousePos = _ml->GetOGLPos(xx, yy);
	//		float xcomp = mousePos.getX() - _frog->getPosition()->getX();
	//		float zcomp = mousePos.getZ() - _frog->getPosition()->getZ();
	//		float xabs = abs(xcomp);
	//		float zabs = abs(zcomp);
	//		std::cout << "X: " << xcomp << std::endl << "Z: " << zcomp << std::endl;
	//		if (xcomp >= 0 && zcomp <= 0) {
	//			//_scene->getFrog()->setUp(true);
	//			if (xabs >= zabs)
	//				//_scene->getFrog()->setRight(true);
	//			else
	//				//_scene->getFrog()->setLeft(true);
	//		}
	//		else {
	//			//_scene->getFrog()->setDown(true);
	//			if (xabs >= zabs)
	//				//_scene->getFrog()->setLeft(true);
	//			else
	//				//_scene->getFrog()->setRight(true);
	//		}
	//	}
	//}

	////stop tracking the mouse
	//else if (state == GLUT_UP) {
	//	if (tracking == 1) {
	//		float alpha = _scene->getCam()->getA() - (xx - startX);
	//		_scene->getCam()->setA(alpha);
	//		float beta = _scene->getCam()->getB() + (yy - startY);
	//		_scene->getCam()->setB(beta);
	//	}
	//	else if (tracking == 2) {
	//		_scene->getFrog()->setUp(false);
	//		_scene->getFrog()->setDown(false);
	//		_scene->getFrog()->setLeft(false);
	//		_scene->getFrog()->setRight(false);

	//		float r = _scene->getCam()->getR();
	//		r += (yy - startY) * 0.01f;
	//		if (r < 0.1f)
	//			r = 0.1f;
	//		_scene->getCam()->setR(r);
	//	}
	//	tracking = 0;
	//}
}
void
GameManager::keyPressed(unsigned char key, int xx, int yy) {
	switch (key) {

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
		//_frog->setLeft(true);
		break;

	case 'p':
		//_frog->setRight(true);
		break;

	case 'q':
		//_frog->setUp(true);
		break;

	case 'a':
		//_frog->setDown(true);
		break;

	case 27:
		glutLeaveMainLoop();
		break;

	case 'c':
		/*printf("Camera Spherical Coordinates (%f, %f, %f)\n"
			, _scene->getCam()->getA(), _scene->getCam()->getB(), _scene->getCam()->getR());*/
		break;
	}
}

void
GameManager::keyUp(unsigned char key, int xx, int yy) {
	switch (key) {
	case 'o':
		//_scene->getFrog()->setLeft(false);
		break;

	case 'p':
		//_scene->getFrog()->setRight(false);
		break;

	case 'q':
		//_scene->getFrog()->setUp(false);
		break;

	case 'a':
		//_scene->getFrog()->setDown(false);
		break;
	}
}

//////////////////////////////////////////////////////////////////////// RENDERING
void
GameManager::display()
{
	++FrameCount;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderScene();
	glutSwapBuffers();
}

void
GameManager::reshape(int w, int h) {	
		
	switch (camType){
		case ORTHOGONAL: 
			_orthoCam->computeProjectionMatrix();
			break;
		case PERSPECTIVETOP	: 
			_perspCam->computeProjectionMatrix();
			break;
		case PERSPECTIVE3RD: 
			_tpCam->computeProjectionMatrix();
			break;
	}
		
}

/////////////////////////////////////////////////////////////////////// CALLBACKS - TIMER

void
GameManager::cleanup()
{
	destroyShaderProgram();
	destroyBufferObjects();
}

void
GameManager::destroyBufferObjects()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	_gl_errors.checkOpenGLError("ERROR: Could not destroy VAOs and VBOs.");
}

/////////////////////////////////////////////////////////////////////// SCENE

void
GameManager::renderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	switch (camType)
	{
	case ORTHOGONAL:
		_orthoCam->computeVisualizationMatrix();
		break;
	case PERSPECTIVETOP:
		_perspCam->computeVisualizationMatrix();
		break;
	case PERSPECTIVE3RD:
		_tpCam->updateAt(_frogX, _frogY, _frogZ);
		_tpCam->computeVisualizationMatrix();
		break;
	}
		
	glUseProgram(_shader->getProgramIndex());
	glUniformMatrix4fv(viewMatrixId, 1, false, _ml->getViewMatrix());
	glUniformMatrix4fv(projId, 1, false, _ml->getProjMatrix());

	std::vector<GameObject*>::iterator it_obj;
	for (it_obj = _game_objects->begin(); it_obj != _game_objects->end(); it_obj++)
	{
		(*it_obj)->draw();
	}


	_gl_errors.checkOpenGLError("ERROR: Could not draw scene.");
}
//
//void
//GameManager::applySurfaceTransformations() {
//
//	glUniformMatrix4fv(viewMatrixId, 1, false, _calc->getViewMatrix());
//	glUniformMatrix4fv(projId, 1, false, _calc->getProjMatrix());
//
//	for (int i = 0; i < _mySurf->getMyMeshLength(); i++) {
//		applyTransformations(i);
//	}
//}
//
//
//void
//GameManager::applyColor(float r, float g, float b){
//
//	GLuint colorId = glGetUniformLocation(_shader.getProgramIndex(), "in_color");
//	glUniform4f(colorId, r, g, b, 1.0);
//}
//
//
//void Scene::applyTransformations(int objId) {
//	switch (objId) {
//		//River
//	case 0:
//		_calc->translation(0.0f, 0.0f, -10.0f);
//		applyColor(0.0f, 0.0f, 1.0f);
//		sendToGL(objId);
//		_calc->translation(10.0f, 0.0f, 0.0f);
//		applyColor(0.0f, 0.0f, 1.0f);
//		sendToGL(objId);
//		break;
//
//		//Road
//	case 1:
//		_calc->translation(-12.0f, 0.0f, 2.0f);
//		applyColor(0.0f, 0.0f, 0.0f);
//		sendToGL(objId);
//		_calc->translation(-2.0f, 0.0f, 12.0f);
//		applyColor(0.0f, 0.0f, 0.0f);
//		sendToGL(objId);
//		break;
//
//		//Top Logs
//	case 2:
//		for (int j = 0; j < 3; j++){
//			_calc->translation(2.0f + 6.5f*j, 2.75f, -15.0f + 6.5f*j);
//			_calc->rotate(90.0f, 1.0f, 0.0f, -1.0f);
//			applyColor(0.6f, 0.4f, 0.12f);
//			sendToGL(objId);
//		}
//		break;
//
//		//Middle Logs
//	case 3:
//		for (int j = 0; j < 3; j++){
//			_calc->translation(-2.5f + 6.5f*j, 2.75f, -12.5f + 6.5f*j);
//			_calc->rotate(90.0f, 1.0f, 0.0f, -1.0f);
//			applyColor(0.6f, 0.4f, 0.12f);
//			sendToGL(objId);
//		}
//		break;
//
//		//Bottom Logs
//	case 4:
//		for (int j = 0; j < 3; j++){
//			_calc->translation(-2.5f + 6.0f*j, 2.75f, -9.5f + 6.0f*j);
//			_calc->rotate(90.0f, 1.0f, 0.0f, -1.0f);
//			applyColor(0.6f, 0.4f, 0.12f);
//			sendToGL(objId);
//		}
//		break;
//
//		// Grass Edge 
//	case 5:
//		_calc->translation(11.0f, 0.5f, -11.0f);
//		_calc->rotate(90.0f, 1.0f, 0.0f, -1.0f);
//		applyColor(0.0f, 1.0f, 0.0f);
//		sendToGL(objId);
//		break;
//
//		//Street Edge
//	case 6:
//		_calc->translation(-1.0f, .5f, 1.0f);
//		_calc->rotate(90.0f, 1.0f, 0.0f, -1.0f);
//		applyColor(0.25f, 0.25f, 0.25f);
//		sendToGL(objId);
//		break;
//
//		// Sidewalk Edge
//	case 7:
//		_calc->translation(-13.0f, .5f, 13.0f);
//		_calc->rotate(90.0f, 1.0f, 0.0f, -1.0f);
//		applyColor(0.98f, 0.625f, 0.12f);
//		sendToGL(objId);
//		break;
//
//		//Frog' Body
//	case 8:
//		_calc->translation(_frog->getX(), _frog->getY(), _frog->getZ());
//		applyColor(1.0f, 0.0f, 1.0f);
//		sendToGL(objId);
//		break;
//
//		// Frog's Head
//	case 9:
//		_calc->translation(_frog->getX(), _frog->getY() + 1.0f, _frog->getZ());
//		applyColor(1.0f, 0.0f, 0.5f);
//		sendToGL(objId);
//		break;
//
//		//Car's wheels  Note: Bozo Programming
//	case 10:
//		for (int j = 0; j < 4; j++){
//			switch (j){
//			case 0:
//				_calc->translation(-7.3f, 2.5f, 6.0f);
//				break;
//
//			case 1:
//				_calc->translation(-6.0f, 2.5f, 4.7f);
//				break;
//
//			case 2:
//				_calc->translation(-4.7f, 2.5f, 6.0f);
//				break;
//
//			case 3:
//				_calc->translation(-6.0f, 2.5f, 7.3f);
//			}
//			_calc->rotate(90.0f, 1.0f, 0.0f, 1.0f);
//			applyColor(0.1f, 0.1f, 0.1f);
//			sendToGL(objId);
//		}
//
//		//Car's Body
//	case 11:
//		_calc->translation(-6.0f, 3.0f, 6.0f);
//		_calc->rotate(90.0f, 1.0f, 0.0f, -1.0f);
//		applyColor(1.0f, 0.0f, 0.0f);
//		sendToGL(objId);
//		break;
//	}
//}

void 
GameManager::createCameras(){
	
	float winx = WinX;
	float winy = WinY;
	float ratio = (1.0f *winx) / winy;

	_frogX = _frog->getPosition()->getX();
	_frogY = _frog->getPosition()->getY();
	_frogZ = _frog->getPosition()->getZ();

	_orthoCam = new OrthogonalCamera(-25.0f, 25.0f, -25.0f, 25.0f, -25.0f, 25.0f, _ml);
	_orthoCam->setPosition(0.0f, 25.0f, 0.0f);
	_orthoCam->update();

	_perspCam = new PerspectiveCamera(53.13f, ratio, 0.1f, -5.0f, _ml);
	_perspCam->setPosition(0.0f, 30.0f, 0.0f);

	_tpCam = new PerspectiveCamera(53.13f, ratio, 0.1f, -5.0f, _ml);
	_tpCam->setPosition(_frogX, _frogY, _frogZ);


}
/////////////////////////////////////////////////////////////////////// SHADERS

GLuint
GameManager::setupShaders()
{
	_shader->init();
	_shader->loadShader(VSShaderLib::VERTEX_SHADER, "assign1.vert");
	_shader->loadShader(VSShaderLib::FRAGMENT_SHADER, "assign1.frag");

	_shader->setProgramOutput(0, "outFrag");
	_shader->setVertexAttribName(VSShaderLib::VERTEX_COORD_ATTRIB, "in_pos");
	//_shader->setVertexAttribName(VSShaderLib::TEXTURE_COORD_ATTRIB, "in_color");
	_shader->prepareProgram();

	return(_shader->isProgramValid());
}

void 
GameManager::destroyShaderProgram()
{
	glUseProgram(0);
	glDetachShader(_shader->getProgramIndex(), _shader->getShaderIndex(_shader->VERTEX_SHADER));
	glDetachShader(_shader->getProgramIndex(), _shader->getShaderIndex(_shader->FRAGMENT_SHADER));

	glDeleteShader(_shader->getShaderIndex(_shader->FRAGMENT_SHADER));
	glDeleteShader(_shader->getShaderIndex(_shader->VERTEX_SHADER));
	glDeleteProgram(_shader->getProgramIndex());

	_gl_errors.checkOpenGLError("ERROR: Could not destroy shaders.");
}

//////////////////////////////////////////////////////////////////////// CREATE OBJECTS
void
GameManager::createScene()
{
	createRiver();
	createFrog();
	createRoad();
	createRoadside();
	createRiverside();
	createBus();
	createCar();
	createTimberlog();
}

void
GameManager::createRiver()
{
	River * river = new River(_mySurf, _shader, _ml);
	river->setPosition(-15.0f, 0.0f, -15.0f);
	river->create();
	_game_objects->push_back(river);
}

void
GameManager::createFrog()
{
	_frog = new Frog(_mySurf, _shader, _ml);
	_frog->setPosition(-16.0f, 2.0f, 19.0f);
	_frog->create();
	_game_objects->push_back(_frog);
}

void
GameManager::createRoad()
{
	Road * road = new Road(_mySurf, _shader, _ml);
	road->setPosition(-15.0f, 0.0f, 4.0f);
	road->create();
	_game_objects->push_back(road);
}

void
GameManager::createRoadside()
{
	Roadside * roadside = new Roadside(_mySurf, _shader, _ml);
	roadside->setPosition(-15.0f, 0.0f, 19.0f);
	roadside->create();
	_game_objects->push_back(roadside);
}

void
GameManager::createRiverside()
{
	Riverside * riverside = new Riverside(_mySurf, _shader, _ml);
	riverside->setPosition(-15.0f, 0.0f, 0.0f);
	riverside->create();
	_game_objects->push_back(riverside);

	Riverside * riverside2 = new Riverside(_mySurf, _shader, _ml);
	riverside2->setPosition(-15.0f, 0.0f, -19.0f);
	_game_objects->push_back(riverside2);
}

void
GameManager::createBus()
{
	Bus * bus = new Bus(_mySurf, _shader, _ml);
	bus->setPosition(0.0f, 2.0f, 5.0f);
	bus->create();
	_game_objects->push_back(bus);
}

void
GameManager::createCar()
{
	Car * car = new Car(_mySurf, _shader, _ml);
	car->setPosition(0.0f, 2.0f, 10.0f);
	car->create();
	_game_objects->push_back(car);
}

void
GameManager::createTimberlog()
{
	TimberLog * tl = new TimberLog(_mySurf, _shader, _ml);
	tl->setPosition(0.0f, 2.0f, -5.0f);
	tl->create();
	_game_objects->push_back(tl);

	TimberLog * tl2 = new TimberLog(_mySurf, _shader, _ml);
	tl2->setPosition(-7.0f, 2.0f, -5.0f);
	tl2->create(2.f);
	_game_objects->push_back(tl2);
}

void
GameManager::createTurtle()
{
	Turtle * tl = new Turtle(_mySurf, _shader, _ml);
	tl->create();
	_game_objects->push_back(tl);
}
//////////////////////////////////////////////////////////////////////// GAME INIT LOOP
void
GameManager::init()
{

	_shader = new VSShaderLib();
	_mySurf = new VSResSurfRevLib();
	_ml = new MathLib();

	_game_objects = new std::vector<GameObject*>();

	if (!setupShaders())
		exit(1);

	viewMatrixId = glGetUniformLocation(_shader->getProgramIndex(), "viewMatrix");
	projId = glGetUniformLocation(_shader->getProgramIndex(), "projMatrix");
	modelId = glGetUniformLocation(_shader->getProgramIndex(), "model");
	colorId = glGetUniformLocation(_shader->getProgramIndex(), "in_color");

	createScene();
	createCameras();

	
	// final main loop
	glutMainLoop();
	exit(EXIT_SUCCESS);
}

