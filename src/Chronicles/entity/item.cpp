#include "item.h"
#include "../../engine/engine.h"
#include "../main.h"
#include <string>
#include <sstream>
#include "particles/spark.h"

CItem::CItem()
{
#ifdef LOW_RESOLUTION_TEXTURES
	mTexture = CManagedTexture::Load( &gResourceManager, "data/low/crate.bmp" );
#else
	mTexture = CManagedTexture::Load( &gResourceManager, "data/crate.bmp" );
#endif
}

CItem::~CItem()
{
	CManagedTexture::Unload( &gResourceManager, mTexture );
}

void CItem::Update( float dt )
{

};

void CItem::Render()
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

	glEnable( GL_TEXTURE_2D );
	mTexture->BindTexture();


	float r = ((CPCircle*)mObject)->GetRadius() * 0.9f;
	glTranslatef( mObject->GetPosition().x, r, mObject->GetPosition().y );
	
	/*	Temporary Code, Draw A Cube as a Placeholder	*/
  glBegin(GL_QUADS);		// Draw The Cube Using quads

    glColor3f( 0.7f, 0.7f, 0.7f );
	glNormal3f( 0.0f, 1.0f, 0.0f );

	glTexCoord2f( 0.0f, 0.0f );
    glVertex3f( r, r,-r);	// Top Right Of The Quad (Top)
	glTexCoord2f( 1.0f, 0.0f );
    glVertex3f(-r, r,-r);	// Top Left Of The Quad (Top)
	glTexCoord2f( 1.0f, 1.0f );
    glVertex3f(-r, r, r);	// Bottom Left Of The Quad (Top)
	glTexCoord2f( 0.0f, 1.0f );
    glVertex3f( r, r, r);	// Bottom Right Of The Quad (Top)

//    glColor3f(1.0f,0.5f,0.0f);	// Color Orange
	glNormal3f( 0.0f, -1.0f, 0.0f );

	glTexCoord2f( 0.0f, 0.0f );
    glVertex3f( r,-r, r);	// Top Right Of The Quad (Bottom)
	glTexCoord2f( 1.0f, 0.0f );
    glVertex3f(-r,-r, r);	// Top Left Of The Quad (Bottom)
	glTexCoord2f( 1.0f, 1.0f );
    glVertex3f(-r,-r,-r);	// Bottom Left Of The Quad (Bottom)
	glTexCoord2f( 0.0f, 1.0f );
    glVertex3f( r,-r,-r);	// Bottom Right Of The Quad (Bottom)

//    glColor3f(1.0f,0.0f,0.0f);	// Color Red	
	glNormal3f( 0.0f, 0.0f, 1.0f );

	glTexCoord2f( 0.0f, 0.0f );
    glVertex3f( r, r, r);	// Top Right Of The Quad (Front)
	glTexCoord2f( 1.0f, 0.0f );
    glVertex3f(-r, r, r);	// Top Left Of The Quad (Front)
	glTexCoord2f( 1.0f, 1.0f );
    glVertex3f(-r,-r, r);	// Bottom Left Of The Quad (Front)
	glTexCoord2f( 0.0f, 1.0f );
    glVertex3f( r,-r, r);	// Bottom Right Of The Quad (Front)

//    glColor3f(1.0f,1.0f,0.0f);	// Color Yellow
	glNormal3f( 0.0f, 0.0f, -1.0f );

	glTexCoord2f( 0.0f, 0.0f );
    glVertex3f( r,-r,-r);	// Top Right Of The Quad (Back)
	glTexCoord2f( 1.0f, 0.0f );
    glVertex3f(-r,-r,-r);	// Top Left Of The Quad (Back)
	glTexCoord2f( 1.0f, 1.0f );
    glVertex3f(-r, r,-r);	// Bottom Left Of The Quad (Back)
	glTexCoord2f( 0.0f, 1.0f );
    glVertex3f( r, r,-r);	// Bottom Right Of The Quad (Back)

///    glColor3f(0.0f,0.0f,1.0f);	// Color Blue
	glNormal3f( -1.0f, 0.0f, 0.0f );

	glTexCoord2f( 0.0f, 0.0f );
    glVertex3f(-r, r, r);	// Top Right Of The Quad (Left)
	glTexCoord2f( 1.0f, 0.0f );
    glVertex3f(-r, r,-r);	// Top Left Of The Quad (Left)
	glTexCoord2f( 1.0f, 1.0f );
    glVertex3f(-r,-r,-r);	// Bottom Left Of The Quad (Left)
	glTexCoord2f( 0.0f, 1.0f );
    glVertex3f(-r,-r, r);	// Bottom Right Of The Quad (Left)

//    glColor3f(1.0f,0.0f,1.0f);	// Color Violet
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
};

void CItem::Hit( IPhysicalObj* by, float force )
{
	
	SVector v;
	mObject->GetPosition().Difference( &by->GetPhysicalObject()->GetPosition(), &v );

	SPoint pointOfContact = mObject->GetPosition();
	v.Scale( -0.5f );
	pointOfContact.Translate( &v );
	gEntities.Add( new CSpark( pointOfContact.x, 3.0f, pointOfContact.y,
		1.0f, 1.0f, 1.0f, 2.0f, 1, 4.4f, 15.0f, "data/wood.bmp" ) );

	v.Flip();

	v.Normalize();
	v.Scale( force * by->GetPhysicalObject()->GetMass() / mObject->GetMass() );
	mObject->SetVelocity( &v );
}

void CItem::LogState()
{
	std::string state;

	if ( mObject )
	{
		std::stringstream stream;
		stream << "X: " << mObject->GetPosition().x << " Y: " << mObject->GetPosition().y << " XSP: " <<
			mObject->GetVelocity().x << " YSP " << mObject->GetVelocity().y;
		state = stream.str();
	} else {
		state = "No Physical Existance";
	}

	gLog.LogItem( new CLogMessage( state ) );
}
