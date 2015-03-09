#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include "SDL.h" 
#include "SDL_opengl.h"
#include <GL/gl.h>
#include <GL/glu.h>

#include "../../engine/entity/camera.h"
#include "../engine.h"

#ifndef PI
#	define	PI		(3.141592f)
#endif
#define HALF_PI	(1.570796f)
#ifndef RADS
#	define RADS	(PI/180.0f)
#endif
#define DEGREES		(180.0f/PI)
#ifndef ROOT_TWO
#	define ROOT_TWO	(1.414214f)
#endif

#define VIEW_DISTANCE			55.0f
#define VIEW_DISTANCE_SQUARED	3025.0f

// Graphics Options:
//#define NO_FOG
//#define SHOW_FPS
//#define MAX_LIGHTS	2
//#define WIREFRAME_RENDERING
//#define LOW_QUALITY_GEOMETRY
//#define LOW_RESOLUTION_TEXTURES
//#define SCR_WIDTH 	1024
//#define SCR_HEIGHT 	768
//#define SCR_DEPTH		16
//#define FULLSCREEN

#ifndef _DEBUG
#	define FULLSCREEN
#else
#	define SHOW_FPS
#endif

#define HIGH_QUALITY

#ifdef HIGH_QUALITY
#	define MAX_LIGHTS	8
#	define SCR_DEPTH	32
#	define SCR_WIDTH	1024
#	define SCR_HEIGHT	768
#endif



#ifndef FULLSCREEN
	#define SCR_FLAGS 	(SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_HWPALETTE | SDL_HWSURFACE | SDL_HWACCEL ) // SDL_HWPALETTE SDL_HWSURFACE SDL_HWACCEL SDL_FULLSCREEN
#else
	#define SCR_FLAGS 	(SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_HWPALETTE | SDL_HWSURFACE | SDL_HWACCEL | SDL_FULLSCREEN)
#endif


int 	GraphicsStartup();
int 	GraphicsShutdown();
int 	GameRender();
void	BindCamera( CCamera* c );

#endif
