#include "../../engine/graphics/graphics.h"
#include "../../engine/sound/wavesound.h"
#include "../../engine/graphics/texture.h"
#include "../../engine/common/timer.h"

void RenderSplash( float time, CTexture* image );

void SplashScreen()
{
	gLog.LogItem( new CLogMessage("Splash Screen") );

	CGameTimer timer;
	timer.Clear();
	timer.Step();

	CTexture* splashImage = new CTexture( "data/title.bmp" );
	CWaveSound* splashSound = new CWaveSound( "data/opening.wav" );
	CSound* sound = new CSound( splashSound->Play( 0 ) );

	glEnable( GL_TEXTURE_2D );
	glDisable( GL_DEPTH_TEST );

	while ( sound->IsPlaying() )
	{
		RenderSplash( timer.Time(), splashImage );
	}

	glEnable( GL_DEPTH_TEST );
	glDisable( GL_TEXTURE_2D );

	delete sound;
	delete splashImage;
	delete splashSound;
}

void RenderSplash( float time, CTexture* image )
{
	glMatrixMode (GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D( 0.0, 1.0, 1.0, 0.0 );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	image->BindTexture();

	glBegin( GL_QUADS );
		if ( time > 4.0f )
		{
			float bright = (6.0f - time)/2.0f;
			glColor4f( bright, bright, bright, 1.0f );		
		} 
		else if ( time < 1.3f ) 
		{
			float bright = (time-0.3f);
			glColor4f( bright, bright, bright, 1.0f );		
		}

		glTexCoord2f( 0.0f, 0.0f );
		glVertex2f( 0.0f,  0.0f );
		glTexCoord2f( 0.0f, 1.0f );
		glVertex2f( 0.0f,  1.0f );
		glTexCoord2f( 1.0f, 1.0f );
		glVertex2f( 1.0f,  1.0f );
		glTexCoord2f( 1.0f, 0.0f );
		glVertex2f( 1.0f,  0.0f );
	glEnd();


	SDL_GL_SwapBuffers();
}
