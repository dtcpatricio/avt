#include "Flare.h"

#define WinX 640
#define WinY 480

#define HEIGHTFROMWIDTH(w)  ((320*(w)*WinY)/(240*WinX))

#define isqrt(x)        (int)((double)(x))

Flare::Flare(VSResSurfRevLib * mySurf, VSShaderLib * shader, MathLib* calc, int lx, int ly,
		int cx, int cy)
		: StaticObject(mySurf, shader, calc), _lx(lx), _ly(ly), _cx(cx), _cy(cy) 
{
	amb[0] = 0.0f; amb[1] = 0.0f; amb[2] = 1.f; amb[3] = 1.f;
	diff[0] = .1f; diff[1] = 0.1f; diff[2] = .9f; diff[3] = .4f;
	spec[0] = 0.04f; spec[1] = 0.04f; spec[2] = .9f; spec[3] = 1.f;
	emissive[0] = 0.f;  emissive[1] = 0.f; emissive[2] = 0.f; emissive[3] = 1.f;
	shininess = new float(12.f * 128.f);
}


Flare::~Flare()
{
}

void
Flare::randomize(int nTextures, int nPieces, float fMaxSize, unsigned int minColour, unsigned int maxColour)
{
	int             i;
	float           fFracDist;
	float           fEnvelopeSize;
	FLARE_ELEMENT_DEF *element;

	// Initialize flare with requested number of elements, with
	// random placement, colour, and size of elements.
	_nPieces = nPieces;
	_fMaxSize = fMaxSize;
	_minColour = minColour;
	_maxColour = maxColour;
	fFracDist = 1.0f / (float)(_nPieces - 1);

	for (i = 0; i < _nPieces; ++i)
	{
		element = &_element[i];
		element->fDistance = (fFracDist*i) + FLARE_FRANGE(0, fFracDist);

		// Envelope size is maximum at ends of line, minimum in the middle (i.e. two
		// cones, touching at the tips).
		fEnvelopeSize = (float)fabs(1.0f - 2 * element->fDistance);

		element->fSize = FLARE_FRANGE(0.6f, 1.0f) * fEnvelopeSize;
		element->argb = FLARE_RANGE(minColour & 0xff000000, maxColour & 0xff000000) |
			FLARE_RANGE(minColour & 0x00ff0000, maxColour & 0x00ff0000) |
			FLARE_RANGE(minColour & 0x0000ff00, maxColour & 0x0000ff00) |
			FLARE_RANGE(minColour & 0x000000ff, maxColour & 0x000000ff);
	}
}

void
Flare::create(){

	int     dx, dy;          // Screen coordinates of "destination"
	int     px, py;          // Screen coordinates of flare element
	int     maxflaredist, flaredist, flaremaxsize, flarescale;
	int     width, height, alpha;    // Piece parameters;
	int     i;
	FLARE_ELEMENT_DEF    *element;
	int idAux = id;

	// Compute how far off-center the flare source is.
	maxflaredist = isqrt(_cx*_cx + _cy*_cy);
	flaredist = isqrt((_lx - _cx)*(_lx - _cx) +
		(_ly - _cy)*(_ly - _cy));
	flaredist = abs(maxflaredist - flaredist);
	flaremaxsize = (int)(WinX * _fMaxSize);
	flarescale = (int)(WinX * _fMaxSize);

	// Destination is opposite side of centre from source
	dx = _cx + (_cx - _lx);
	dy = _cy + (_cy - _ly);

	// Render each element.
	for (i = 0; i < _nPieces; ++i)
	{
		element = &_element[i];

		// Position is interpolated along line between start and destination.
		px = (int)((1.0f - element->fDistance)*_lx + element->fDistance*dx);
		py = (int)((1.0f - element->fDistance)*_ly + element->fDistance*dy);

		// Piece size are 0 to 1; flare size is proportion of
		// screen width; scale by flaredist/maxflaredist.
		width = (int)((flaredist*flarescale*element->fSize) / maxflaredist);
		
		// Width gets clamped, to allows the off-axis flares
		// to keep a good size without letting the elements get
		// too big when centered.
		if (width > flaremaxsize)
		{
			width = flaremaxsize;
		}

		// Flare elements are square (round) so height is just
		// width scaled by aspect ratio.
		height = ((320 * (width)*WinX) / (240 * WinY));
		alpha = (flaredist*(element->argb >> 24)) / maxflaredist;

		if (width > 1)
		{
			unsigned int    argb = (alpha << 24) | (element->argb & 0x00ffffff);
			_mySurf->setObjId(idAux);
			_mySurf->createRectangle(px - width / 2, py - height / 2, width, height);
			applyColor();
			idAux++;
		}
		else{ aux++; }
	}
}

void
Flare::draw()
{
	int idAux = id;
	for (int i = 0; i < _nPieces - aux; i++)
	{
		_mySurf->setObjMaterials(idAux, _shader);
		_calc->translation(_position.getX(), _position.getY() + 10.0f, _position.getZ());
		sendToGL(idAux);
		idAux++;
	}
	
}
