/****************************
 * src/Chronicles/main.cpp
 *	by Greg Tourville
 *	March 8th, 2007
**/   

#include "SDL.h"
#include "../engine/engine.h"
#include "../engine/sound/music.h"

#include "game/game.h"
#include "game/splash.h"
#include "game/levelGenerator.h"
#include "game/dungeonGenerator.h"
#include "entity/level.h"
#include "entity/player.h"
#include "entity/hud.h"

#define NEXT_LEVEL_STATE	1

CLevel* gLevel = NULL;
CPlayer* gPlayer = NULL;
CMusic* gBackgroundMusic = NULL;
int gLevelNumber = 0;

const char* gameTitle = "Chronicles Game by Greg Tourville";

void NextLevel();




int main( int argc, char *argv[] )
{
	gLog.SetLogging( true );

	Game();

	gLog.SaveLog( "gamelog.log" );
	gLog.ClearLog();

	return 0;
}




int GamePrecache()
{
	/*	Load all game resources in advance	*/
	CManagedWaveSound::Load( &gResourceManager, "data/gremling.wav" );
	CManagedTexture::Load( &gResourceManager, "data/bat.bmp" );
	CManagedWaveSound::Load( &gResourceManager, "data/grunt.wav" );
	CManagedTexture::Load( &gResourceManager, "data/ghoul1.bmp" );
	CManagedTexture::Load( &gResourceManager, "data/ghoulMask.bmp" );
	CManagedTexture::Load( &gResourceManager, "data/ghoul.bmp" );
	CManagedTexture::Load( &gResourceManager, "data/gremling.bmp" );
	CManagedWaveSound::Load( &gResourceManager, "data/shriek.wav" );
	CManagedWaveSound::Load( &gResourceManager, "data/swish.wav" );
	CManagedTexture::Load( &gResourceManager, "data/sword.bmp" );
	CManagedTexture::Load( &gResourceManager, "data/swordmask.bmp" );
	CManagedWaveSound::Load( &gResourceManager, "data/spawn.wav" );
	CManagedTexture::Load( &gResourceManager, "data/portal.bmp" );
	CManagedTexture::Load( &gResourceManager, "data/shine.bmp" );
	CManagedWaveSound::Load( &gResourceManager, "data/warp.wav" );
	CManagedWaveSound::Load( &gResourceManager, "data/portal.wav" );
	CManagedTexture::Load( &gResourceManager, "data/key.bmp" );
	CManagedWaveSound::Load( &gResourceManager, "data/portalActive.wav" );
	CManagedTexture::Load( &gResourceManager, "data/stonewall.bmp" );
	CManagedTexture::Load( &gResourceManager, "data/stonefloor.bmp" );
	CManagedTexture::Load( &gResourceManager, "data/level.bmp" );
	CManagedTexture::Load( &gResourceManager, "data/levelmask.bmp" );

	gBackgroundMusic = new CMusic( "data/music.mp3" );
	return 1;
}



void GameUnload()
{
	delete gBackgroundMusic;
	gLevel->Shutdown();
}




void GameStart()
{
#ifndef _DEBUG
	SplashScreen();
#endif

	gBackgroundMusic->Play( -1 );
	InitPlayerStats();

	NextLevel();
}




void GameStateChange( int from, int to )
{
	switch ( to )
	{
	case NEXT_LEVEL_STATE:
		NextLevel();
		ChangeGameState( DEFAULT_STATE );
		break;
	default:
		EndGame();
	}

}



void NextLevel()
{
	gLevelNumber++;

	if ( gLevel )
		gLevel->Shutdown();

	CLevelGenerator* randomLevelGenerator = new CDungeonGenerator(
		25, 26, 3, 0, 0, 0, 5, 7
//		int minRoomSize, int maxRoomSize, int pathLength, int pathTurns, 
//		int subPaths, int subPathLength, int hallwaySize, int mapSize
	);

	gLevel = new CLevel();

	randomLevelGenerator->GenerateLevel( gLevel, 555, gLevelNumber );
	delete randomLevelGenerator;

	gLevel->Init();

	gEntities.Add( gLevel );
	gEntities.Add( new CHud() );
}

