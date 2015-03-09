#ifndef _DUNGEONGENERATOR_H
#define _DUNGEONGENERATOR_H

#include "levelGenerator.h"

#define	DUNGEON_NORMAL_ROOM		0
#define	DUNGEON_START_ROOM		1
#define	DUNGEON_END_ROOM		2
#define	DUNGEON_ITEM_ROOM		3
#define	DUNGEON_KEY_ROOM		4
#define DUNGEON_GEM_ROOM		5
#define DUNGEON_LOCKED_ROOM		6


struct CDungeonNode
{
	CDungeonNode( int size, bool directions[4] );

	int size;
	bool directions[4];
	int room;
};


class CDungeonGenerator: public CLevelGenerator
{
public:
	CDungeonGenerator( int minRoomSize, int maxRoomSize, int pathLength, int pathTurns, int subPaths, int subPathLength, int hallwaySize, int mapSize );
	~CDungeonGenerator();

	void GenerateLevel( CLevel* level, int seed, int difficulty );

protected:
	void AddFloor( CLevel* level, SFloor* floor, int tesselations );
	void GenerateMap( int difficulty );
	void CreateLevel( CLevel* level, int difficulty );
	void PlaceObjects( CLevel* level, int difficulty );
	void ClearData();

	void SetRoom( int x, int y, int roomType );
	void MakeHallways();

	int mMinRoomSize, mMaxRoomSize;
	int mMainPathLength, mMainPathTurns;
	int mNumSubPaths, mSubPathLength;
	int mHallwaySize;
	int mMapSize;
	CDungeonNode** mMap;
};


#endif
