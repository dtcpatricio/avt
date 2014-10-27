#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <math.h>
#include <vector>

#include <GL/glew.h>
#include <GL/freeglut.h>

#define ILUT_USE_OPENGL
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

#include "GameManager.h"
#include "vsShaderLib.h"
#include "GLErrors.h"

#define WinX 640
#define WinY 480

const GLfloat FPS = 1000 / 60;
