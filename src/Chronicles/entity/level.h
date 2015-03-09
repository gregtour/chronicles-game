#ifndef _LEVEL_H
#define _LEVEL_H

#include "../../engine/entity/entity.h"
#include "../../engine/entity/graphicalObj.h"
#include "../../engine/common/list.h"
#include "../../engine/physics/physics.h"
#include "../../engine/graphics/texture.h"
#include "../../engine/graphics/font.h"
#include "../../engine/entity/camera.h"
#include "cameraController.h"
#include "../../engine/graphics/lighting.h"
#include "../../engine/common/timer.h"

#define WALL_OBJ_TYPE		4
#define STATIC_OBJ_TYPE		6

#define WALL_TEXTURE_SCALE	0.10f

struct SFloor
{
	SPoint points[4];
};

class CLevel: public CEntity, public IGraphicalObj, public IOrthoGraphicalObj
{
public:
	CLevel();
	CLevel( char* level );
	~CLevel();

	void RandomLevelInit();

	void FakeLoad();
	void Init();
	void Shutdown();

	void Update( float dt );
	void Render();
	void RenderList();
	void RenderOrtho();
	void Lighting();
	void Light( float x, float y, float z );
	void Light( float x, float z );
	void Light( SPoint p );
	void Fog();
	
	void CreatePortal();

private:

	// Graphical
	CTexture* mFloorTexture;
	CTexture* mWallTexture;
	CTexture* mLevelText;
	CTexture* mLevelMask;
	CFontRenderer*	mLevelFont;
	GLuint mDisplayLists;

	CTimer mLevelTextTimer;

public:
	// Physical
	CPhysics* mWorld;

	// Data
	CLinkedList<SFloor>	mFloor;
	CLinkedList<CCamera>	mCameras;
	CLinkedList<CCameraController>	mCameraControllers;
	CCamera* mBoundCamera;
	CLinkedList<CLight>		mLights;

	// Fog
	bool mFog;
	float mFogColorR, mFogColorG, mFogColorB, mFogColorA;
	float mFogDensity, mFogStart, mFogEnd;

	// Player
	float mPlayerX, mPlayerY, mPlayerZ, mPlayerRot;
	float mPlayerRadius, mPlayerBounce, mPlayerMass, mPlayerFriction;
	bool mPlayerDynamic;

	// Portal
	bool mPortal;	// Whether or not the portal initially exists
	float mPortalX, mPortalZ;

	// Objects
	CLinkedList<CEntity>	mObjectsData;
};

#endif
