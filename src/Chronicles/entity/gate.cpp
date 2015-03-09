#include "gate.h"
#include <math.h>
#include "../../engine/sound/wavesound.h"

CGate::CGate()
{
	mOpen = 0;
	mOpening = false;
	mTexture = CManagedTexture::Load( &gResourceManager, "data/gate.bmp" );
	mMask = CManagedTexture::Load( &gResourceManager, "data/gateMask.bmp" );
}

CGate::~CGate()
{
	CManagedTexture::Unload( &gResourceManager, mTexture );
	CManagedTexture::Unload( &gResourceManager, mMask );
}

void CGate::CreatePhysical( CPhysics* world, SPoint* p1, SPoint* p2 )
{
	SPoint pos;
	SVector vec;
	mObject = new CPLine( p1, p2, &pos, &vec, false, GATE_OBJ_TYPE );
	mObject->SetOwner( (IPhysicalObj*)this );

	mWorld = world;
	if ( mWorld )
		mWorld->Add( mObject );

}

void CGate::Open( )
{
	mOpen = 1;
	mOpening = true;
	CWaveSound* s = CManagedWaveSound::Load( &gResourceManager, "data/gate.wav" );
	s->Play( 3 );
}

bool CGate::IsOpen()
{
	return (mOpen != 0);
}

void CGate::Update( float dt )
{
	if ( mOpening )
	{
		SPoint p1 = ((CPLine*)mObject)->GetPoint1();
		SPoint p2 = ((CPLine*)mObject)->GetPoint2();

		p1.x += (p2.x - p1.x) * dt;
		p1.y += (p2.y - p1.y) * dt;

		((CPLine*)mObject)->SetPoints( &p1, &p2 );

		if ( fabs(p1.x - p2.x) + fabs(p1.y-p2.y) < 0.2f )
		{
			mOpen = 2;
		}
	}
}

void CGate::RenderAlpha()
{
	if ( mOpen != 2 )
	{
		glDisable( GL_CULL_FACE );
		glEnable( GL_BLEND );
		glEnable( GL_TEXTURE_2D );
		glDisable( GL_LIGHTING );
		glDisable( GL_FOG );

		SPoint p1 = ((CPLine*)mObject)->GetPoint1();
		SPoint p2 = ((CPLine*)mObject)->GetPoint2();

	//		mTexture->BindTexture();
		for ( int i = 0; i < 2; i++ )
		{

			if ( i == 0 )
			{
				glBlendFunc( GL_DST_COLOR, GL_ZERO );
				mMask->BindTexture();
			}
			else if ( i == 1 )
			{
				glBlendFunc( GL_ONE, GL_ONE );
				mTexture->BindTexture();
			}

			glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
			glBegin( GL_QUADS );
					glTexCoord2f( 0.0f, 0.01f );
				glVertex3f( p1.x, 8.0f, p1.y );
					glTexCoord2f( 0.0f, 1.0f );
				glVertex3f( p1.x, 0.0f, p1.y );
					glTexCoord2f( 1.0f, 1.0f );
				glVertex3f( p2.x, 0.0f, p2.y );
					glTexCoord2f( 1.0f, 0.01f );
				glVertex3f( p2.x, 8.0f, p2.y );
			glEnd();
		}

		glEnable( GL_CULL_FACE );
		glDisable( GL_BLEND );
		glDisable( GL_TEXTURE_2D );
		glEnable( GL_LIGHTING );
		glEnable( GL_FOG );
	}
}
