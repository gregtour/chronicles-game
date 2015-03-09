#include "music.h"
#include "../common/log.h"
#include "../engine.h"

CMusic::CMusic()
{
	mLoaded = false;
}

CMusic::CMusic( std::string file )
{
	mLoaded = false;
	Load( file );
}

CMusic::~CMusic()
{
	Unload();
}

void CMusic::Load( std::string file )
{
	if ( !mLoaded )
	{
		mMusic = Mix_LoadMUS( file.c_str() );
		if ( mMusic )
			mLoaded = true;
		else
		{
			mLoaded = false;
			std::string message = "LoadMUS failed on file " + file;
			gLog.LogItem( new CLogMessage(message) );
		}
	}
}


void CMusic::Unload()
{
	if ( mLoaded )
	{
		Mix_FreeMusic( mMusic );
		mLoaded = false;
	}
}

/* Returns the channel of the playing sound,
	-1 means failure				*/
void CMusic::Play( int loops )
{
#ifndef NO_MUSIC
	if ( mLoaded )
	{
		Mix_PlayMusic( mMusic, loops );
	}
#endif
}

bool CMusic::IsLoaded()
{
	return mLoaded;
}
