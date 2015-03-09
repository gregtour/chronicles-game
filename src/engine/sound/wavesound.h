#ifndef _WAVESOUND_H
#define _WAVESOUND_H

#include "sound.h"

class CWaveSound
{
public:
	CWaveSound();
	CWaveSound( std::string file );
	~CWaveSound();

	void Load( std::string file );
	void Unload();
	int Play( int loops );

	bool IsLoaded();
	
private:
	bool mLoaded;
	Mix_Chunk* mWave;
};

class CManagedWaveSound: public CManagedResource
{
public:
	CManagedWaveSound( std::string );
	~CManagedWaveSound();

	static CWaveSound* Load( CResourceManager* m, std::string file );
	static bool Unload( CResourceManager* m, CWaveSound* ws );

	CWaveSound* GetSound();

private:
	bool LoadResource( std::string file );
	void UnloadResource();

	CWaveSound mData;
};

#endif
