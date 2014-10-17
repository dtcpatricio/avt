
#include "Scene.h"

#ifndef __FROGGERIO_H__
#define __FROGGERIO_H__

#include <GL/glew.h>
#include <GL/freeglut.h>


class Scene;

class IO {
public:
	IO(Scene *scene);

	GLfloat getMouseWorldX() { return mouseWorldP[0]; }
	GLfloat getMouseWorldY() { return mouseWorldP[1]; }
	GLfloat getMouseWorldZ() { return mouseWorldP[2]; }

	void setMouseWorldX(GLfloat x) { mouseWorldP[0] = x; }
	void setMouseWorldY(GLfloat y) { mouseWorldP[1] = y; }
	void setMouseWorldZ(GLfloat z) { mouseWorldP[2] = z; }

	// Used as GLUT callbacks
	void mouseWheel(int wheel, int direction, int x, int y);
	void processMouseMotion(int xx, int yy);
	void processMouseButtons(int button, int state, int xx, int yy);
	void processKeys(unsigned char key, int xx, int yy);
	void processUpKeys(unsigned char key, int x, int y);

	void reshape(int w, int h);

private:
	Scene *_scene;

	// World Coordinates
	GLfloat mouseWorldP[3];

	// Tracking Variables
	int startX, startY, tracking = 0;
};

#endif