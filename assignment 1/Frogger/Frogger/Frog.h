#include "Scene.h"

#ifndef __FROG_H__
#define __FROG_H__

class Scene;

class Frog {

public:
	Frog(Scene *scene);
	~Frog();

	float getX();
	float getY();
	float getZ();

	void setX(float x);
	void setY(float y);
	void setZ(float z);

	void setLeft (bool b);
	void setRight(bool b);
	void setUp   (bool b);
	void setDown (bool b);

	void draw();

private:
	Scene *_scene;

	float xPos, yPos, zPos;
	float speed = 0.05 * 1 / 60;
	bool leftPressed, rightPressed, upPressed, downPressed;
	
	void goLeft();
	void goRight();
	void goForward();
	void goBackwards();
};

#endif