#include "GameManager.h"

enum LIGHT_TYPE active_light = SPOT_LIGHT;

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
		//on = !on;
		//l->setState(on);
		active_light = active_light == SPOT_LIGHT
			? POINT_LIGHT
			: SPOT_LIGHT;
		break;

	case 27:
		glutLeaveMainLoop();
		break;

	case 'c':
		/*printf("Camera Spherical Coordinates (%f, %f, %f)\n"
			, _scene->getCam()->getA(), _scene->getCam()->getB(), _scene->getCam()->getR());*/
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
	GLint loc;
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

	LightSource *l;
	if (active_light == SPOT_LIGHT) {
		l = _light_sources->at(0);
		updateLightPos(l);
		if (frogUp)
			l->setDirection(new Vector4(0.f, .5f, -.75f, 0.f));
		if (frogDown)
			l->setDirection(new Vector4(0.f, -.5f, -.75f, 0.f));
		if (frogLeft)
			l->setDirection(new Vector4(-.5f, .0f, -.75f, 0.f));
		if (frogRight)
			l->setDirection(new Vector4(.5f, .0f, -.75f, 0.f));
	}
	else {
		l = _light_sources->at(1);
	}

	float res[4];
	float *lpos = l->getPosition()->Vec4ToFloat();
	_ml->MultiplyMatrixByVector4by4OpenGL_FLOAT(res, _ml->getViewMatrix(), lpos);
	glUniform4fv(lightId, 1, res);

	float* amb = l->getAmbient().Vec4ToFloat();
	loc = glGetUniformLocation(_shader->getProgramIndex(), "light.ambient");
	glUniform4fv(loc, 1, amb);

	float* diff = l->getDiffuse().Vec4ToFloat();
	loc = glGetUniformLocation(_shader->getProgramIndex(), "light.diffuse");
	glUniform4fv(loc, 1, diff);

	float* spec = l->getSpecular().Vec4ToFloat();
	loc = glGetUniformLocation(_shader->getProgramIndex(), "light.specular");
	glUniform4fv(loc, 1, spec);

	float shininess = l->getShininess();
	loc = glGetUniformLocation(_shader->getProgramIndex(), "light.shininess");
	glUniform1f(loc, shininess);

	/* BEGIN Spotlight */

	float* spot_dir = l->getDirection().Vec4ToFloat();
	loc = glGetUniformLocation(_shader->getProgramIndex(), "light.spotDir");
	glUniform4fv(loc, 1, spot_dir);

	float cutoff = l->getCutOff();
	loc = glGetUniformLocation(_shader->getProgramIndex(), "light.cutoff");
	glUniform1f(loc, cutoff);

	/* END Spotlight */

	glUniformMatrix4fv(viewMatrixId, 1, false, _ml->getViewMatrix());
	glUniformMatrix4fv(projId, 1, false, _ml->getProjMatrix());

	incrementSpeed();
	updateDynamicObj();

	std::vector<GameObject*>::iterator it_obj;
	for (it_obj = _game_objects->begin(); it_obj != _game_objects->end(); it_obj++)
	{
		(*it_obj)->draw();
	}


	//_gl_errors.checkOpenGLError("ERROR: Could not draw scene.");
}

void
GameManager::updateLightPos(LightSource* l) {
	l->setPosition(new Vector4(
		_frog->getPosition()->getX() +  .5f, // Add half of frog's body width
		_frog->getPosition()->getY() + 1.0f,
		_frog->getPosition()->getZ() + -.125f,
		1.f));
}

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
		l = new LightSource(SPOT_LIGHT);
		l->setAmbient(new Vector4(.0f, .0f, .0f, 1.f));
		l->setDiffuse(new Vector4(.8f, .8f, .8f, 1.f));
		l->setSpecular(new Vector4(1.f, 1.f, 1.f, 1.f));

		updateLightPos(l);
		l->setDirection(new Vector4(0.f, .5f, -.75f, 0.f));

		l->setExponent(100.f);
		l->setCutOff(.5f);

		_light_sources->push_back(l);

		l = new LightSource(POINT_LIGHT);
		l->setAmbient(new Vector4(.8f, .8f, .8f, 1.f));
		l->setDiffuse(new Vector4(.8f, .8f, .8f, 1.f));
		l->setSpecular(new Vector4(1.f, 1.f, 1.f, 1.f));
		l->setPosition(new Vector4(0.f, 40.f, 0.f, 1.f));
		l->setExponent(100.f);

		_light_sources->push_back(l);
}

/////////////////////////////////////////////////////////////////////// SHADERS

GLuint
GameManager::setupShaders()
{
	return setupShader(_shader_point, "assign2.vert", "assign2.frag")
		&& setupShader(_shader_spot, "spotlight.vert", "spotlight.frag");
}

GLuint
GameManager::setupShader(VSShaderLib *shader, char *vert_filename, char *frag_filename)
{
	shader->init();

	shader->loadShader(VSShaderLib::VERTEX_SHADER, vert_filename);
	shader->loadShader(VSShaderLib::FRAGMENT_SHADER, frag_filename);

	shader->setProgramOutput(0, "outFrag");
	shader->setVertexAttribName(VSShaderLib::VERTEX_COORD_ATTRIB, "in_pos");
	shader->setVertexAttribName(VSShaderLib::NORMAL_ATTRIB, "normal");
	shader->prepareProgram();

	viewMatrixId     = glGetUniformLocation(shader->getProgramIndex(), "viewMatrix");
	projId           = glGetUniformLocation(shader->getProgramIndex(), "projMatrix");
	modelId          = glGetUniformLocation(shader->getProgramIndex(), "model");
	normal_uniformId = glGetUniformLocation(shader->getProgramIndex(), "m_normal");
	lightId          = glGetUniformLocation(shader->getProgramIndex(), "l_pos");

	return(shader->isProgramValid());
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
	_frog->setPosition(0.0f, 2.0f, 19.0f);
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
	Bus * bus;
	for (int i = 0; i < 2; i++){
		for (int j = 0; j < 3; j++){
			bus = new Bus(_mySurf, _shader, _ml);
			bus->setPosition(-10.0f + 7.0f*j, 2.0f, 5.0f + 10.0f*i);
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
		tl->setPosition(-9.0f + 10.0f * i, 2.0f, -13.0f);
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
		tl->setPosition(-11.0f + 9.0f * i, 2.0f, -7.0f);
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
		tl->setPosition(-8.0f + 8.0f * i, 2.0f, -5.0f);
		tl->create();
		_game_objects->push_back(tl);
		_logs->push_back(tl);
	}
}

void
GameManager::createTopTurtles()
{
	Turtle * tl;
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 2; j++){
			tl = new Turtle(_mySurf, _shader, _ml);
			tl->setPosition(-8.0f + 1.3f*j + 6.0f*i, 2.0f, -10.0f);
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
	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++){
			tl = new Turtle(_mySurf, _shader, _ml);
			tl->setPosition(-10.0f + 1.3f*j + 10.0f*i, 2.0f, -2.0f);
			tl->create();
			_game_objects->push_back(tl);
			_turtles->push_back(tl);
		}
	}
}

void
GameManager::incrementSpeed(){

	int time = glutGet(GLUT_ELAPSED_TIME)/1000;

	if (time % 10 == 0)
		speedIncr += 0.000001;

}

//////////////////////////////////////////////////////////////////////// GAME INIT LOOP
void
GameManager::init()
{
	_shader_point = new VSShaderLib();
	_shader_spot = new VSShaderLib();

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

	// Default shader
	_shader = _shader_spot;

	createScene();
	createCameras();
	createLightsources();
	
	// final main loop
	glutMainLoop();
	exit(EXIT_SUCCESS);
}