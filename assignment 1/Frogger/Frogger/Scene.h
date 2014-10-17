#ifndef __SCENE_H__
#define __SCENE_H__

#include "misc.h"

#include "vsResSurfRevLib.h"

class IO;
class Cam;
class Frog;
class OurMathLib;

class Scene {
	IO          *_io;
	Cam         *_cam;
	Frog        *_frog;
	OurMathLib  *_calc;
	VSShaderLib &_shader;
	VSResSurfRevLib _mySurf;

	GLuint viewMatrixId, projId, modelId, colorId;

	void createRiver();
	void createRoad();
	void createTopLogs();
	void middleRowLogs();
	void bottomRowLogs();
	void createLogs();
	void createGrassEdge();
	void createStreetEdge();
	void createSidewalkEdge();
	void createEdges();
	void createFrog(Frog frog);
	void createCar();

	void applySurfaceTransformations();
	void applyTransformations(int objId);
	void sendToGL(int objId);
	void applyColor(float r, float g, float b);

public:
	Scene(VSShaderLib &shader);
	
	IO              *getIO();
	Cam             *getCam();
	Frog            *getFrog();
	OurMathLib      *getCalc();
	VSShaderLib     getShader();
	VSResSurfRevLib getSurf();

	void create();
	void initIDs();

	void update();
};

#endif