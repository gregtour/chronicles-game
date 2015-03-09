#include "dungeonGenerator.h"

void CDungeonGenerator::SetRoom( int x, int y, int roomType )
{
	bool directions[4] =
	{ false, false, false, false };
	mMap[ x + y*mMapSize ] = new CDungeonNode( mMinRoomSize, directions );
	mMap[ x + y*mMapSize ]->room = roomType;
}

void CDungeonGenerator::MakeHallways()
{
	for ( int y = 0; y < mMapSize; y++ )
	{
		for ( int x = 0; x < mMapSize; x++ )
		{
			if ( mMap[ x + y*mMapSize ] != NULL )
			{
				if ( y == 0 || mMap[ x + (y-1)*mMapSize ] == NULL )
					mMap[ x + y*mMapSize ]->directions[0] = false;
				else
					mMap[ x + y*mMapSize ]->directions[0] = true;

				if ( x == mMapSize || mMap[ x+1 + y*mMapSize ] == NULL )
					mMap[ x + y*mMapSize ]->directions[1] = false;
				else
					mMap[ x + y*mMapSize ]->directions[1] = true;

				if ( y == mMapSize || mMap[ x + (y+1)*mMapSize ] == NULL )
					mMap[ x + y*mMapSize ]->directions[2] = false;
				else
					mMap[ x + y*mMapSize ]->directions[2] = true;

				if ( x == 0 || mMap[ x-1 + y*mMapSize ] == NULL )
					mMap[ x + y*mMapSize ]->directions[3] = false;
				else
					mMap[ x + y*mMapSize ]->directions[3] = true;
			}
		}
	}
}


void CDungeonGenerator::GenerateMap( int difficulty )
{

	switch ( difficulty )
	{
	case 1:
		SetRoom( 1, 0, DUNGEON_START_ROOM );
		SetRoom( 1, 1, DUNGEON_NORMAL_ROOM );
		SetRoom( 1, 2, DUNGEON_NORMAL_ROOM );
		SetRoom( 1, 3, DUNGEON_END_ROOM );
		break;

	case 2:
		SetRoom( 1, 0, DUNGEON_START_ROOM );
		SetRoom( 0, 1, DUNGEON_ITEM_ROOM );
		SetRoom( 1, 1, DUNGEON_NORMAL_ROOM );
		SetRoom( 2, 1, DUNGEON_NORMAL_ROOM );
		SetRoom( 3, 1, DUNGEON_END_ROOM );
		break;

	case 3:
		SetRoom( 0, 0, DUNGEON_START_ROOM );
		SetRoom( 0, 1, DUNGEON_NORMAL_ROOM );
		SetRoom( 0, 2, DUNGEON_NORMAL_ROOM );
		SetRoom( 1, 2, DUNGEON_NORMAL_ROOM );
		SetRoom( 2, 2, DUNGEON_GEM_ROOM );
		SetRoom( 3, 2, DUNGEON_ITEM_ROOM );
		SetRoom( 2, 1, DUNGEON_END_ROOM );
		break;

	case 4:
		SetRoom( 2, 0, DUNGEON_START_ROOM );
		SetRoom( 2, 1, DUNGEON_NORMAL_ROOM );
		SetRoom( 0, 3, DUNGEON_KEY_ROOM );
		SetRoom( 1, 1, DUNGEON_NORMAL_ROOM );
		SetRoom( 0, 1, DUNGEON_NORMAL_ROOM );
		SetRoom( 0, 2, DUNGEON_GEM_ROOM );
		SetRoom( 2, 2, DUNGEON_LOCKED_ROOM );
		SetRoom( 2, 3, DUNGEON_NORMAL_ROOM );
		SetRoom( 3, 3, DUNGEON_ITEM_ROOM );
		SetRoom( 4, 3, DUNGEON_END_ROOM );
		break;

	case 5:
		SetRoom( 2, 0, DUNGEON_START_ROOM );
		SetRoom( 2, 1, DUNGEON_NORMAL_ROOM );
		SetRoom( 2, 2, DUNGEON_NORMAL_ROOM );
		SetRoom( 0, 2, DUNGEON_KEY_ROOM );
		SetRoom( 1, 2, DUNGEON_NORMAL_ROOM );
		SetRoom( 3, 2, DUNGEON_NORMAL_ROOM );
		SetRoom( 4, 2, DUNGEON_GEM_ROOM );
		SetRoom( 2, 3, DUNGEON_NORMAL_ROOM );
		SetRoom( 2, 4, DUNGEON_LOCKED_ROOM );
		SetRoom( 2, 5, DUNGEON_END_ROOM );
		break;
	
	case 6:
		
		SetRoom( 0, 0, DUNGEON_START_ROOM );
		SetRoom( 0, 1, DUNGEON_GEM_ROOM );
		SetRoom( 0, 2, DUNGEON_NORMAL_ROOM );
		SetRoom( 0, 3, DUNGEON_NORMAL_ROOM );
		SetRoom( 0, 4, DUNGEON_NORMAL_ROOM );
		SetRoom( 1, 4, DUNGEON_GEM_ROOM );
		SetRoom( 2, 4, DUNGEON_NORMAL_ROOM );
		SetRoom( 2, 3, DUNGEON_NORMAL_ROOM );
		SetRoom( 2, 2, DUNGEON_GEM_ROOM );
		SetRoom( 2, 1, DUNGEON_END_ROOM );
		break;

	default:

		SetRoom( 0, 0, DUNGEON_START_ROOM );
		SetRoom( 1, 0, DUNGEON_NORMAL_ROOM );
		SetRoom( 2, 0, DUNGEON_GEM_ROOM );
		SetRoom( 2, 1, DUNGEON_NORMAL_ROOM );
		SetRoom( 3, 1, DUNGEON_ITEM_ROOM );
		SetRoom( 4, 1, DUNGEON_KEY_ROOM );
		SetRoom( 2, 2, DUNGEON_GEM_ROOM );
		SetRoom( 1, 2, DUNGEON_LOCKED_ROOM );
		SetRoom( 0, 2, DUNGEON_GEM_ROOM );
		SetRoom( 0, 3, DUNGEON_END_ROOM );
		break;
	}

	MakeHallways();
}
