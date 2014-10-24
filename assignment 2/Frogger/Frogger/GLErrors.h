#pragma once

#include <GL/glew.h>
#include <string>
#include <iostream>

class GLErrors
{
public:
	GLErrors();
	~GLErrors();

	void checkOpenGLError(std::string error);
	
private:
	bool isOpenGLError();
};

