#include "../../engine/engine.h"
#include "bat.h"
#include "player.h"
#include "particles/death.h"
#include <math.h>

#define	SOUND_CUTOFF_DISTANCE	1500.0f
#define SOUND_REACH				10

CBat::CBat( float rotSpeed, float flapHeight, float flapSpeed, float flapIntensity )
{
	mEffect = NULL;

	mRotSpeed = rotSpeed;
	mAction = 0;

	mFlap = 0.0f;
	mFlapHeight = flapHeight;
	mFlapSpeed = flapSpeed;
	mFlapIntensity = flapIntensity;

	mY = BAT_ALTITUDE;
	
	mSound = NULL;
	mBatSound = CManagedWaveSound::Load( &gResourceManager, "data/bat.wav" );

	LoadTexture();
}

CBat::~CBat()
{
	CManagedWaveSound::Unload( &gResourceManager, mBatSound );
	if ( mSound )
	{
		if ( mSound->IsPlaying() )
			mSound->Stop();
		delete mSound;
	}

	if ( gEntities.FindEntry( mEffect ) )
	{
		if ( mEffect->GetParticlesLeft() < 0 )
			mEffect->SetParticlesLeft( 0 );
	}
}

void CBat::LoadTexture()
{
#ifdef LOW_RESOLUTION_TEXTURES
	mTexture = CManagedTexture::Load( &gResourceManager, "data/low/bat.bmp" );
#else
	mTexture = CManagedTexture::Load( &gResourceManager, "data/bat.bmp" );
#endif
}


void CBat::Update( float dt )
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
				mSound = new CSound( mBatSound->Play( -1 ) );
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



	if ( mEffect == NULL )
	{
		mEffect = new CFire( 100.0f, 0.0f, 80.0f );
		gEntities.Add( (CEntity*)mEffect );
//		mEffect->SetEmissionRate( 15.0f );
	}
	SPoint p = mObject->GetPosition();
	mEffect->SetPosition( p.x, mY, p.y );

	if ( gPlayer )
	{

		if ( mAction != 1 )	// Flap if Normal or Climbing, Not Diving
		{
			/*	Flap Wings	*/
			mFlap += dt * mFlapSpeed;
			if ( mAction == 2 && mY < BAT_ALTITUDE && mFlap < mFlapHeight )
					mY += dt * mFlapSpeed;

			if ( mFlap > mFlapHeight )
			{
				mFlap += dt * mFlapIntensity * mFlapSpeed;

				if ( mFlap > mFlapHeight * 2 )
				{
					mFlap -= mFlapHeight * 2;
				}
			}
		}

		/*	Manage Altitude	*/
		if ( mAction == 1 )
		{
			if ( mY > PLAYER_HEIGHT - 2.0f )
			{
				mY += ( -BAT_ALTITUDE - PLAYER_HEIGHT - 2.0f ) * dt / 2.0f;
			}
		}

		/*	Fly Around Player	*/
		SVector distance;
		gPlayer->GetPhysicalObject()->GetPosition().Difference( &mObject->GetPosition(), &distance );
		float distanceToPlayer = distance.LengthSquared();

		/*	Distance	*/
		if ( mAction == 0 && distanceToPlayer < BAT_DIVE_DISTANCE * BAT_DIVE_DISTANCE )
			mAction = 1;
		if ( mAction == 1 && distanceToPlayer < 1.2f*1.2f && mY <= PLAYER_HEIGHT )
			mAction = 2;
		if ( mAction == 2 && mY >= BAT_ALTITUDE )
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

void CBat::RenderAlpha()
{
	{
		float mX = mObject->GetPosition().x;
		float mZ = mObject->GetPosition().y;
		float x, y, z;
		gCamera->GetPosition( &x, &y, &z );
		if (  ((x-mX)*(x-mX) + (z-mZ)*(z-mZ)) > VIEW_DISTANCE_SQUARED )
			return;
	}

	/*	Shadow		*/
	float r = ((CPCircle*)mObject)->GetRadius() * 0.9f;
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable( GL_BLEND );

	glTranslatef( mObject->GetPosition().x, 0.0f, mObject->GetPosition().y );
	glRotatef( (GLfloat)(mRot * DEGREES), 0.0f, -1.0f, 0.0f );

	glBegin( GL_QUADS );
		glColor4f( 0.0f, 0.0f, 0.0f, 0.5f );

		glVertex3f( -r, 0.01f,  r );
		glVertex3f(  r, 0.01f,  r );
		glVertex3f(  r, 0.01f, -r );
		glVertex3f( -r, 0.01f, -r );
	glEnd();

	glDisable( GL_BLEND );
}

void CBat::Render()
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
	glTranslatef( mObject->GetPosition().x, 0.0f, mObject->GetPosition().y );
	glRotatef( (GLfloat)(mRot * DEGREES), 0.0f, -1.0f, 0.0f );

	/*		Model		*/
	if ( mFlap > mFlapHeight )
		glTranslatef( 0.0f, mY + mFlap - mFlapHeight/2, 0.0f );
	else
		glTranslatef( 0.0f, mY + mFlapHeight*1.5f - mFlap, 0.0f );

	glEnable( GL_TEXTURE_2D );
	mTexture->BindTexture();
	
	/*	Temporary Code, Draw A Cube as a Placeholder	*/
  glBegin(GL_QUADS);		// Draw The Cube Using quads

    glColor3f( 0.7f, 0.7f, 0.7f );
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

void CBat::DeathEffect()
{
//	float x = mObject->GetPosition().x;
//	float z = mObject->GetPosition().y;
//	gEntities.Add( new CDeath( x, mY, z, 1.0f, 0.0f, 0.0f, 1.0f ) );
	if ( mEffect )
	{
		mEffect->SetParticlesLeft( 100 );
		mEffect->SetEmissionRate( 200.0f );
		mEffect->SetGravity( true );
	}
}
