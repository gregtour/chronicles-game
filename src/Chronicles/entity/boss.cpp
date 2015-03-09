#include "boss.h"
#include "gremling.h"
#include "pickup.h"
#include <math.h>
#include "particles/death.h"

#define	SOUND_CUTOFF_DISTANCE	2500.0f
#define SOUND_REACH				12

int gNumBosses = 0;

CBoss::CBoss( float min, float prefered )
{
	mMinDistanceToPlayer = min;
	mPreferedDistance = prefered;
	mRunning = false;
	mScuttle = 0.0f;
	LoadTexture();
	mSound = NULL;
	mGremSound = CManagedWaveSound::Load( &gResourceManager, "data/boss.wav" );
	LoadHitSound();
	gNumBosses++;

	mFaceTimer.Clear();
	mFaceTimer.Step();
}

CBoss::~CBoss()
{
	CManagedTexture::Unload( &gResourceManager, mFace );

	CManagedWaveSound::Unload( &gResourceManager, mGremSound );
	if ( mSound )
	{
		if ( mSound->IsPlaying() )
			mSound->Stop();
		delete mSound;
	}
	gNumBosses--;
}

void CBoss::LoadHitSound()
{
	mHitSound = CManagedWaveSound::Load( &gResourceManager, "data/bossHit.wav" );
}

void CBoss::LoadTexture()
{
#ifdef LOW_RESOLUTION_TEXTURES
	mTexture = CManagedTexture::Load( &gResourceManager, "data/low/gremling.bmp" );
#else
	mTexture = CManagedTexture::Load( &gResourceManager, "data/gremling.bmp" );
#endif
	mFace = CManagedTexture::Load( &gResourceManager, "data/gremling2.bmp" );
}

void CBoss::Update( float dt )
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
				mSound = new CSound( mGremSound->Play( -1 ) );
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
		SVector distance;
		gPlayer->GetPhysicalObject()->GetPosition().Difference( &mObject->GetPosition(), &distance );
		float distanceToPlayer = distance.LengthSquared();

		if ( mRunning )
		{
			if ( distanceToPlayer > mPreferedDistance*mPreferedDistance )
				mRunning = false;

			// Face Player
			mRot = atan( &distance );

			// Run!
			SVector force;
			force.x = cos( mRot + fabs( mScuttle ) - BOSS_SCUTTLE_INTENSITY/2.0f ) * 75.0f * dt;
			force.y = sin( mRot + fabs( mScuttle ) - BOSS_SCUTTLE_INTENSITY/2.0f ) * 75.0f * dt;
			mObject->ApplyForce( &force );

			mScuttle = mScuttle + BOSS_SCUTTLE_SPEED * dt;
			if ( mScuttle > BOSS_SCUTTLE_INTENSITY )
				mScuttle = -BOSS_SCUTTLE_INTENSITY;
		} else {
			if ( distanceToPlayer < mMinDistanceToPlayer*mMinDistanceToPlayer )
				mRunning = true;

			SVector force;
			force.x = cos( mRot ) * 15.0f * dt;
			force.y = sin( mRot ) * 15.0f * dt;
			mObject->ApplyForce( &force );
			mRot -= 0.5f * dt;
		}
	} else {
		SVector force;
		force.x = cos( mRot ) * 15.0f * dt;
		force.y = sin( mRot ) * 15.0f * dt;
		mObject->ApplyForce( &force );
		mRot -= 0.5f * dt;
	}
}

int CBoss::AttackStrength()
{
	return 3;
}

void CBoss::Render()
{	
	{
		float mX = mObject->GetPosition().x;
		float mZ = mObject->GetPosition().y;
		float x, y, z;
		gCamera->GetPosition( &x, &y, &z );
		if (  ((x-mX)*(x-mX) + (z-mZ)*(z-mZ)) > VIEW_DISTANCE_SQUARED )
			return;
	}
		

	gLevel->Light( mObject->GetPosition() );

	float r = ((CPCircle*)mObject)->GetRadius() * 0.9f;
	glTranslatef( mObject->GetPosition().x, r, mObject->GetPosition().y );
	glRotatef( (GLfloat)((mRot+( mRot + fabs( mScuttle ) - BOSS_SCUTTLE_INTENSITY/2.0f )) * DEGREES), 0.0f, -1.0f, 0.0f );

	glEnable( GL_TEXTURE_2D );
	if ( mFaceTimer.Time() > 0.75f )
	{
		mFace->BindTexture();
		if ( mFaceTimer.Time() > 1.5f )
			mFaceTimer.Step();
	}
	else
	{
		mTexture->BindTexture();
	}

	
	/*	Temporary Code, Draw A Cube as a Placeholder	*/
  glBegin(GL_QUADS);		// Draw The Cube Using quads

    glColor3f( 0.8f, 0.8f, 0.8f );
	if ( mLastHit.Time() < 1.0f )
		glColor3f( 1.0f - mLastHit.Time(), 0.4f, 0.4f );

	glNormal3f( 0.0f, 1.0f, 0.0f );

	glTexCoord2f( 0.0f, 0.0f );
    glVertex3f( r, r,-r);	// Top Right Of The Quad (Top)
	glTexCoord2f( 0.0f, 0.0f );
    glVertex3f(-r, r,-r);	// Top Left Of The Quad (Top)
	glTexCoord2f( 0.0f, 0.0f );
    glVertex3f(-r, r, r);	// Bottom Left Of The Quad (Top)
	glTexCoord2f( 0.0f, 0.0f );
    glVertex3f( r, r, r);	// Bottom Right Of The Quad (Top)

	glNormal3f( 0.0f, -1.0f, 0.0f );

	glTexCoord2f( 0.0f, 0.0f );
    glVertex3f( r,-r, r);	// Top Right Of The Quad (Bottom)
	glTexCoord2f( 0.0f, 0.0f );
    glVertex3f(-r,-r, r);	// Top Left Of The Quad (Bottom)
	glTexCoord2f( 0.0f, 0.0f );
    glVertex3f(-r,-r,-r);	// Bottom Left Of The Quad (Bottom)
	glTexCoord2f( 0.0f, 0.0f );
    glVertex3f( r,-r,-r);	// Bottom Right Of The Quad (Bottom)


	glNormal3f( 0.0f, 0.0f, 1.0f );

	glTexCoord2f( 0.0f, 0.0f );
    glVertex3f( r, r, r);	// Top Right Of The Quad (Front)
	glTexCoord2f( 1.0f, 0.0f );
    glVertex3f(-r, r, r);	// Top Left Of The Quad (Front)
	glTexCoord2f( 1.0f, 1.0f );
    glVertex3f(-r,-r, r);	// Bottom Left Of The Quad (Front)
	glTexCoord2f( 0.0f, 1.0f );
    glVertex3f( r,-r, r);	// Bottom Right Of The Quad (Front)
	

	glNormal3f( 0.0f, 0.0f, -1.0f );

	glTexCoord2f( 0.0f, 1.0f );
    glVertex3f( r,-r,-r);	// Top Right Of The Quad (Back)
	glTexCoord2f( 1.0f, 1.0f );
    glVertex3f(-r,-r,-r);	// Top Left Of The Quad (Back)
	glTexCoord2f( 1.0f, 0.0f );
    glVertex3f(-r, r,-r);	// Bottom Left Of The Quad (Back)
	glTexCoord2f( 0.0f, 0.0f );
    glVertex3f( r, r,-r);	// Bottom Right Of The Quad (Back)


	glNormal3f( -1.0f, 0.0f, 0.0f );

	glTexCoord2f( 0.0f, 0.0f );
    glVertex3f(-r, r, r);	// Top Right Of The Quad (Left)
	glTexCoord2f( 1.0f, 0.0f );
    glVertex3f(-r, r,-r);	// Top Left Of The Quad (Left)
	glTexCoord2f( 1.0f, 1.0f );
    glVertex3f(-r,-r,-r);	// Bottom Left Of The Quad (Left)
	glTexCoord2f( 0.0f, 1.0f );
    glVertex3f(-r,-r, r);	// Bottom Right Of The Quad (Left)

	glNormal3f( 1.0f, 0.0f, 0.0f );

	glTexCoord2f( 0.0f, 0.0f );
    glVertex3f( r, r,-r);	// Top Right Of The Quad (Right)
	glTexCoord2f( 1.0f, 0.0f );
    glVertex3f( r, r, r);	// Top Left Of The Quad (Right)
	glTexCoord2f( 1.0f, 1.0f );
    glVertex3f( r,-r, r);	// Bottom Left Of The Quad (Right)
	glTexCoord2f( 0.0f, 1.0f );
    glVertex3f( r,-r,-r);	// Bottom Right Of The Quad (Right)
  glEnd();			// End Drawing The Cube

  glDisable( GL_TEXTURE_2D );
}

void CBoss::DeathEffect()
{
	float x = mObject->GetPosition().x;
	float z = mObject->GetPosition().y;
	gEntities.Add( new CDeath( x, 2.5f, z, 0.0f, 1.0f, 0.0f, 5.0f ) );

	/*	Make 3 little Gremlings and put down the gem pickup	*/

	// Gremlings
	for ( int i = 0; i < 3; i++ )
	{
		CMonster* monster = new CGremling( 10.0f, 12.0f );
		monster->SetHealth( 3 );
		monster->SetRot( i * HALF_PI );
		SPoint pos( cos( i * HALF_PI ) * 3.0f,
			sin( i * HALF_PI ) * 3.0f );
		pos.x += x;
		pos.y += z;
		((IPhysicalObj*)monster)->CreatePhysical( gWorld, &pos, new SVector(), 0.8f, 5.0f, 5.0f, 0.01f, true, MONSTER_OBJ_TYPE );
		gEntities.Add( (CEntity*)monster );
	}

	// Gem
	if ( gNumBosses <= 1 )
	{
		SPoint pos( x, z );
		SVector vel( 0.0f, 0.0f );
		CPickup* pickup = new CPickup( GEM_PICKUP_TYPE );
		((IPhysicalObj*)pickup)->CreatePhysical( gWorld, &pos, &vel, 1.0f, 2.0f, 7.0f, 0.7f, false, PICKUP_OBJ_TYPE );
		gEntities.Add( (CEntity*)pickup );
	}
	
}
