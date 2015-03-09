#include <sstream>
#include "../main.h"

#include "spike.h"
#include "gate.h"
#include "ghoul.h"
#include "level.h"
#include "player.h"
#include "monster.h"
#include "gremling.h"
#include "bat.h"
#include "item.h"
#include "cameraController.h"
#include "portal.h"

#include "../../engine/update.h"
#include "../../engine/physics/physics.h"
#include "../../engine/graphics/graphics.h"
#include "../../engine/graphics/lighting.h"
#include "../../engine/entity/camera.h"
#include "../game/game.h"

#include <math.h>


CLevel::CLevel()
{
}

CLevel::CLevel( char* level )
{
}

/*
	~CLevel()
		Deletes any excess data, especially data created from Load().
*/
CLevel::~CLevel()
{
}


void CLevel::RandomLevelInit()
{
	mWorld = new CPhysics();

#ifdef LOW_RESOLUTION_TEXTURES
	mWallTexture = CManagedTexture::Load( &gResourceManager, "data/low/stonewall.bmp" );
	mFloorTexture = CManagedTexture::Load( &gResourceManager, "data/low/stonefloor.bmp" );
#else
	mWallTexture = CManagedTexture::Load( &gResourceManager, "data/stonewall.bmp" );
	mFloorTexture = CManagedTexture::Load( &gResourceManager, "data/stonefloor.bmp" );
#endif

	mLevelText = CManagedTexture::Load( &gResourceManager, "data/level.bmp" );
	mLevelMask = CManagedTexture::Load( &gResourceManager, "data/levelmask.bmp" );
	mLevelFont = new CFontRenderer( "data/digits.bmp", "data/digitsmask.bmp", 10, "0123456789" );

	/* Set up gWorld	*/
	mWorld->DisableBoundries();

	mPlayerY = 0.0f;
	mPlayerRot = 0.0f;
	mPlayerRadius = 1.2f;
	mPlayerBounce = 0.01f;
	mPlayerMass = 5.0f;
	mPlayerFriction = 7.0f;
	mPlayerDynamic = true;

	/*	Spawns	*/

		// None

	/*	Cameras	*/
	CCamera* cam = new CCamera();
	cam->SetPosition( 0.0f, 0.0f, 0.0f );
	cam->LookAt( 0.0f, 0.0f, 0.0f );
	mCameras.Add( cam );
	mBoundCamera = cam;
	
	/* Set up Controller for the Camera */
	CCameraController* camControl = new CCameraController();
	camControl->SetAnchored( false );
	camControl->SetViewDistance( 15.0f, 17.0f );
	camControl->ControlCamera( cam );
	camControl->FocusOnObject( NULL );
	camControl->SetViewAngle( -HALF_PI );
	mCameraControllers.Add( camControl );


	/*	Fog		*/
#ifdef NO_FOG
	mFog = false;
#else
	mFog = true;
#endif
	mFogColorR = 0.0f;
	mFogColorG = 0.0f;
	mFogColorB = 0.0f;
	mFogColorA = 1.0f; 
	mFogDensity = 0.25f;
	mFogStart = 35.0f;
	mFogEnd = 45.0f;
}

void CLevel::FakeLoad()
{
	mWorld = new CPhysics();

#ifdef LOW_RESOLUTION_TEXTURES
	mWallTexture = CManagedTexture::Load( &gResourceManager, "data/low/stonewall.bmp" );
	mFloorTexture = CManagedTexture::Load( &gResourceManager, "data/low/stonefloor.bmp" );
#else
	mWallTexture = CManagedTexture::Load( &gResourceManager, "data/low/stonewall.bmp" );
	mFloorTexture = CManagedTexture::Load( &gResourceManager, "data/low/stonefloor.bmp" );
#endif

	/* Set up gWorld	*/
	mWorld->SetBoundry( 0.0f, 0.0f, 60.0f, 60.0f );

	/* CCW ORDER!!! */
	mWorld->Add( (CPObject*)(new CPLine( new SPoint( 50.0f, 10.0f ), new SPoint( 10.0f, 10.0f ), 
		new SPoint(), new SVector(), false, WALL_OBJ_TYPE )) );

	mWorld->Add( (CPObject*)(new CPLine( new SPoint( 10.0f, 10.0f ), new SPoint( 10.0f, 50.0f ), 
		new SPoint(), new SVector(), false, WALL_OBJ_TYPE )) );

	mWorld->Add( (CPObject*)(new CPLine( new SPoint( 50.0f, 50.0f ), new SPoint( 50.0f, 10.0f ), 
		new SPoint(), new SVector(), false, WALL_OBJ_TYPE )) );

	mWorld->Add( (CPObject*)(new CPLine( new SPoint( 10.0f, 50.0f ), new SPoint( 50.0f, 50.0f ), 
		new SPoint(), new SVector(), false, WALL_OBJ_TYPE )) );


	//	Set up Player	
	mPlayerX = 15.0f;
	mPlayerY = 0.0f;
	mPlayerZ = 15.0f;
	mPlayerRot = 0.0f;
	mPlayerRadius = 1.2f;
	mPlayerBounce = 0.01f;
	mPlayerMass = 5.0f;
	mPlayerFriction = 7.0f;
	mPlayerDynamic = true;

	SPoint pos(0.0f, 0.0f);
	SVector vel(0.0f, 0.0f);

	/*	Monsters	*/
		/*	Gremlings	*/
		CMonster* monster = new CGremling( 8.0f, 10.0f );
		monster->SetHealth( 3 );
		pos.Set( 40.0f, 40.0f );
		((IPhysicalObj*)monster)->CreatePhysical( mWorld, &pos, &vel, 0.8f, 5.0f, 8.0f, 0.01f, true, MONSTER_OBJ_TYPE );
		mObjectsData.Add( (CEntity*)monster );

		monster = new CGremling( 8.0f, 10.0f );
		monster->SetHealth( 3 );
		pos.Set( 40.0f, 37.0f );
		((IPhysicalObj*)monster)->CreatePhysical( mWorld, &pos, &vel, 0.8f, 5.0f, 8.0f, 0.01f, true, MONSTER_OBJ_TYPE );
		mObjectsData.Add( (CEntity*)monster );

		monster = new CGremling( 8.0f, 10.0f );
		monster->SetHealth( 3 );
		pos.Set( 37.0f, 40.0f );
		((IPhysicalObj*)monster)->CreatePhysical( mWorld, &pos, &vel, 0.8f, 5.0f, 8.0f, 0.01f, true, MONSTER_OBJ_TYPE );
		mObjectsData.Add( (CEntity*)monster );

		monster = new CGremling( 8.0f, 10.0f );
		monster->SetHealth( 3 );
		pos.Set( 37.0f, 37.0f );
		((IPhysicalObj*)monster)->CreatePhysical( mWorld, &pos, &vel, 0.8f, 5.0f, 8.0f, 0.01f, true, MONSTER_OBJ_TYPE );
		mObjectsData.Add( (CEntity*)monster );

		/*	Bats		*/
		monster = new CBat( -40.1f, 1.0f, 2.0f, 2.0f );
		monster->SetHealth( 2 );
		monster->SetRot( 2.0f );
		pos.Set( 27.0f, 45.0f );
		((IPhysicalObj*)monster)->CreatePhysical( mWorld, &pos, &vel, 0.4f, 5.0f, 8.0f, 0.01f, true, BAT_OBJ_TYPE );
		mObjectsData.Add( (CEntity*)monster );

		monster = new CBat( -40.7f, 1.0f, 2.0f, 2.0f );
		monster->SetHealth( 2 );
		monster->SetRot( 2.0f );
		pos.Set( 27.0f, 45.0f );
		((IPhysicalObj*)monster)->CreatePhysical( mWorld, &pos, &vel, 0.4f, 5.0f, 8.0f, 0.01f, true, BAT_OBJ_TYPE );
		mObjectsData.Add( (CEntity*)monster );

		monster = new CBat( 40.0f, 1.0f, 1.9f, 2.0f);
		monster->SetHealth( 2 );
		monster->SetRot( 0.0f );
		pos.Set( 45.0f, 24.0f );
		((IPhysicalObj*)monster)->CreatePhysical( mWorld, &pos, &vel, 0.4f, 5.0f, 8.0f, 0.01f, true, BAT_OBJ_TYPE );
		mObjectsData.Add( (CEntity*)monster );

		monster = new CBat( 40.2f, 1.0f, 2.1f, 2.0f );
		monster->SetHealth( 2 );
		monster->SetRot( 3.0f );
		pos.Set( 48.0f, 47.0f );
		((IPhysicalObj*)monster)->CreatePhysical( mWorld, &pos, &vel, 0.4f, 5.0f, 8.0f, 0.01f, true, BAT_OBJ_TYPE );
		mObjectsData.Add( (CEntity*)monster );


	/* Items / Objects	*/
	CItem* item = new CItem();	// "Soccer Ball"
	pos.Set( 15.0f, 33.0f );
	((IPhysicalObj*)item)->CreatePhysical( mWorld, &pos, &vel, 1.0f, 2.5f, 0.7f, 0.9f, true, ITEM_OBJ_TYPE );
	mObjectsData.Add( (CEntity*)item );

	item = new CItem();	// "Soccer Ball"
	pos.Set( 15.0f, 35.0f );
	((IPhysicalObj*)item)->CreatePhysical( mWorld, &pos, &vel, 1.0f, 2.5f, 0.7f, 0.9f, true, ITEM_OBJ_TYPE );
	mObjectsData.Add( (CEntity*)item );

	item = new CItem();	// "Soccer Ball"
	pos.Set( 13.0f, 35.0f );
	((IPhysicalObj*)item)->CreatePhysical( mWorld, &pos, &vel, 1.0f, 2.5f, 0.7f, 0.9f, true, ITEM_OBJ_TYPE );
	mObjectsData.Add( (CEntity*)item );

	item = new CItem();	// "Big Soccer Ball"
	pos.Set( 13.0f, 33.0f );
	((IPhysicalObj*)item)->CreatePhysical( mWorld, &pos, &vel, 2.0f, 7.0f, 3.1f, 0.9f, true, ITEM_OBJ_TYPE );
	mObjectsData.Add( (CEntity*)item );

	item = new CItem();			// "Crate"
	pos.Set( 25.0f, 25.0f );
	((IPhysicalObj*)item)->CreatePhysical( mWorld, &pos, &vel, 4.0f, 20.0f, 10.0f, 0.1f, true, ITEM_OBJ_TYPE );
	mObjectsData.Add( (CEntity*)item );

	item = new CItem();			// "Tree"
	pos.Set( 45.0f, 15.0f );
	((IPhysicalObj*)item)->CreatePhysical( mWorld, &pos, &vel, 2.0f, 0.5f, 0.05f, 0.9f, false, ITEM_OBJ_TYPE );
	mObjectsData.Add( (CEntity*)item );


	/*	Spawns	*/

		// None

	/*	Cameras	*/
	CCamera* cam = new CCamera();
	cam->SetPosition( 30.0f, 25.0f, 30.0f );
	cam->LookAt( 0.0f, 0.0f, 0.0f );
	mCameras.Add( cam );
	mBoundCamera = cam;
	
	/* Set up Controller for the Camera */
	CCameraController* camControl = new CCameraController();
	camControl->SetAnchored( false );
	camControl->SetViewAngle( 0.75f );
	camControl->SetViewDistance( 15.0f, 17.0f );
	camControl->ControlCamera( cam );
	camControl->FocusOnObject( NULL );
	mCameraControllers.Add( camControl );

	/*	Set up Level Lighting	*/
/*	CLight* light1 = new SLight();

	light1->mX = 30.0f;
	light1->mY = 25.0f;
	light1->mZ = 30.0f;
	light1->mAR = 0.5f;
	light1->mAG = 0.5f;
	light1->mAB = 0.5f;
	light1->mAA = 1.0f;
	light1->mDR = 0.5f;
	light1->mDG = 0.5f;
	light1->mDB = 0.5f;
	light1->mDA = 1.0f;

	mLights.Add( light1 );
*/
	/*	Fog		*/
	mFog = false;
	mFogColorR = 0.0f;
	mFogColorG = 0.0f;
	mFogColorB = 0.0f;
	mFogColorA = 1.0f; 
	mFogDensity = 0.6f;
	mFogStart = 25.0f;
	mFogEnd = 30.0f;
}

/*
	Init()
		Clears current game set up and overwrites it with data stored in
		this CLevel object.
*/
void CLevel::Init()
{
	// Copy over the new CPhysics World
	if ( gWorld )
		delete gWorld;
	gWorld = mWorld;

	gWorld->SetCollisionFunction( &GameCollisionFunction );
	gWorld->SetObjectTypeCollisionsArraySize( NUM_OBJ_TYPES, true );

	gWorld->SetObjectTypeCollision( PLAYER_OBJ_TYPE, SWORD_OBJ_TYPE, false );
	gWorld->SetObjectTypeCollision( SWORD_OBJ_TYPE, MONSTER_OBJ_TYPE, false );
	gWorld->SetObjectTypeCollision( SWORD_OBJ_TYPE, ITEM_OBJ_TYPE, false );

	/*	Disable collision between bats and everything else (flying)	*/
	for ( int i = 0; i < NUM_OBJ_TYPES; i++ )
	{
		gWorld->SetObjectTypeCollision( BAT_OBJ_TYPE, i, false );
	}	

	gWorld->SetObjectTypeCollision( BAT_OBJ_TYPE, BAT_OBJ_TYPE, true );
	gWorld->SetObjectTypeCollision( BAT_OBJ_TYPE, WALL_OBJ_TYPE, true );

	for ( int i = 0; i < NUM_OBJ_TYPES; i++ )
	{
		gWorld->SetObjectTypeCollision( GHOUL_OBJ_TYPE, i, false );
	}	


	for ( int i = 0; i < NUM_OBJ_TYPES; i++ )
	{
		gWorld->SetObjectTypeCollision( PORTAL_OBJECT_TYPE, i, false );
	}	

	for ( int i = 0; i < NUM_OBJ_TYPES; i++ )
	{
		gWorld->SetObjectTypeCollision( PLAYER_LEVITATE_OBJ, i, false );
	}	

	for ( int i = 0; i < NUM_OBJ_TYPES; i++ )
	{
		gWorld->SetObjectTypeCollision( GATE_OBJ_TYPE, i, true );
	}	

	for ( int i = 0; i < NUM_OBJ_TYPES; i++ )
	{
		gWorld->SetObjectTypeCollision( SPIKE_OBJ_TYPE, i, false );
	}	

	// Clear the entity list
	CListEntry<CEntity>* entity = gEntities.GetFirst();
	while ( entity )
	{
		delete entity->data;
		gEntities.RemoveEntry( entity );
		entity = gEntities.GetFirst();
	}

	CPlayer* player = new CPlayer();
	SPoint pos( mPlayerX, mPlayerZ );
	SVector vel( 0.0f, 0.0f );
	((IPhysicalObj*)player)->CreatePhysical( gWorld, &pos, &vel, mPlayerRadius, mPlayerMass, mPlayerFriction, mPlayerBounce, mPlayerDynamic, PLAYER_OBJ_TYPE );
	player->SetRot( HALF_PI );
	gEntities.Add( (CEntity*)player );

	if ( mPortal )
	{
		CreatePortal();
	}


	CListEntry<CCamera>* cameras = mCameras.GetFirst();
	while ( cameras )
	{
		gEntities.Add( (CEntity*)cameras->data );
		cameras = cameras->GetNext();
	}
	BindCamera( mBoundCamera );

	CListEntry<CCameraController>* camControllers = mCameraControllers.GetFirst();
	while ( camControllers )
	{
		camControllers->data->FocusOnObject( player );
		gEntities.Add( (CEntity*)camControllers->data );
		camControllers = camControllers->GetNext();
	}

	CListEntry<CEntity>* objects = mObjectsData.GetFirst();
	while ( objects )
	{
		gEntities.Add( objects->data );
		objects = objects->GetNext();
	}

	RenderList();
}

void CLevel::CreatePortal()
{
	SVector vec( 0.0f, 0.0f );
	SPoint pos( mPortalX, mPortalZ );
	CPortal* endPortal = new CPortal();
	((IPhysicalObj*)endPortal)->CreatePhysical( gWorld, &pos, &vec, PORTAL_SIZE, 0.1f, 0.0f, 0.0f, false, PORTAL_OBJECT_TYPE );
	gEntities.Add( endPortal );
}

/*
	Shutdown()
		Clears this level out of the current game.
*/
void CLevel::Shutdown()
{
	CManagedTexture::Unload( &gResourceManager, mFloorTexture );
	CManagedTexture::Unload( &gResourceManager, mWallTexture );
	CManagedTexture::Unload( &gResourceManager, mLevelText );
	CManagedTexture::Unload( &gResourceManager, mLevelMask );
	delete mLevelFont;

	// Clear the entity list
	CListEntry<CEntity>* entity = gEntities.GetFirst();
	while ( entity )
	{
		delete entity->data;
		gEntities.RemoveEntry( entity );
		entity = gEntities.GetFirst();
	}
	
	// Copy over the old physics world
	delete gWorld;
	gWorld = NULL;
}

void CLevel::Update( float dt )
{
}


/*
	Lighting() and Render()
		Sets up drawing and draws current level.
*/
void CLevel::Lighting()
{
//	GLfloat globalAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat globalAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
//	GLfloat globalAmbient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	AmbientLighting( globalAmbient );

	glEnable( GL_COLOR_MATERIAL );
	glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
}


void CLevel::Light( float x, float y, float z )
{
	EnableLighting( &mLights, x, y, z );
}

void CLevel::Light( float x, float z )
{
	Light( x, 3.0f, z );
}

void CLevel::Light( SPoint p )
{
	Light( p.x, 3.0f, p.y );
}


void CLevel::Fog()
{

	if ( mFog )
	{
		GLfloat fogColor[4] = { mFogColorR, mFogColorG, mFogColorB, mFogColorA };
		glClearColor( mFogColorR, mFogColorG, mFogColorB, 1.0f );
		glFogi( GL_FOG_MODE, GL_LINEAR );	/*	TODO: Use COptions	*/
		glFogfv( GL_FOG_COLOR, fogColor );
		glFogf( GL_FOG_DENSITY, mFogDensity );
		glHint( GL_FOG_HINT, GL_DONT_CARE );
		glFogf( GL_FOG_START, mFogStart );
		glFogf( GL_FOG_END, mFogEnd );
		glEnable( GL_FOG );
	} else
		glDisable( GL_FOG );

}

void CLevel::Render()
{
	mFloorTexture->BindTexture();
	glCallList( mDisplayLists );

	mWallTexture->BindTexture();
	glCallList( mDisplayLists + 1 );

	/*
	if ( mDisplayList )
	{

	}
	else 
	{
		mDisplayList = glGenLists( 2 );
		glNewList( mDisplayList, GL_COMPILE );

		RenderLevel();

		glEndList();
	}
	*/
}

void CLevel::RenderList()
{
	mDisplayLists = glGenLists( 2 );
	glNewList( mDisplayLists, GL_COMPILE );

	Lighting();
	Fog();

	/*	Draw the room	*/

	glEnable( GL_TEXTURE_2D );
	
	glColor3f( 0.7f, 0.7f, 0.7f );
	glNormal3f( 0.0f, 1.0f, 0.0f );

	CListEntry<SFloor>* floors = mFloor.GetFirst();
	while ( floors )
	{
		SFloor* f = floors->data;

		Light( (f->points[0].x + f->points[3].x)/2.0f , (f->points[0].y + f->points[3].y)/2.0f );

		glBegin( GL_QUADS );
			for ( int i = 0; i < 4; i++ )
			{
				glTexCoord2f( f->points[i].x / 10.0f, f->points[i].y / 10.0f );
				glVertex3f( f->points[i].x,  0.0f, f->points[i].y );	
			}
		glEnd();

		floors = floors->GetNext();
	}

	glEndList();
	glNewList( mDisplayLists + 1, GL_COMPILE );


	/*	Loop through the CPhysics walls	*/
	CLinkedList<CPObject>* list = mWorld->GetObjectList();
	CListEntry<CPObject>* entry = list->GetFirst();
	while ( entry )
	{
		if ( entry->data->GetObjectType() == WALL_OBJ_TYPE )
		{
			CPLine* wall = (CPLine*)entry->data;
			float wx1 = wall->GetPoint1().x;
			float wz1 = wall->GetPoint1().y;
			float wx2 = wall->GetPoint2().x;
			float wz2 = wall->GetPoint2().y;

			int numTesselations;
			#ifdef LOW_QUALITY_GEOMETRY
			numTesselations = 1;
			#else
			numTesselations = 4;
			#endif

			for ( int i = 0; i < numTesselations; i++ )
			{
				float x1, x2, z1, z2;
				x1 = wx1 + (wx2 - wx1)*i/numTesselations;
				x2 = wx1 + (wx2 - wx1)*(i+1)/numTesselations;
				z1 = wz1 + (wz2 - wz1)*i/numTesselations;
				z2 = wz1 + (wz2 - wz1)*(i+1)/numTesselations;

				Light( (x1+x2)/2.0f, (z1+z2)/2.0f );

				SVector normal;
				wall->GetPoint1().Difference( &wall->GetPoint2(), &normal );
				normal.Rotate( 90.0f );
				normal.Normalize();
				glNormal3f( normal.x, 0.0f, normal.y );

				glBegin(GL_QUADS);

					glTexCoord2f( (x2+z2)*WALL_TEXTURE_SCALE, 0.0f );
					glVertex3f( x2,  0.0f, z2);

					glTexCoord2f( (x1+z1)*WALL_TEXTURE_SCALE, 0.0f );
					glVertex3f( x1,  0.0f, z1);

					glTexCoord2f( (x1+z1)*WALL_TEXTURE_SCALE, 2.0f );
					glVertex3f( x1, 20.0f, z1);

					glTexCoord2f( (x2+z2)*WALL_TEXTURE_SCALE, 2.0f );
					glVertex3f( x2, 20.0f, z2);
				glEnd();
			}

		}
		entry = entry->GetNext();
	}

	glDisable( GL_TEXTURE_2D );

	glEndList();
}

#define	MINI_MAP_CENTER_X	(7.0f / 40.0f)
#define	MINI_MAP_CENTER_Y	(7.0f / 40.0f)
#define MAP_SCALE			(700.0f)

void CLevel::RenderOrtho()
{
	if ( gPlayer )
	{
		glPushMatrix();

		glDisable( GL_LIGHTING );

		/*	Player Cursor	*/
		glColor3f( 0.0f, 1.0f, 0.0f );
		glBegin( GL_LINES );
			glVertex2f( MINI_MAP_CENTER_X,  MINI_MAP_CENTER_Y );
			glVertex2f( MINI_MAP_CENTER_X + (-0.5f / 40.0f),  MINI_MAP_CENTER_Y + (0.5f / 40.0f) );
			glVertex2f( MINI_MAP_CENTER_X,  MINI_MAP_CENTER_Y );
			glVertex2f( MINI_MAP_CENTER_X + ( 0.5f / 40.0f),  MINI_MAP_CENTER_Y + (0.5f / 40.0f) );
		glEnd();

		/*	Rotate & Translate the MiniMap Based on Player's Rotation & Position	*/
		glTranslatef( MINI_MAP_CENTER_X, MINI_MAP_CENTER_Y, 0.0f );
		glRotatef( gPlayer->GetRot() * DEGREES + 90.0f, 0.0f, 0.0f, -1.0f );
		glTranslatef( -gPlayer->GetPhysicalObject()->GetPosition().x / MAP_SCALE, 
			-gPlayer->GetPhysicalObject()->GetPosition().y / MAP_SCALE, 0.0f );


		//	Loop through the CPhysics walls
		CLinkedList<CPObject>* list = mWorld->GetObjectList();
		CListEntry<CPObject>* entry = list->GetFirst();

		glColor3f( 1.0f, 1.0f, 1.0f );
		while ( entry )
		{
			if ( entry->data->GetObjectType() == WALL_OBJ_TYPE )
			{
				CPLine* wall = (CPLine*)entry->data;
				float x1 = wall->GetPoint1().x;
				float z1 = wall->GetPoint1().y;
				float x2 = wall->GetPoint2().x;
				float z2 = wall->GetPoint2().y;
				x1 /= MAP_SCALE;
				x2 /= MAP_SCALE;
				z1 /= MAP_SCALE;
				z2 /= MAP_SCALE;

				glBegin( GL_LINES );

					glVertex2f( x2,  z2 );
					glVertex2f( x1,  z1 );
				glEnd();

			}
			entry = entry->GetNext();
		}

		glEnable( GL_LIGHTING );

		glPopMatrix();
	}

	if ( mLevelTextTimer.Time() < 4.0f )
	{
		glDisable( GL_LIGHTING );
		glEnable( GL_BLEND );
		glDisable( GL_DEPTH_TEST );
		glEnable( GL_TEXTURE_2D );

		glBlendFunc( GL_DST_COLOR, GL_ZERO );
		mLevelMask->BindTexture();

		glBegin( GL_QUADS );
			glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
			glTexCoord2f( 0.0f, 0.0f );
			glVertex2f( 0.4f, 0.45f );
			glTexCoord2f( 0.0f, 1.0f );
			glVertex2f( 0.4f, 0.55f );
			glTexCoord2f( 1.0f, 1.0f );
			glVertex2f( 0.6f, 0.55f );
			glTexCoord2f( 1.0f, 0.0f );
			glVertex2f( 0.6f, 0.45f );
		glEnd();

		glBlendFunc( GL_ONE, GL_ONE );
		mLevelText->BindTexture();

		glBegin( GL_QUADS );
			glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
			glTexCoord2f( 0.0f, 0.0f );
			glVertex2f( 0.4f, 0.45f );
			glTexCoord2f( 0.0f, 1.0f );
			glVertex2f( 0.4f, 0.55f );
			glTexCoord2f( 1.0f, 1.0f );
			glVertex2f( 0.6f, 0.55f );
			glTexCoord2f( 1.0f, 0.0f );
			glVertex2f( 0.6f, 0.45f );
		glEnd();

		glDisable( GL_TEXTURE_2D );
		glDisable( GL_BLEND );
		glEnable( GL_DEPTH_TEST );
		glEnable( GL_LIGHTING );

		std::stringstream stream;
		stream << gLevelNumber;
		std::string score = stream.str();
		mLevelFont->RenderString( score, 0.58f, 0.465f, 0.08f, 0.075f, 0.035f );
	}

}

