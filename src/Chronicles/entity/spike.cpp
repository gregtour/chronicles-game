#include "spike.h"
#include <math.h>

CSpike::CSpike( float frequency )
{
	mUp = false;
	mY = 0.0f;
	mTime.Clear();
	mTime.Step();
	mFrequency = frequency;
	mSound = CManagedWaveSound::Load( &gResourceManager, "data/spike.wav" );

	mQuadric = gluNewQuadric();
	gluQuadricNormals( mQuadric, GLU_SMOOTH );
	gluQuadricTexture( mQuadric, GL_TRUE );

}

CSpike::~CSpike()
{
	gluDeleteQuadric( mQuadric );
	CManagedWaveSound::Unload( &gResourceManager, mSound );
}

void CSpike::LoadTexture()
{
	mTexture = NULL;
}


void CSpike::CreatePhysical( CPhysics* world, SPoint* p1, SPoint* p2 )
{
	SPoint pos;
	SVector vec;

	pos.Add( p1 );
	pos.Add( p2 );
	pos.x /= 2;
	pos.y /= 2;
	
	SPoint pos1( p1->x, p1->y );
	SPoint pos2( p2->x, p2->y );
	pos1.x -= pos.x;
	pos2.x -= pos.x;
	pos1.y -= pos.y;
	pos2.y -= pos.y;


	mObject = new CPLine( &pos1, &pos2, &pos, &vec, false, SPIKE_OBJ_TYPE );
	mObject->SetOwner( (IPhysicalObj*)this );

	mWorld = world;
	if ( mWorld )
		mWorld->Add( mObject );
}

void CSpike::Update( float dt )
{
	if ( mTime.Time() > mFrequency )
	{
		mTime.Step();
		mUp = !mUp;

		if ( mUp )
		{
			mY = -3.0f;

			float mX = mObject->GetPosition().x;
			float mZ = mObject->GetPosition().y;
			float x, y, z;
			gCamera->GetPosition( &x, &y, &z );
			if (  ((x-mX)*(x-mX) + (z-mZ)*(z-mZ)) <= 2000.0f )
				mSound->Play( 0 );
		}
	}

}

void CSpike::Render()
{
	if ( mUp )
	{
		glDisable( GL_LIGHTING );
		glColor3f( 1.0f, 1.0f, 1.0f );

		CPLine* line = ((CPLine*)mObject);
		SPoint p1 = line->GetPoint1();
		p1.Add( &line->GetPosition() );
		SPoint p2 = line->GetPoint2();
		p2.Add( &line->GetPosition() );

		SVector v;
		p2.Difference( &p1, &v );
		v.Scale( 0.25f );

		float height = fabs( (mTime.Time() - mFrequency / 2.0f) / (mFrequency / 2.0f) );
		height = 1.0f - height;

		glPushMatrix();
		for ( int i = 0; i < 4; i++ )
		{
			glPopMatrix();
			glPushMatrix();
			glTranslatef( p1.x, 0.0f, p1.y );
			glTranslatef( v.x / 2.0f, 0.0f, v.y / 2.0f );
			for ( int z = 0; z < i; z++ )
				glTranslatef( v.x, 0.0f, v.y );
			glRotatef( 90.0f, -1.0f, 0.0f, 0.0f );
			gluCylinder( mQuadric, 1.0f * height, 0.0f, 3.0f * height, 10, 4 );
		}
		glPopMatrix();

		glEnable( GL_LIGHTING );
	}
}

bool CSpike::IsUp()
{
	float height = fabs( (mTime.Time() - mFrequency / 2.0f) / (mFrequency / 2.0f) );
	height = 1.0f - height;
	return mUp && height > 0.2f;
}

