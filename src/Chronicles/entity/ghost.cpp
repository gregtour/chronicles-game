#include "../../engine/engine.h"
#include "ghost.h"
#include "player.h"
#include "particles/death.h"
#include <math.h>

#define	SOUND_CUTOFF_DISTANCE	1500.0f
#define SOUND_REACH				10

CGhost::CGhost( float rotSpeed )
{
	mRotSpeed = rotSpeed;
	mAction = 0;

	mY = 6.0f;
	
	mSound = NULL;
	mGhostSound = CManagedWaveSound::Load( &gResourceManager, "data/ghost.wav" );

	LoadTexture();
}

CGhost::~CGhost()
{
	CManagedWaveSound::Unload( &gResourceManager, mGhostSound );
	if ( mSound )
	{
		if ( mSound->IsPlaying() )
			mSound->Stop();
		delete mSound;
	}
}

void CGhost::LoadTexture()
{
	mTexture = CManagedTexture::Load( &gResourceManager, "data/ghost.bmp" );
}


void CGhost::Update( float dt )
{

	// Object Sound
	{
		float mX = mObject->GetPosition().x;
		float mZ = mObject->GetPosition().y;
		float x, y, z;
		gCamera->GetPosition( &x, &y, &z );
		float distance = (x-mX)*(x-mX) + (z-mZ)*(z-mZ);

		if ( distance < SOUND_CUTOFF_DISTANCE )
		{
			distance = sqrt( distance );

			if ( !mSound )
			{
				mSound = new CSound( mGhostSound->Play( -1 ) );
			}

			int intDistance = (int)distance;
			if ( intDistance <= 0 )
				intDistance = 1;

			int volume = 128 / intDistance * SOUND_REACH;
			if ( volume <= 0 )
				volume = 1;
			if ( volume > 128 )
				volume = 128;
			
			mSound->SetVolume( volume );

		}
		else	// Out of range
		{
			if ( mSound )
			{
				if ( mSound->IsPlaying() )
					mSound->Stop();

				delete mSound;
				mSound = NULL;
			}
		}
	}

	if ( gPlayer )
	{


		/*	Manage Altitude	*/
		if ( mAction == 1 )
		{
			if ( mY > PLAYER_HEIGHT - 2.0f )
			{
				mY += ( -6.0f - PLAYER_HEIGHT - 2.0f ) * dt / 2.0f;
			}
		}

		/*	Fly Around Player	*/
		SVector distance;
		gPlayer->GetPhysicalObject()->GetPosition().Difference( &mObject->GetPosition(), &distance );
		float distanceToPlayer = distance.LengthSquared();

		/*	Distance	*/
		if ( mAction == 0 && distanceToPlayer < 8.0f * 8.0f )
			mAction = 1;
		if ( mAction == 1 && distanceToPlayer < 1.2f*1.2f && mY <= PLAYER_HEIGHT )
			mAction = 2;
		if ( mAction == 2 && mY >= 6.0f )
			mAction = 0;
		
		/*	Rotation	*/
		if ( mAction == 0 || mAction == 2 )	// Circle
		{
		//	mRot = atan( &distance );
			mRot += mRotSpeed * RADS * dt;
		}
		else if ( mAction == 1 )	// Charge at it
		{
			mRot = atan( &distance );
		} 
//		else						// Run Away
//		{
//			distance.Rotate( 180.0f );
//			mRot = atan( &distance );
//		}

		/*	Apply Movement	*/
		SVector force;
		force.x = cos( mRot ) * dt;
		force.y = sin( mRot ) * dt;

		if ( mAction == 0 )
			force.Scale( BAT_SPEED );
		else if ( mAction == 1 )
			force.Scale( BAT_DIVE_SPEED );
		else
			force.Scale( BAT_CLIMB_SPEED );

		if ( mAction == 2 && mY > PLAYER_HEIGHT )
			force.Scale( 2.0f );

		mObject->ApplyForce( &force );
	} else {
		mRot += mRotSpeed * RADS * dt;
		SVector force;
		force.x = cos( mRot ) * dt;
		force.y = sin( mRot ) * dt;
		force.Scale( BAT_SPEED );
		mObject->ApplyForce( &force );
	}
}

void CGhost::RenderAlpha()
{
	SPoint position = mObject->GetPosition();
	glTranslatef( position.x, mY, position.y );

	SPoint cameraPos;
	float y;
	gCamera->GetPosition( &cameraPos.x, &y, &cameraPos.y );

	SVector billboardAngle;
	position.Difference( &cameraPos, &billboardAngle );
	glRotatef( atan( &billboardAngle ) * DEGREES + 90.0f, 0.0f, -1.0f, 0.0f );

	glDisable( GL_CULL_FACE );
	glDisable( GL_LIGHTING );
	glEnable( GL_BLEND );
	glEnable( GL_TEXTURE_2D );
	glDisable( GL_FOG );
	glDepthMask( GL_FALSE );

	glBlendFunc( GL_ONE, GL_ONE );	
	mTexture->BindTexture();


	glBegin( GL_QUADS );
		glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
		
		glTexCoord2f( 0.0f, 0.0f );
		glVertex3f( -1.0f, 2.0f, 0.0f );

		glTexCoord2f( 0.0f, 1.0f );
		glVertex3f( -1.0f, 0.0f, 0.0f );

		glTexCoord2f( 1.0f, 1.0f );
		glVertex3f( 1.0f, 0.0f, 0.0f );

		glTexCoord2f( 1.0f, 0.0f );
		glVertex3f( 1.0f, 2.0f, 0.0f );
	glEnd();

	glDisable( GL_TEXTURE_2D );
	glDisable( GL_BLEND );
	glEnable( GL_LIGHTING );
	glEnable( GL_CULL_FACE );
	glDepthMask( GL_TRUE );
	glEnable( GL_FOG );

}

void CGhost::Render()
{	
}

void CGhost::DeathEffect()
{
	float x = mObject->GetPosition().x;
	float z = mObject->GetPosition().y;
	gEntities.Add( new CDeath( x, mY, z, 1.0f, 1.0f, 1.0f, 1.0f ) );
}
