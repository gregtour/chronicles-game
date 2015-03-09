// Graphics.cpp

#include "../engine.h"
#include "graphics.h"
#include "../common/list.h"
#include "../entity/camera.h"
#include "../entity/graphicalObj.h"

SDL_Surface* gScreen;
CCamera* gCamera = NULL;

void BindCamera( CCamera* c )
{
	gCamera = c;
}

int GraphicsStartup()
{		
	SDL_ShowCursor( 0 );

	gScreen = SDL_SetVideoMode( SCR_WIDTH, SCR_HEIGHT, SCR_DEPTH, SCR_FLAGS );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	SDL_WM_SetCaption( gameTitle, gameTitle );
	
//	int hasARBImaging = isExtensionSupported("GL_ARB_imaging");
/*
#ifdef _WIN32
  if (hasPointParams) {
    glPointParameterfEXT = (PFNGLPOINTPARAMETERFEXTPROC)
    wglGetProcAddress("glPointParameterfEXT");
    glPointParameterfvEXT = (PFNGLPOINTPARAMETERFVEXTPROC)
    wglGetProcAddress("glPointParameterfvEXT");
  }
#endif
*/

	glShadeModel( GL_SMOOTH );

	glFrontFace( GL_CCW );

	glEnable( GL_CULL_FACE );

	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	glClearDepth( 1.0f );

	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );

#ifdef WIREFRAME_RENDERING
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

	glViewport( 0, 0, SCR_WIDTH, SCR_HEIGHT );

	glMatrixMode( GL_PROJECTION );
	gluPerspective( 55.0f , SCR_WIDTH/(GLfloat)SCR_HEIGHT , 1.0f, VIEW_DISTANCE );

	glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

	return 1;
}

int GraphicsShutdown()
{
	return 1;
}


int GameRender()
{

/* Render Order:
Render()		-Solid Graphics		(Level, Characters, Etc)
RenderAlpha()	-Blended Graphics	(Shadows, Particles, Effects)
RenderOrtho()	-Overlays			(HUD, Menu)
*/


	/*		Reset for new Rendering Pass	*/
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 55.0f , SCR_WIDTH/(GLfloat)SCR_HEIGHT , 1.0f , VIEW_DISTANCE );

	glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


	/*	Set to Camera View	*/
	if ( gCamera )
	{
		gCamera->Render();
	}

	/*		Render Normal		*/
	CListEntry<CEntity>* entity = gEntities.GetFirst();
	while ( entity )
	{
		// Render it if its an IGraphicalObj* w/ Render() function
		IGraphicalObj* o = dynamic_cast<IGraphicalObj*>(entity->data);
		if ( o != NULL )
		{
			glPushMatrix();	// Save Modelview Matrix @ Camera Position
			o->Render();
			glPopMatrix();	// Restore it
		}
		
		entity = entity->GetNext();
	}

	/*		Render Alpha		*/
	entity = gEntities.GetLast();
	while ( entity )
	{
		// Render it if its an IGraphicalObj* w/ RenderAlpha() function
		IAlphaGraphicalObj* o = dynamic_cast<IAlphaGraphicalObj*>(entity->data);
		if ( o != NULL )
		{
			glPushMatrix();	// Save Modelview Matrix @ Camera Position
			o->RenderAlpha();
			glPopMatrix();	// Restore it
		}
		
		entity = entity->GetPrev();
	}

	/*	Setup for Orthographic Rendering	*/
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
	glOrtho( 0.0, 1.0, (1.0/SCR_WIDTH*SCR_HEIGHT), 0.0, 0.5f, 2.0f );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	/*		Render Ortho		*/
	entity = gEntities.GetFirst();
	while ( entity )
	{
		// Render it if its an IGraphicalObj* w/ RenderOrtho() function
		IOrthoGraphicalObj* o = dynamic_cast<IOrthoGraphicalObj*>(entity->data);
		if ( o != NULL )
		{
			glLoadIdentity();
			glTranslatef( 0.0f, 0.0f, -1.0f );
			o->RenderOrtho();
		}
		entity = entity->GetNext();
	}

	/*	Reset Projection Matrix		*/
	glMatrixMode( GL_PROJECTION );
	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );


	SDL_GL_SwapBuffers( );	

	return 1;
}
