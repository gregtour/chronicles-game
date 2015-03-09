#include <math.h>
#include "cameraController.h"
#include "../../engine/graphics/graphics.h"
#include "../../engine/input/inputData.h"
#include "../main.h"

int gCameraMode = 0;

CCameraController::CCameraController()
{
	mCamera = NULL;
	mFocusObject = NULL;
	mDistanceSp = 0.0f;
	mRotSp = 0.0f;
	mAngle = 0.0f;
	SetViewDistance( 0.0f, 0.0f );
	SetViewAngle( 0.0f );
	SetAnchored( false );
}

CCameraController::~CCameraController()
{

}

void CCameraController::ControlCamera( CCamera* c )
{
	mCamera = c;
}

void CCameraController::FocusOnObject( IPhysicalObj* o )
{
	mFocusObject = o;
}

void CCameraController::SetViewDistance( float min, float max )
{
	mMinDistance = min;
	mMaxDistance = max;
}

void CCameraController::SetViewAngle( float a )
{
	mAngle = a;
}

void CCameraController::SetAnchored( bool a )
{
	mAllowMovement = !a;
}

void CCameraController::Update( float dt )
{
	if ( mFocusObject && (gPlayer) )
	{
		CPObject* obj = mFocusObject->GetPhysicalObject();
		SPoint objPosition = obj->GetPosition();
		float objectAngle = mFocusObject->GetRot() + PI;
		float objx = objPosition.x;
		float objz = objPosition.y;

		if ( mAllowMovement )
		{

			//	Track Camera Movement
			float cx, cy, cz;
			mCamera->GetPosition( &cx, &cy, &cz );
			
			mAngle += mRotSp * dt;
			// cameraAngle = gPlayer->GetMouse().mMouseX / 200.0f;
			// mAngle = gPlayer->GetMouse().mMouseY * 1.0f / 300.0f;
			
			
			// Above view!	//
//				cameraAngle = objectAngle;
			if ( gCameraMode == 0 )
			{
				mDistance = VIEWING_ANGLE * CAMERA_DISTANCE;
			} else {
				mDistance = VIEWING_ANGLE2 * CAMERA_DISTANCE2;
			}
			// ------------ //

		
			float angleOffset = ( objectAngle - mAngle );

			while ( angleOffset > 2*PI )
				angleOffset -= 2*PI;

			while ( angleOffset < 0.0f )
				angleOffset += (2*PI);

			if ( angleOffset > PI )
				angleOffset = angleOffset - 2*PI;
			
			mRotSp = angleOffset / 0.55f;
		

#ifdef CAMERA_NO_ROTATE
			mAngle = 0.0f;
#endif

			// Set Camera Position
			cx = (cos( mAngle ) * mDistance) + objx;
			cz = (sin( mAngle ) * mDistance) + objz;
			if ( gCameraMode == 0 )
			{
				cy = CAMERA_DISTANCE / VIEWING_ANGLE; // cos( mAngle ) * mDistance;
			} else {
				cy = CAMERA_DISTANCE2 / VIEWING_ANGLE2; // cos( mAngle ) * mDistance;
			}

			mCamera->SetPosition( cx, cy, cz );

			/*
			// Set Speed for Next Iteration
			float targetDistanceMove = 0.0f;

			if ( mDistance > mMaxDistance )
				targetDistanceMove = mMaxDistance - mDistance;
			else if ( mDistance < mMinDistance )
				targetDistanceMove = mMinDistance - mDistance;

			mDistanceSp = targetDistanceMove * 5.0f;
			*/
		}

		objx -= cos( mAngle ) * LOOK_AHEAD_DISTANCE;
		objz -= sin( mAngle ) * LOOK_AHEAD_DISTANCE;
		mCamera->LookAt( objx , 1.0f, objz );
	}
}
