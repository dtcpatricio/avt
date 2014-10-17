#include "Frog.h"
#include "Scene.h"

Frog::Frog(Scene *scene) : _scene(scene) {}
Frog::~Frog() {}

float Frog::getX() { return xPos; }
float Frog::getY() { return yPos; }
float Frog::getZ() { return zPos; }

void Frog::setX(float x) { xPos = x; }
void Frog::setY(float y) { yPos = y; }
void Frog::setZ(float z) { zPos = z; }

void Frog::setLeft (bool b) { leftPressed  = b; }
void Frog::setRight(bool b) { rightPressed = b; }
void Frog::setUp   (bool b) { upPressed    = b; }
void Frog::setDown (bool b) { downPressed  = b; }

void Frog::draw() {
	if (leftPressed  == true) goLeft();
	if (rightPressed == true) goRight();
	if (upPressed    == true) goForward();
	if (downPressed  == true) goBackwards();
}

void Frog::goLeft(){
	xPos -= speed;
	zPos -= speed;
}

void Frog::goRight(){
	xPos += speed;
	zPos += speed;
}

void Frog::goForward(){
	xPos += speed;
	zPos -= speed;
}

void Frog::goBackwards(){
	xPos -= speed;
	zPos += speed;
}