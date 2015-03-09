#include "ghoul.h"


CGhoul::CGhoul( float min )
{
	mMinDistanceToPlayer = min;
	mChasing = true;
	mTexture = CManagedTexture::Load( &gResourceManager, "data/ghoul1.bmp" );
	mMask = CManagedTexture::Load( &gResourceManager, "data/ghoulMask.bmp" );
	mOverTex = CManagedTexture::Load( &gResourceManager, "data/ghoul.bmp" );
}

CGhoul::~CGhoul()
{
	CManagedTexture::Unload( &gResourceManager, mTexture );
	CManagedTexture::Unload( &gResourceManager, mMask );
	CManagedTexture::Unload( &gResourceManager, mOverTex );
}

void CGhoul::LoadTexture()
{

}

void CGhoul::DeathEffect()
{

}

void CGhoul::Update( float dt )
{

	if ( gPlayer )
	{
		if ( mChasing )
		{
			SPoint player = gPlayer->GetPhysicalObject()->GetPosition();
			SVector v;
			mObject->GetPosition().Difference( &player, &v );
			v.Normalize();
			v.Scale( -GHOUL_SPEED * dt );
			mObject->ApplyForce( &v );
		} else {


		}
	}
}

void CGhoul::Render()
{
}

void CGhoul::RenderAlpha()
{

	SPoint position = mObject->GetPosition();
	glTranslatef( position.x, 0.0f, position.y );

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

	for ( int i = 0; i < 3; i++ )
	{
		if ( i == 0 )
		{
			glBlendFunc( GL_DST_COLOR, GL_ZERO );
			mMask->BindTexture();
		} 
		else if ( i == 1 )
		{
			glBlendFunc( GL_ONE_MINUS_SRC_ALPHA, GL_ONE );	
			mTexture->BindTexture();
		}
		else
		{
			glBlendFunc( GL_SRC_ALPHA, GL_ONE );	
			mOverTex->BindTexture();
		}

		glBegin( GL_QUADS );
			glColor4f( 1.0f, 1.0f, 1.0f, 0.4f );
			
			glTexCoord2f( 0.0f, 0.0f );
			glVertex3f( -4.0f, 8.0f, 0.0f );

			glTexCoord2f( 0.0f, 1.0f );
			glVertex3f( -4.3f, 0.0f, 0.0f );

			glTexCoord2f( 1.0f, 1.0f );
			glVertex3f( 4.3f, 0.0f, 0.0f );

			glTexCoord2f( 1.0f, 0.0f );
			glVertex3f( 4.0f, 8.0f, 0.0f );
		glEnd();
	}

	glDisable( GL_TEXTURE_2D );
	glDisable( GL_BLEND );
	glEnable( GL_LIGHTING );
	glEnable( GL_CULL_FACE );
	glDepthMask( GL_TRUE );
	glEnable( GL_FOG );
}
