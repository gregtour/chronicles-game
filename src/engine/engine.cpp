/*	Chronicles Game Engine	*/
/*	by Greg Tourville		*/

/* Current List Order:
	Player
		
	Camera
		CameraController
		
	Entities
		Monsters
		Items
		
	Level
*/


/*	 Library Includes	*/
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#ifdef WIN32
	#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include "SDL.h"
#include "SDL_mixer.h"

/*	Accessory Classes	*/
#include "common/list.h"
#include "common/timer.h"
#include "common/resource.h"
#include "common/log.h"

/*	Game Includes		*/
#include "entity/entity.h"
#include "entity/camera.h"

#include "engine.h"
#include "graphics/graphics.h"
#include "update.h"
#include "input/input.h"
#include "sound/sound.h"
#include "physics/physics.h"

/*	Global Variables	*/
bool gRunning;
bool gActive;
bool gPaused;

CLinkedList<CEntity>	gEntities;
CPhysics*				gWorld = NULL;
CLog					gLog;
CResourceManager		gResourceManager;
CLinkedList<CCollision> gCollisions;
float gFPS;
int gState;


/*	Code			*/

int Startup() 
{
	gState = DEFAULT_STATE;
	gRunning = true;
	gActive = true;
	gPaused = false;

#ifdef _DEBUG
	if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER  | SDL_INIT_NOPARACHUTE ) != 0 )
		return 0;	
#else
	if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER ) != 0)
		return 0;
#endif

	gLog.ResetTimeStamp();

	if (
		SoundStartup() &&
		GraphicsStartup() &&
		SetupInput() &&
		GamePrecache()
	)
		return 1;
	
	return 0;
}


void Shutdown() 
{ 
	GameUnload();
	SoundShutdown();
	GraphicsShutdown();
	SDL_Quit();

	gResourceManager.UnloadAll();
}



void EndGame()
{
	gRunning = false;
}

void ChangeGameState( int newState )
{
	gState = newState;
}


void SdlEvents()
{
	SDL_Event event;

	while ( SDL_PollEvent( &event ) )
	{
		switch( event.type )
		{
		/*	Window Focus	*/
		case SDL_ACTIVEEVENT:
			if ( event.active.gain == 0 )
				gActive = false;
			else
				gActive = true;
			break;	

		/* Any of the Input Event Types */
		case SDL_MOUSEMOTION:
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
		case SDL_JOYAXISMOTION:
		case SDL_JOYBUTTONDOWN:
		case SDL_JOYBUTTONUP:
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			InputEvent( &event );
			break;
		
		case SDL_QUIT:
			EndGame();
			break;
		}
	}
}




void Game()
{
	gLog.LogItem( new CLogMessage("Starting Up") );
	if ( Startup() == 0 )
	{
		gLog.LogItem( new CLogMessage("Error Starting Up") );
		gLog.LogItem( new CLogMessage("Shutting Down") );
		Shutdown();
		return;
	}

	gLog.LogItem( new CLogMessage("Starting Game") );
	GameStart();

	CGameTimer timer;
	timer.Clear();

	int oldState = gState;
	int fpsTick = 0;
	gFPS = 10.0f;

	gLog.LogItem( new CLogMessage("Entering Main Loop") );
	while ( gRunning ) 
	{
		if ( oldState != gState )
			GameStateChange( oldState, gState );
		oldState = gState;

		SdlEvents();
		timer.Step();
		
		fpsTick = (fpsTick+1) % (60);
		if ( fpsTick == 0 )
			gFPS = 1 / timer.DT();

		if ( gActive )
		{
			if ( !gPaused )
			{
				gWorld->Update( timer.DT() );
				HandleCollisions();
				GameUpdate( timer.DT() );				
			}

			GameRender();
		}

	}

	gLog.LogItem( new CLogMessage("Shutting Down") );
	Shutdown();
}
