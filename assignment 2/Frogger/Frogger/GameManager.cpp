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
	
	int deltaX = -xx + startX;
	int deltaY = yy - startY;

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
	/*else if (tracking == 2) {
		alphaAux = alpha;
		betaAux = beta;
		rAux = r + (deltaY * 0.01f);
		if (rAux < 0.1f)
			rAux = 0.1f;
	}*/

	_tpCam->updateEye(rAux, alphaAux, betaAux, _frog->getPosition());

}
void
GameManager::mouseButtons(int button, int state, int xx, int yy)
{
	// start tracking the mouse
	if (state == GLUT_DOWN)  {
		startX = xx;
		startY = yy;
		if (button == GLUT_LEFT_BUTTON)
			tracking = 1;
	}
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

	//stop tracking the mouse
	else if (state == GLUT_UP) {
		if (tracking == 1) {
			alpha -= xx - startX;
			beta += yy - startY;
		}
		tracking = 0;
	}
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
	Vector3 *dir = new Vector3(0.0f, 0.0f, 0.0f);
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
		frogLeft = true;
		dir->set(-1.0f, 0.0f, 0.0f);
		break;

	case 'p':
		frogRight = true;
		dir->set(1.0f, 0.0f, 0.0f);
		break;

	case 'q':
		frogUp = true;
		dir->set(0.0f, 0.0f, -1.0f);
		break;

	case 'a':
		frogDown = true;
		dir->set(1.0f, 0.0f, 1.0f);
		break;

	case 'n':
		onGlobal = !onGlobal;
		_light_sources->at(0)->setState(onGlobal);
		break;

	case 'c':
		onLamps = !onLamps;
		for ( int i = 1; i < 7; i++)
		{
			_light_sources->at(i)->setState(onLamps);
		}
		break;

	case 27:
		glutLeaveMainLoop();
		break;

	}
	_frog->setSpeed(_frog->getSpeed().operator+(dir));
}

void
GameManager::keyUp(unsigned char key, int xx, int yy) {
	switch (key) {
	case 'o':
		frogLeft = false;
		break;

	case 'p':
		frogRight = false;
		break;

	case 'q':
		frogUp = false;
		break;

	case 'a':
		frogDown = false;
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
		_tpCam->updateAt(_frog->getPosition());
		_tpCam->computeVisualizationMatrix();
		break;
	}
		
	glUseProgram(_shader->getProgramIndex());
	
	
	updateLights();

	glUniformMatrix4fv(viewMatrixId, 1, false, _ml->getViewMatrix());
	glUniformMatrix4fv(projId, 1, false, _ml->getProjMatrix());

	incrementSpeed();
	updateDynamicObj();

	std::vector<GameObject*>::iterator it_obj;
	for (it_obj = _game_objects->begin(); it_obj != _game_objects->end(); it_obj++)
	{
		(*it_obj)->draw();
	}


	_gl_errors.checkOpenGLError("ERROR: Could not draw scene.");

	glutSwapBuffers();
}


////////////////////////////////////////////////////////////////////// Object Movement 

void
GameManager::updateDynamicObj(){
	updateLogs();
	updateBus();
	updateCars();
	updateTurtles();
	updateFrog();
}

void
GameManager::updateFrog()
{
	if (frogUp)
	{
		_frog->setPosition(
			_frog->getPosition()->getX(),
			_frog->getPosition()->getY(),
			_frog->getPosition()->getZ() - _frog_speed);
		_tpCam->updateEye(rAux, alphaAux, betaAux, _frog->getPosition());
	}
	if (frogDown)
	{
		_frog->setPosition(
			_frog->getPosition()->getX(),
			_frog->getPosition()->getY(),
			_frog->getPosition()->getZ() + _frog_speed);
		_tpCam->updateEye(rAux, alphaAux, betaAux, _frog->getPosition());
	}
	if (frogLeft)
	{
		_frog->setPosition(
			_frog->getPosition()->getX() - _frog_speed,
			_frog->getPosition()->getY(),
			_frog->getPosition()->getZ());
		_tpCam->updateEye(rAux, alphaAux, betaAux, _frog->getPosition());
	}
	if (frogRight)
	{
		_frog->setPosition(
			_frog->getPosition()->getX() + _frog_speed,
			_frog->getPosition()->getY(),
			_frog->getPosition()->getZ());
		_tpCam->updateEye(rAux, alphaAux, betaAux, _frog->getPosition());
	}
}

void 
GameManager::updateLogs()
{
	std::vector<TimberLog*>::iterator it_obj;
	for (it_obj = _logs->begin(); it_obj != _logs->end(); it_obj++)
	{	
		(*it_obj)->update(speedIncr);
	}
}

void
GameManager::updateBus()
{
	std::vector<Bus*>::iterator it_obj;
	for (it_obj = _bus->begin(); it_obj != _bus->end(); it_obj++)
	{
		(*it_obj)->update(0.5f *speedIncr);
	}
}

void
GameManager::updateCars()
{
	std::vector<Car*>::iterator it_obj;
	for (it_obj = _cars->begin(); it_obj != _cars->end(); it_obj++)
	{
		(*it_obj)->update(0.75f *speedIncr);
	}
}

void
GameManager::updateTurtles()
{
	std::vector<Turtle*>::iterator it_obj;
	for (it_obj = _turtles->begin(); it_obj != _turtles->end(); it_obj++)
	{
		(*it_obj)->update(0.2f *speedIncr);
	}
}

void
GameManager::incrementSpeed(){

	int time = glutGet(GLUT_ELAPSED_TIME) / 1000;

	if (time % 10 == 0)
		speedIncr += 0.00001;

}

/////////////////////////////////////////////////////////////////////// Lights and Camera

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

	_perspCam = new PerspectiveCamera(53.13f, ratio, 20.0f, -20.0f, _ml);
	_perspCam->setPosition(0.0f, 45.0f, 0.0f);
	_perspCam->update();

	_tpCam = new PerspectiveCamera(53.13f, ratio, 0.1f, -5.0f, _ml);
	_tpCam->updateEye(r, alpha, beta, _frog->getPosition());
	_tpCam->updateUp();
	_tpCam->updateAt(_frog->getPosition());
	


}

void
GameManager::createLightsources()
{
	LightSource * l = new LightSource((GLenum)0);
	Vector4 *params = new Vector4(1.f, 1.f, 1.f, 1.f);
	l->setAmbient(params);
	l->setDiffuse(params);
	l->setSpecular(params);
	l->setExponent(100.f);
	l->setPosition(new Vector4(0.0f, 4.0f, 0.0f, 0.f));
	_light_sources->push_back(l);


	params = new Vector4(0.f, 0.f, 0.f, 1.f);
	int id = 1;
	LightSource* lamp;
	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 2; j++){
			lamp = new LightSource((GLenum)id);
			lamp->setAmbient(params);
			lamp->setDiffuse(params);
			lamp->setSpecular(params);
			lamp->setExponent(100.f);
			lamp->setCutOff(90.f);
			lamp->setPosition(new Vector4(-10.0f + 20.0f*j, 4.0f, 19.0f - 17.75f*i, 1.f));
			_light_sources->push_back(lamp);

			id++;
		}
	}
}

void
GameManager::updateLights(){
	GLint loc;

	LightSource *l = _light_sources->at(0);
	float res[4];
	float *lpos = l->getPosition()->Vec4ToFloat();
	_ml->MultiplyMatrixByVector4by4OpenGL_FLOAT(res, _ml->getViewMatrix(), lpos);

	loc = glGetUniformLocation(_shader->getProgramIndex(), "l_pos");
	glUniform4fv(loc, 1, res);

	float state;

	if (onGlobal)
		state = 1.0f;
	else
		state = 0.0f;

	glUniform1f(globalId, state);

	if (onLamps)
		state = 1.0f;
	else
		state = 0.0f;


	glUniform1f(lampId, state);


	for (int i = 1; i < 7; i++){
		l = _light_sources->at(i);
		lpos = l->getPosition()->Vec4ToFloat();
		_ml->MultiplyMatrixByVector4by4OpenGL_FLOAT(res, _ml->getViewMatrix(), lpos);

		glUniform4fv(pointsIds[i-1], 1, res);
	}

}

/////////////////////////////////////////////////////////////////////// SHADERS

GLuint
GameManager::setupShaders()
{
	_shader->init();
	//_shader->loadShader(VSShaderLib::VERTEX_SHADER, "assign2.vert");
	//_shader->loadShader(VSShaderLib::FRAGMENT_SHADER, "assign2.frag");

	_shader->loadShader(VSShaderLib::VERTEX_SHADER, "lights.vert");
	_shader->loadShader(VSShaderLib::FRAGMENT_SHADER, "lights.frag");

	_shader->setProgramOutput(0, "outFrag");
	_shader->setVertexAttribName(VSShaderLib::VERTEX_COORD_ATTRIB, "in_pos");
	_shader->setVertexAttribName(VSShaderLib::NORMAL_ATTRIB, "normal");
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
	createTopTimberLog();
	createMiddleTimberLog();
	createBottomTimberLog();
	createTopTurtles();
	createBottomTurtles();
}

void
GameManager::createLamps(){
	for ( int i = 1; i < 7; i++)
	{
		LightSource* l = _light_sources->at(i);
		
		Lamp* lamp = new Lamp(_mySurf, _shader, _ml, l);
		lamp->create();
		_game_objects->push_back(lamp);
	}
}

void
GameManager::createRiver()
{
	River * river = new River(_mySurf, _shader, _ml);
	river->setPosition(0.0f, .5f, -7.5f);
	river->create();
	_game_objects->push_back(river);
}

void
GameManager::createFrog()
{
	_frog = new Frog(_mySurf, _shader, _ml);
	_frog->setPosition(0.0f, 2.0f, 19.0f);
	_frog->create();
	_game_objects->push_back(_frog);
}

void
GameManager::createRoad()
{
	Road * road = new Road(_mySurf, _shader, _ml);
	road->setPosition(0.0f, .5f, 9.5f);
	road->create();
	_game_objects->push_back(road);
}

void
GameManager::createRoadside()
{
	Roadside * roadside = new Roadside(_mySurf, _shader, _ml);
	roadside->setPosition(0.0f, 1.f, 19.f);
	roadside->create();
	_game_objects->push_back(roadside);
}

void
GameManager::createRiverside()
{
	Riverside * riverside = new Riverside(_mySurf, _shader, _ml);
	riverside->setPosition(0.0f, 1.0f, 1.0f);
	riverside->create();
	_game_objects->push_back(riverside);

	Riverside * riverside2 = new Riverside(_mySurf, _shader, _ml);
	riverside2->setPosition(0.0f, 1.0f, -16.0f);
	_game_objects->push_back(riverside2);
}

void
GameManager::createBus()
{
	Bus * bus;
	for (int i = 0; i < 2; i++){
		for (int j = 0; j < 3; j++){
			bus = new Bus(_mySurf, _shader, _ml);
			bus->setPosition(-10.0f + 7.0f*j, 2.0f, 4.5f + 10.0f*i);
			bus->create();
			_game_objects->push_back(bus);
			_bus->push_back(bus);
		}
	}
}

void
GameManager::createCar()
{
	Car * car;
	for (int i = 0; i < 2; i++){
		for (int j = 0; j < 4; j++){
			car = new Car(_mySurf, _shader, _ml);
			car->setPosition(-11.0f + 6.0f*j - 2*i, 2.0f, 9.5f + 2.0f*i);
			car->create();
			_game_objects->push_back(car);
			_cars->push_back(car);
		}
	}
	
}

void
GameManager::createTopTimberLog()
{
	TimberLog * tl;
	for (int i = 0; i < 3; i++){
		tl = new TimberLog(_mySurf, _shader, _ml);
		tl->setPosition(-9.0f + 10.0f * i, 1.0f, -13.0f);
		tl->create();
		_game_objects->push_back(tl);
		_logs->push_back(tl);
	}
}

void
GameManager::createMiddleTimberLog()
{
	TimberLog * tl;
	for (int i = 0; i < 3; i++){
		tl = new TimberLog(_mySurf, _shader, _ml);
		tl->setPosition(-11.0f + 9.0f * i, 1.0f, -7.5f);
		tl->create();
		_game_objects->push_back(tl);
		_logs->push_back(tl);
	}

}

void
GameManager::createBottomTimberLog()
{
	TimberLog * tl;
	for (int i = 0; i < 3; i++){
		tl = new TimberLog(_mySurf, _shader, _ml);
		tl->setPosition(-8.0f + 8.0f * i, 1.0f, -5.0f);
		tl->create();
		_game_objects->push_back(tl);
		_logs->push_back(tl);
	}
}

void
GameManager::createTopTurtles()
{
	Turtle * tl;
	for (int i = 0; i < 2; i++){
		for (int j = 0; j < 2; j++){
			tl = new Turtle(_mySurf, _shader, _ml);
			tl->setPosition(-10.0f + 3.4f*j + 10.4f*i, 1.0f, -10.0f);
			tl->create();
			_game_objects->push_back(tl);
			_turtles->push_back(tl);
		}
	}
}

void
GameManager::createBottomTurtles()
{
	Turtle * tl;
	for (int i = 0; i < 2; i++){
		for (int j = 0; j < 3; j++){
			tl = new Turtle(_mySurf, _shader, _ml);
			tl->setPosition(-10.0f + 3.4f*j + 12.1f*i, 1.0f, -2.0f);
			tl->create();
			_game_objects->push_back(tl);
			_turtles->push_back(tl);
		}
	}
}

//////////////////////////////////////////////////////////////////////// GAME INIT LOOP
void
GameManager::init()
{
	_shader = new VSShaderLib();
	_mySurf = new VSResSurfRevLib();
	_ml = new MathLib();

	_game_objects = new std::vector<GameObject*>();
	_light_sources = new std::vector<LightSource*>();
	_logs = new std::vector<TimberLog*>();
	_bus = new std::vector<Bus*>();
	_cars = new std::vector<Car*>();
	_turtles = new std::vector<Turtle*>();

	if (!setupShaders())
		exit(1);

	viewMatrixId = glGetUniformLocation(_shader->getProgramIndex(), "viewMatrix");
	projId = glGetUniformLocation(_shader->getProgramIndex(), "projMatrix");
	modelId = glGetUniformLocation(_shader->getProgramIndex(), "model");
	normal_uniformId = glGetUniformLocation(_shader->getProgramIndex(), "m_normal");
	globalId = glGetUniformLocation(_shader->getProgramIndex(), "stateGbl");
	lampId = glGetUniformLocation(_shader->getProgramIndex(), "stateL");

	pointsIds[0] = glGetUniformLocation(_shader->getProgramIndex(), "lamp1");
	pointsIds[1] = glGetUniformLocation(_shader->getProgramIndex(), "lamp2");
	pointsIds[2] = glGetUniformLocation(_shader->getProgramIndex(), "lamp3");
	pointsIds[3] = glGetUniformLocation(_shader->getProgramIndex(), "lamp4");
	pointsIds[4] = glGetUniformLocation(_shader->getProgramIndex(), "lamp5");
	pointsIds[5] = glGetUniformLocation(_shader->getProgramIndex(), "lamp6");

	//lightId = glGetUniformLocation(_shader->getProgramIndex(), "l_pos");

	createScene();
	createCameras();
	createLightsources();
	createLamps();

	// final main loop
	glutMainLoop();
	exit(EXIT_SUCCESS);
}

