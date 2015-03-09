#include "particle.h"
#include "graphics.h"
#include "../engine.h"

#define NULL	0

/*	Particle Methods	*/
CParticle::CParticle( std::string texture, float life, float x, float y, float z, float startSize, float endSize )
{
	mTexture = CManagedTexture::Load( &gResourceManager, texture );
	mLife = life;
	mAge = 0.0f;
	mX = x;
	mY = y;
	mZ = z;
	mStartSize = startSize;
	mEndSize = endSize;
	SetStartColor( 1.0f, 1.0f, 1.0f, 1.0f );
	SetEndColor( 1.0f, 1.0f, 1.0f, 0.0f );
	SetSpeed( 0.0f, 0.0f, 0.0f );
}

CParticle::~CParticle()
{
	CManagedTexture::Unload( &gResourceManager, mTexture );
}

void CParticle::GetSpeed( float* xSpeed, float* ySpeed, float* zSpeed )
{
	*xSpeed = mXSpeed;
	*ySpeed = mYSpeed;
	*zSpeed = mZSpeed;
}

void CParticle::SetStartColor( float startR, float startG, float startB, float startA )
{
	mStartR = startR;
	mStartG = startG;
	mStartB = startB;
	mStartA = startA;
}


void CParticle::SetEndColor( float r, float g, float b, float a )
{
	mEndR = r;
	mEndG = g;
	mEndB = b;
	mEndA = a;
}


void CParticle::SetSpeed( float xSpeed, float ySpeed, float zSpeed )
{
	mXSpeed = xSpeed;
	mYSpeed = ySpeed;
	mZSpeed = zSpeed;
}


void CParticle::Update( float dt )
{
	if ( IsAlive() )
	{
		mAge += dt;
		mX += mXSpeed * dt;
		mY += mYSpeed * dt;
		mZ += mZSpeed * dt;
	}
}


void CParticle::RenderAlpha()
{
	if ( IsAlive() )
	{

		glPushMatrix();

		glTranslatef( mX, mY, mZ );

		SPoint cameraPos;
		float y;
		gCamera->GetPosition( &cameraPos.x, &y, &cameraPos.y );

		SVector billboardAngle;
		SPoint position( mX, mZ );
		position.Difference( &cameraPos, &billboardAngle );
		glRotatef( atan( &billboardAngle ) * DEGREES + 90.0f, 0.0f, -1.0f, 0.0f );

		float r = (mStartR + (mEndR - mStartR) * (mAge / mLife) );
		float g = (mStartG + (mEndG - mStartG) * (mAge / mLife) );
		float b = (mStartB + (mEndB - mStartB) * (mAge / mLife) );
		float a = (mStartA + (mEndA - mStartA) * (mAge / mLife) );
		float size = (mStartSize + (mEndSize - mStartSize) * (mAge / mLife) );

		mTexture->BindTexture();
		glColor4f( r, g, b, a );

		glBegin( GL_QUADS );
			glTexCoord2f( 1.0f, 0.0f );
			glVertex3f( -size, -size, 0.0f );
			glTexCoord2f( 0.01f, 0.0f );
			glVertex3f( -size, size, 0.0f );
			glTexCoord2f( 0.01f, 1.0f );
			glVertex3f( size, size, 0.0f );
			glTexCoord2f( 1.0f, 1.0f );
			glVertex3f( size, -size, 0.0f );
		glEnd();

		glPopMatrix();
	}
}

bool CParticle::IsAlive()
{ return ( mAge < mLife ); }


void CParticle::Accelerate( float xSpeed, float ySpeed, float zSpeed )
{
	mXSpeed += xSpeed;
	mYSpeed += ySpeed;
	mZSpeed += zSpeed;
}





/*	Particle Emitter Methods	*/

CParticleEmitter::CParticleEmitter()
{
	mTimer = 0.0f;
	mEmissionRate = 0.0001f;
	mParticlesLeft = -1;
}

CParticleEmitter::CParticleEmitter( float rate, int particles )
{
	mTimer = 0.0f;
	SetEmissionRate( rate );
	SetParticlesLeft( particles );
}

CParticleEmitter::~CParticleEmitter()
{
	/*	Delete Remaining Particles	*/
	CListEntry<CParticle>* particle = mParticles.GetFirst();
	while ( particle )
	{
		delete particle->data;
		mParticles.RemoveEntry( particle );
		particle = mParticles.GetFirst();
	}
}

void CParticleEmitter::SetEmissionRate( float rate )
{ mEmissionRate = rate; }


void CParticleEmitter::SetParticlesLeft( int particles )
{ mParticlesLeft = particles; }

int CParticleEmitter::GetParticlesLeft()
{ return mParticlesLeft; }



void CParticleEmitter::SetPosition( float x, float y, float z )
{
	mX = x;
	mY = y;
	mZ = z;
}


void CParticleEmitter::Update( float dt )
{
	{
		float x, y, z;
		gCamera->GetPosition( &x, &y, &z );
		if (  ((x-mX)*(x-mX) + (z-mZ)*(z-mZ)) > VIEW_DISTANCE_SQUARED )
			return;
	}

	/*	Create New Particles	*/
	mTimer += dt;
	while ( mTimer > ( 1.0f / mEmissionRate ) && mParticlesLeft != 0 )
	{
		CreateParticle();
		mTimer -= ( 1.0f / mEmissionRate );
		if ( mParticlesLeft > 0 )
			mParticlesLeft--;
	}

	/*	Update Old Particles	*/
	CListEntry<CParticle>* particle = mParticles.GetFirst();
	while ( particle )
	{
		particle->data->Update( dt );
		EffectParticle( particle->data, dt );	//	Effects like gravity

		/*	Delete Dead Particles	*/
		if ( !particle->data->IsAlive() )
		{
			delete particle->data;
			CListEntry<CParticle>* nextParticle = particle->GetNext();
			mParticles.RemoveEntry( particle );
			particle = nextParticle;
		}
		else
			particle = particle->GetNext();
	}

	Logic( dt );

	/*	If there are no particles, and no particles left, delete this emitter	*/
	if ( mParticles.length == 0 && mParticlesLeft == 0 )
	{
		/*	Set the ListEntry for it to NULL so it will be removed automatically	*/
		gEntities.FindEntry( this )->data = NULL;
		delete this;
	}
}

void CParticleEmitter::Logic( float dt )
{
	return;
}

void CParticleEmitter::RenderAlpha()
{
	{
		float x, y, z;
		gCamera->GetPosition( &x, &y, &z );
		if (  ((x-mX)*(x-mX) + (z-mZ)*(z-mZ)) > VIEW_DISTANCE_SQUARED )
			return;
	}

	glDisable( GL_LIGHTING );
	glEnable( GL_BLEND );
	glEnable( GL_TEXTURE_2D );
	glDisable( GL_CULL_FACE );
//	glDisable( GL_DEPTH_TEST );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE );
//	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glDepthMask( GL_FALSE );

	CListEntry<CParticle>* particle = mParticles.GetFirst();
	while ( particle )
	{
		particle->data->RenderAlpha();
		particle = particle->GetNext();
	}

	glDisable( GL_TEXTURE_2D );
	glDisable( GL_BLEND );
	glEnable( GL_LIGHTING );
	glEnable( GL_CULL_FACE );
	glDepthMask( GL_TRUE );
//	glEnable( GL_DEPTH_TEST );
}
