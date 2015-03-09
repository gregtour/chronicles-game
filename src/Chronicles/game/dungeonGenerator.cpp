#include "dungeonGenerator.h"
#include <math.h>

#include "../../engine/graphics/lighting.h"
#include "../entity/ghoul.h"
#include "../entity/ghost.h"
#include "../entity/monster.h"
#include "../entity/gremling.h"
#include "../entity/boss.h"
#include "../entity/bat.h"
#include "../entity/item.h"
#include "../entity/portal.h"
#include "../entity/pickup.h"
#include "../entity/gate.h"
#include "../entity/spike.h"

#include "../entity/particles/fountain.h"
#include "../entity/particles/fire.h"
#include "../entity/particles/smoke.h"
#include "../entity/particles/spin.h"
#include "../entity/particles/death.h"

CDungeonNode::CDungeonNode( int size, bool directions[4] )
{
	this->size = size;
	for ( int i = 0; i < 4; i++ )
		this->directions[i] = directions[i];
}


CDungeonGenerator::CDungeonGenerator( int minRoomSize, int maxRoomSize, int pathLength, int pathTurns, int subPaths, int subPathLength, int hallwaySize, int mapSize )
{
	mMinRoomSize = minRoomSize;
	mMaxRoomSize = maxRoomSize;
	mMainPathLength = pathLength;
	mMainPathTurns = pathTurns;
	mNumSubPaths = subPaths;
	mSubPathLength = subPathLength;
	mHallwaySize = hallwaySize;
	mMapSize = mapSize;

	mMap = new CDungeonNode*[ mMapSize * mMapSize ];

	for ( int x = 0; x < mMapSize; x++ )
		for ( int y = 0; y < mMapSize; y++ )
			mMap[ x + y*mMapSize ] = NULL;

}

CDungeonGenerator::~CDungeonGenerator()
{
	ClearData();
	delete[] mMap;
}

			

void CDungeonGenerator::AddFloor( CLevel* level, SFloor* floor, int tesselations )
{
	if ( tesselations == 0 )
	{
		level->mFloor.Add( floor );
	} 
	else 
	{
		for ( int i = 0; i < 4; i++ )
		{
			SFloor* smallFloor = new SFloor();
			for ( int x = 0; x < 4; x++ )
			{
				SPoint p = floor->points[i];
				p.Add( &floor->points[x] );
				p.x /= 2.0f;
				p.y /= 2.0f;
				smallFloor->points[x] = p;
			}
			AddFloor( level, smallFloor, tesselations-1 );
		}

		delete floor;
	}
}

void CDungeonGenerator::GenerateLevel( CLevel* level, int seed, int difficulty )
{
	srand( seed );

	GenerateMap( difficulty );
	CreateLevel( level, difficulty );
	ClearData();
}


void CDungeonGenerator::CreateLevel( CLevel* level, int difficulty )
{
	/* UNDONE - Should create level data into some kind of data structure
		and should create graphics in a vertex list	*/

	level->RandomLevelInit();
	PlaceObjects( level, difficulty );

	for ( int x = 0; x < mMapSize; x++ )
		for ( int y = 0; y < mMapSize; y++ )
			if ( mMap[ x + y*mMapSize ] )
			{
				SPoint mapOffset( (x+1) * mMaxRoomSize * 2.0f, (y+1) * mMaxRoomSize * 2.0f );
				float angle = -90.0f * RADS;

				if ( mMap[ x + y*mMapSize ]->room == DUNGEON_START_ROOM )
				{
					level->mPlayerX = (x+1) * mMaxRoomSize * 2.0f;
					level->mPlayerZ = (y+1) * mMaxRoomSize * 2.0f;
//					level->mBoundCamera->SetPosition( level->mPlayerX, 10.0f, level->mPlayerZ );
				}

				SFloor* f = new SFloor();
				
				for ( int i = 0; i < 4; i++ )
				{
					if ( mMap[ x + y*mMapSize ]->directions[i] )
					{
						SPoint point1;
						point1.Set( cos( angle - 45.0f*RADS ) * mMap[ x + y*mMapSize ]->size,
							sin( angle - 45.0f*RADS ) * mMap[ x + y*mMapSize ]->size );

						SPoint point2;
						point2.Set( cos( angle ) * mMap[ x + y*mMapSize ]->size * ROOT_TWO / 2.0f,
							sin( angle ) * mMap[ x + y*mMapSize ]->size * ROOT_TWO / 2.0f );
						point2.x += cos( angle - 90.0f * RADS ) * mHallwaySize;
						point2.y += sin( angle - 90.0f * RADS ) * mHallwaySize;

						SPoint point3 = point2;
						point3.x += cos( angle ) * (mMaxRoomSize - (mMap[ x + y*mMapSize ]->size ) * ROOT_TWO / 2.0f);
						point3.y += sin( angle ) * (mMaxRoomSize - (mMap[ x + y*mMapSize ]->size ) * ROOT_TWO / 2.0f);


						SPoint point6;
						point6.Set( cos( angle + 45.0f*RADS ) * mMap[ x + y*mMapSize ]->size,
							sin( angle + 45.0f*RADS ) * mMap[ x + y*mMapSize ]->size );

						SPoint point5;
						point5.Set( cos( angle ) * mMap[ x + y*mMapSize ]->size * ROOT_TWO / 2.0f,
							sin( angle ) * mMap[ x + y*mMapSize ]->size * ROOT_TWO / 2.0f );
						point5.x += cos( angle + 90.0f * RADS ) * mHallwaySize;
						point5.y += sin( angle + 90.0f * RADS ) * mHallwaySize;

						SPoint point4 = point5;
						point4.x += cos( angle ) * (mMaxRoomSize - (mMap[ x + y*mMapSize ]->size ) * ROOT_TWO / 2.0f);
						point4.y += sin( angle ) * (mMaxRoomSize - (mMap[ x + y*mMapSize ]->size ) * ROOT_TWO / 2.0f);
						
						point1.Add( &mapOffset );
						point2.Add( &mapOffset );
						point3.Add( &mapOffset );
						point4.Add( &mapOffset );
						point5.Add( &mapOffset );
						point6.Add( &mapOffset );

						level->mWorld->Add( (CPObject*)(
							new CPLine( &point2, &point1, 
								new SPoint(), new SVector(), false, WALL_OBJ_TYPE )
						));

						level->mWorld->Add( (CPObject*)(
							new CPLine( &point3, &point2, 
								new SPoint(), new SVector(), false, WALL_OBJ_TYPE )
						));

						level->mWorld->Add( (CPObject*)(
							new CPLine( &point5, &point4, 
								new SPoint(), new SVector(), false, WALL_OBJ_TYPE )
						));

						level->mWorld->Add( (CPObject*)(
							new CPLine( &point6, &point5, 
								new SPoint(), new SVector(), false, WALL_OBJ_TYPE )
						));

						SFloor* floor = new SFloor;
						floor->points[0] = point5;
						floor->points[1] = point4;
						floor->points[2] = point3;
						floor->points[3] = point2;

						if ( i == 2 && mMap[ x + y*mMapSize ]->room == DUNGEON_LOCKED_ROOM )
						{
							CGate* gate = new CGate();
							gate->CreatePhysical( level->mWorld, &point4, &point3 );
							level->mObjectsData.Add( (CEntity*)gate );
						}
						else if ( difficulty > 4 && rand()%4 == 1 )
						{
							CSpike* spike = new CSpike( 0.5f + (rand()%250/100.0f) );
							spike->CreatePhysical( level->mWorld, &point4, &point3 );
							level->mObjectsData.Add( (CEntity*)spike );
						}


						#ifdef LOW_QUALITY_GEOMETRY
						AddFloor( level, floor, 0 );
						#else
						AddFloor( level, floor, 1 );
						#endif
						
//						level->mFloor.Add( floor );


						f->points[3-i] = point1;

					} 
					else //*/
					{
						SPoint point1;
						SPoint point2;
						point1.Set( cos( angle - 45.0f*RADS ) * mMap[ x + y*mMapSize ]->size,
							sin( angle - 45.0f*RADS ) * mMap[ x + y*mMapSize ]->size );

						point2.Set( cos( angle + 45.0f*RADS ) * mMap[ x + y*mMapSize ]->size,
							sin( angle + 45.0f*RADS ) * mMap[ x + y*mMapSize ]->size );
						

						point1.Add( &mapOffset );
						point2.Add( &mapOffset );

						level->mWorld->Add( (CPObject*)(
							new CPLine( &point2, &point1, 
								new SPoint(), new SVector(), false, WALL_OBJ_TYPE )
						));

						f->points[3-i] = point1;
					}

					angle += 90.0f * RADS;
				}

				#ifdef LOW_QUALITY_GEOMETRY
				AddFloor( level, f, 2 );
				#else
				AddFloor( level, f, 3 );
				#endif
				
			}
}


void CDungeonGenerator::PlaceObjects( CLevel* level, int difficulty )
{
	/* UNDONE - Should place objects more randomly, also needs to handle END_ROOM
		and should create some more details in the level, for more depth to the game */

	level->mPortal = true;

	int minGremlingsPerRoom = 1;
	int maxGremlingsPerRoom = 1 + difficulty;
//	if ( maxGremlingsPerRoom > 10 )
//		maxGremlingsPerRoom = 6;

	int batsPerRoom = difficulty - 1;
//	if ( batsPerRoom > 4 )
//		batsPerRoom = 4;

	for ( int x = 0; x < mMapSize; x++ )
		for ( int y = 0; y < mMapSize; y++ )
		{
			if ( mMap[ x + y*mMapSize ] )
			{
				SPoint mapOffset( (x+1) * mMaxRoomSize * 2.0f, (y+1) * mMaxRoomSize * 2.0f );

				CLight* light = new CLight();
				GLfloat position[] = {mapOffset.x, 14.0f, mapOffset.y, 1.0f};
				GLfloat ambient[] = {0.8f, 0.3f, 0.3f, 1.0f}; 
				GLfloat diffusive[] = {0.9f, 0.0f, 0.0f, 1.0f};
				GLfloat attenuation[] = {0.0f, 0.07f, 0.0017f};
				light->SetPosition( position );
				light->SetAmbient( ambient );
				light->SetDiffusive( diffusive );
				light->SetAttenuation( attenuation );

				if ( mMap[ x + y*mMapSize ]->room == DUNGEON_NORMAL_ROOM  )
				{
					for ( int i = 0; i < batsPerRoom; i++ )
					{
						CMonster* monster = new CBat( -40.7f, 1.0f, 2.0f, 2.0f );
						monster->SetHealth( 2 );
						monster->SetRot( i * HALF_PI );
						SPoint pos( cos( i * HALF_PI ) * mMap[ x + y*mMapSize ]->size / 2.0f,
							sin( i * HALF_PI ) * mMap[ x + y*mMapSize ]->size / 2.0f );
						pos.Add( &mapOffset );
						((IPhysicalObj*)monster)->CreatePhysical( level->mWorld, &pos, new SVector(), 0.4f, 5.0f, 8.0f, 0.01f, true, BAT_OBJ_TYPE );
						level->mObjectsData.Add( (CEntity*)monster );
					}

					int gremlings = minGremlingsPerRoom + rand()%(maxGremlingsPerRoom-minGremlingsPerRoom);

					for ( int i = 0; i < gremlings; i++ )
					{
						CMonster* monster = new CGremling( 10.0f, 12.0f );
						monster->SetHealth( 3 );
						monster->SetRot( i * HALF_PI );
						SPoint pos( cos( (i+1.5f) * HALF_PI ) * mMap[ x + y*mMapSize ]->size / 7.0f,
							sin( (i+1.5f) * HALF_PI ) * mMap[ x + y*mMapSize ]->size / 7.0f );
						pos.Add( &mapOffset );
						((IPhysicalObj*)monster)->CreatePhysical( level->mWorld, &pos, new SVector(), 0.8f, 5.0f, 5.0f, 0.01f, true, MONSTER_OBJ_TYPE );
						level->mObjectsData.Add( (CEntity*)monster );
					}
				}
				else if ( mMap[ x + y*mMapSize ]->room == DUNGEON_ITEM_ROOM  )
				{
					for ( int i = 0; i < batsPerRoom; i++ )
					{
						CMonster* monster = new CBat( -40.7f, 1.0f, 2.0f, 2.0f );
						monster->SetHealth( 2 );
						monster->SetRot( i * HALF_PI );
						SPoint pos( cos( i * HALF_PI ) * mMap[ x + y*mMapSize ]->size / 2.0f,
							sin( i * HALF_PI ) * mMap[ x + y*mMapSize ]->size / 2.0f );
						pos.Add( &mapOffset );
						((IPhysicalObj*)monster)->CreatePhysical( level->mWorld, &pos, new SVector(), 0.4f, 5.0f, 8.0f, 0.01f, true, BAT_OBJ_TYPE );
						level->mObjectsData.Add( (CEntity*)monster );
					}

					int gremlings = minGremlingsPerRoom + rand()%(maxGremlingsPerRoom-minGremlingsPerRoom);

					for ( int i = 0; i < gremlings; i++ )
					{
						CMonster* monster = new CGremling( 10.0f, 12.0f );
						monster->SetHealth( 3 );
						monster->SetRot( i * HALF_PI );
						SPoint pos( cos( (i+1.5f) * HALF_PI ) * mMap[ x + y*mMapSize ]->size / 7.0f,
							sin( (i+1.5f) * HALF_PI ) * mMap[ x + y*mMapSize ]->size / 7.0f );
						pos.Add( &mapOffset );
						((IPhysicalObj*)monster)->CreatePhysical( level->mWorld, &pos, new SVector(), 0.8f, 5.0f, 5.0f, 0.01f, true, MONSTER_OBJ_TYPE );
						level->mObjectsData.Add( (CEntity*)monster );
					}

					/*
					for ( int i = 0; i < 5; i++ )
					{
						CItem* item = new CItem();	// "Soccer Ball"
						SPoint pos( cos( i * 40.0f * RADS ) * 5.0f , sin( i * 40.0f * RADS ) * 5.0f  );
						pos.Add( &mapOffset );
						((IPhysicalObj*)item)->CreatePhysical( level->mWorld, &pos, new SVector(), 1.0f, 2.5f, 0.7f, 0.9f, true, ITEM_OBJ_TYPE );
						level->mObjectsData.Add( (CEntity*)item );
					}

					for ( int i = 0; i < 2; i++ )
					{
						CItem* item = new CItem();	// "Soccer Ball"
						SPoint pos( cos( i * PI ) * 6.0f + 8.0f, sin( i * PI ) * 6.0f  );
						pos.Add( &mapOffset );
						((IPhysicalObj*)item)->CreatePhysical( level->mWorld, &pos, new SVector(), 2.0f, 7.0f, 3.1f, 0.9f, true, ITEM_OBJ_TYPE );
						level->mObjectsData.Add( (CEntity*)item );
					}
					*/

					GLfloat ambient[] = {0.3f, 0.7f, 0.3f, 1.0f};
					GLfloat diffusive[] = {0.3f, 0.7f, 0.3f, 1.0f};
					light->SetAmbient( ambient );
					light->SetDiffusive( diffusive );
				}
				else if ( mMap[ x + y*mMapSize ]->room == DUNGEON_KEY_ROOM  )
				{

					for ( int i = 0; i < 4; i++ )
					{
						CMonster* monster = new CGhost( -40.7f );
						monster->SetHealth( 2 );
						monster->SetRot( i * HALF_PI + PI );
						SPoint pos( cos( i * HALF_PI ) * 6.0f,
							sin( i * HALF_PI ) * 6.0f );
						pos.Add( &mapOffset );
						((IPhysicalObj*)monster)->CreatePhysical( level->mWorld, &pos, new SVector(), 0.4f, 5.0f, 8.0f, 0.01f, true, BAT_OBJ_TYPE );
						level->mObjectsData.Add( (CEntity*)monster );
					}


					/* Key	*/
					SPoint pos( mapOffset.x, mapOffset.y );
					SVector vel( 0.0f, 0.0f );
					CPickup* pickup = new CPickup( KEY_PICKUP_TYPE );
					((IPhysicalObj*)pickup)->CreatePhysical( level->mWorld, &pos, &vel, 0.5f, 2.0f, 7.0f, 0.7f, true, PICKUP_OBJ_TYPE );
					level->mObjectsData.Add( (CEntity*)pickup );

					GLfloat ambient[] = {0.8f, 0.8f, 0.1f, 1.0f};
					GLfloat diffusive[] = {0.8f, 0.8f, 0.1f, 1.0f};
					light->SetAmbient( ambient );
					light->SetDiffusive( diffusive );				}
				else if ( mMap[ x + y*mMapSize ]->room == DUNGEON_END_ROOM  )
				{
					level->mPortalX = mapOffset.x;
					level->mPortalZ = mapOffset.y;

					GLfloat ambient[] = {0.0f, 0.0f, 2.0f, 1.0f}; 
					GLfloat diffusive[] = {0.0f, 0.0f, 2.0f, 1.0f};
					position[1] = 2.0f;
					//GLfloat spotDirection[] = {0.0f, -1.0f, 0.0f};
					light->SetAmbient( ambient );
					light->SetDiffusive( diffusive );
					light->SetPosition( position );
					//light->Spotlight( spotDirection, 0.0f, 45.0f );
					//level->mObjectsData.Add( new CFountain( mapOffset.x, 0.0f, mapOffset.y ) );
				}
				else if ( mMap[ x + y*mMapSize ]->room == DUNGEON_START_ROOM )
				{
					GLfloat ambient[] = {0.1f, 0.1f, 1.0f, 1.0f};
					GLfloat diffusive[] = {1.0f, 0.1f, 0.1f, 1.0f}; 
					light->SetAmbient( ambient );
					light->SetDiffusive( diffusive );
//					level->mObjectsData.Add( new CFountain( mapOffset.x + 6.0f, 1.0f, mapOffset.y ) );
//					level->mObjectsData.Add( new CFire( mapOffset.x - 6.0f, 1.0f, mapOffset.y ) );
					level->mObjectsData.Add( new CSpin( mapOffset.x, 1.0f, mapOffset.y ) );
//					level->mObjectsData.Add( new CDeath( mapOffset.x, 1.0f, mapOffset.y + 6.0f, 0.0f, 1.0f, 0.0f, 1.0f ) );
//					level->mObjectsData.Add( new CSmoke( mapOffset.x, 1.0f, mapOffset.y ) );

					/*	Ghost Test	*/
/*					CMonster* monster = new CGhoul( 12.0f );
					monster->SetHealth( 3 );
					monster->SetRot( 0 * HALF_PI );
					((IPhysicalObj*)monster)->CreatePhysical( level->mWorld, &mapOffset, new SVector(), 0.8f, 5.0f, 5.0f, 0.01f, true, GHOUL_OBJ_TYPE );
					level->mObjectsData.Add( (CEntity*)monster );
*/
				}
				else if ( mMap[ x + y*mMapSize ]->room == DUNGEON_GEM_ROOM )
				{
					if ( difficulty > 4 )
					{
						/* Second Boss Guy	*/
						CMonster* monster = new CBoss( 12.0f, 15.0f );
						monster->SetHealth( 7 );
						monster->SetRot( 103.5f );
						SPoint pos( -5.0f, 5.0f );
						pos.Add( &mapOffset );
						((IPhysicalObj*)monster)->CreatePhysical( level->mWorld, &pos, new SVector(), 2.5f, 15.0f, 6.0f, 0.01f, true, MONSTER_OBJ_TYPE );
						level->mObjectsData.Add( (CEntity*)monster );
					}

					if ( difficulty > 5 )
					{
						for ( int i = 0; i < 4; i+=2 )
						{
							CMonster* monster = new CGhost( -40.7f );
							monster->SetHealth( 2 );
							monster->SetRot( i * HALF_PI + PI );
							SPoint pos( cos( i * HALF_PI ) * 6.0f,
								sin( i * HALF_PI ) * 6.0f );
							pos.Add( &mapOffset );
							((IPhysicalObj*)monster)->CreatePhysical( level->mWorld, &pos, new SVector(), 0.4f, 5.0f, 8.0f, 0.01f, true, BAT_OBJ_TYPE );
							level->mObjectsData.Add( (CEntity*)monster );
						}

						for ( int i = 1; i <4; i+=2 )
						{
							CMonster* monster = new CBat( -40.7f, 1.0f, 2.0f, 2.0f );
							monster->SetHealth( 2 );
							monster->SetRot( i * HALF_PI );
							SPoint pos( cos( i * HALF_PI ) * mMap[ x + y*mMapSize ]->size / 2.0f,
								sin( i * HALF_PI ) * mMap[ x + y*mMapSize ]->size / 2.0f );
							pos.Add( &mapOffset );
							((IPhysicalObj*)monster)->CreatePhysical( level->mWorld, &pos, new SVector(), 0.4f, 5.0f, 8.0f, 0.01f, true, BAT_OBJ_TYPE );
							level->mObjectsData.Add( (CEntity*)monster );
						}

					}

					if ( difficulty > 6 )
					{
						/* Third Boss Guy	*/
						CMonster* monster = new CBoss( 16.0f, 19.0f );
						monster->SetHealth( 7 );
						monster->SetRot( 230.8f );
						SPoint pos( 5.0f, -5.0f );
						pos.Add( &mapOffset );
						((IPhysicalObj*)monster)->CreatePhysical( level->mWorld, &pos, new SVector(), 2.5f, 15.0f, 6.0f, 0.01f, true, MONSTER_OBJ_TYPE );
						level->mObjectsData.Add( (CEntity*)monster );
					}

					/* Boss Guy	*/
					CMonster* monster = new CBoss( 10.0f, 13.0f );
					monster->SetHealth( 5 );
					monster->SetRot( 0.0f );
					((IPhysicalObj*)monster)->CreatePhysical( level->mWorld, &mapOffset, new SVector(), 2.5f, 15.0f, 6.0f, 0.01f, true, MONSTER_OBJ_TYPE );
					level->mObjectsData.Add( (CEntity*)monster );


					level->mPortal = false;

/*
					SPoint pos( mapOffset.x, mapOffset.y );
					SVector vel( 0.0f, 0.0f );
					CPickup* pickup = new CPickup( GEM_PICKUP_TYPE );
					((IPhysicalObj*)pickup)->CreatePhysical( level->mWorld, &pos, &vel, 0.5f, 2.0f, 7.0f, 0.7f, true, PICKUP_OBJ_TYPE );
					level->mObjectsData.Add( (CEntity*)pickup );
*/

					GLfloat ambient[] = {0.2f, 0.7f, 0.7f, 1.0f};
					GLfloat diffusive[] = {0.2f, 0.6f, 0.7f, 1.0f};
					light->SetAmbient( ambient );
					light->SetDiffusive( diffusive );	
				}
				else if ( mMap[ x + y*mMapSize ]->room == DUNGEON_LOCKED_ROOM )
				{

					for ( int i = 0; i < 4; i++ )
					{
						CMonster* monster = new CGhost( -40.7f );
						monster->SetHealth( 2 );
						monster->SetRot( i * HALF_PI + PI );
						SPoint pos( cos( i * HALF_PI ) * 6.0f,
							sin( i * HALF_PI ) * 6.0f );
						pos.Add( &mapOffset );
						((IPhysicalObj*)monster)->CreatePhysical( level->mWorld, &pos, new SVector(), 0.4f, 5.0f, 8.0f, 0.01f, true, BAT_OBJ_TYPE );
						level->mObjectsData.Add( (CEntity*)monster );
					}

					/*	Normal Room + Locked Gate at Top	*/

					GLfloat ambient[] = {0.9f, 0.9f, 0.1f, 1.0f};
					GLfloat diffusive[] = {0.9f, 0.9f, 0.1f, 1.0f};
					light->SetAmbient( ambient );
					light->SetDiffusive( diffusive );		
				}
				

				level->mLights.Add( light );
			}
		}			

}



void CDungeonGenerator::ClearData()
{
	for ( int x = 0; x < mMapSize; x++ )
		for ( int y = 0; y < mMapSize; y++ )
			if ( mMap[ x + y*mMapSize ] )
			{
				delete mMap[ x + y*mMapSize ];
				mMap[ x + y*mMapSize ] = NULL;
			}
}
