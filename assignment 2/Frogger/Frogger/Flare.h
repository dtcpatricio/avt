#pragma once
#include "StaticObject.h"

#define FLARE_MAXELEMENTSPERFLARE         15

#define FLARE_RANGE(A,B)    ( (rand()%((B)-(A)+1)) + (A) )
#define FLARE_FRANGE(A,B)   ( ((float)(rand()&0xffffff)/(float)0xfffffe)*((B)-(A)) + (A) )

typedef struct FLARE_ELEMENT_DEF
{
	struct TEXTURE_DEF  *texture;

	float           fDistance;        // Distance along ray from source (0.0-1.0)
	float           fSize;            // Size relative to flare envelope (0.0-1.0)
	unsigned int    argb;            // ARGB for intensity mapping
}
FLARE_ELEMENT_DEF;

class Flare :
	public StaticObject
{
public:
	Flare(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* calc, 
		int lx, int ly, int cx, int cy);
	~Flare();

	void randomize(int nTextures, int nPieces, float fMaxSize, unsigned int minColour, unsigned int maxColour);
	void create();
	void draw();
	void setLX(int lx){ _lx = lx;}
	void setCX(int cx){ _cx = cx; }

private:
	int id = 17;
	FLARE_ELEMENT_DEF    _element[FLARE_MAXELEMENTSPERFLARE];
	int _nTextures;
	int _nPieces;
	float _fMaxSize;
	unsigned int _minColour;
	unsigned int _maxColour;
	int _lx; 
	int _ly;
	int _cx; 
	int _cy;
	int aux = 0;
};

