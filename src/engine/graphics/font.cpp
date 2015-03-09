#include "font.h"
#include "graphics.h"

CFontRenderer::CFontRenderer( std::string fontTexture, std::string fontMask, int width, std::string fontText )
{
	mFontText = fontText;
	mFontWidth = width;
	mFontHeight = (int)((float)mFontText.length() / width - 0.001f) + 1;
	mFont = CManagedTexture::Load( &gResourceManager, fontTexture );
	mFontMask = CManagedTexture::Load( &gResourceManager, fontMask );
}

CFontRenderer::~CFontRenderer()
{
	CManagedTexture::Unload( &gResourceManager, mFont );
	CManagedTexture::Unload( &gResourceManager, mFontMask );
}

void CFontRenderer::RenderString( std::string text, float x, float y, float width, float height, float spacing )
{
	glDisable( GL_LIGHTING );
	glEnable( GL_BLEND );
	glDisable( GL_DEPTH_TEST );
	glEnable( GL_TEXTURE_2D );

	const char* ctext = text.c_str();
	for ( int i = 0; i < (int)text.length(); i++ )
	{
		RenderCharacter( ctext[i], x + (spacing*i), y, width, height );
	}

	glDisable( GL_TEXTURE_2D );
	glDisable( GL_BLEND );
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_LIGHTING );
}

void CFontRenderer::RenderCharacter( char c, float x, float y, float width, float height )
{
	int index = mFontText.find( c, 0 );
	if ( index == std::string::npos )
		return;

	int row = index / mFontWidth;
	int col = index % mFontWidth;

	/*	Render 2 Passes, 1st the Mask and 2nd the Image	*/
	for ( int i = 0; i < 2; i++ )
	{
		if ( i == 0 )
		{
			glBlendFunc( GL_DST_COLOR, GL_ZERO );
			mFontMask->BindTexture();
		} 
		else 
		{
			glBlendFunc( GL_ONE, GL_ONE );	
			mFont->BindTexture();
		}

		glBegin( GL_QUADS );
			glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
			
			glTexCoord2f( (float)(col) / mFontWidth, (float)(row + 1) / mFontHeight );
			glVertex2f( x, y + height );

			glTexCoord2f( (float)(col + 1) / mFontWidth, (float)(row + 1) / mFontHeight );
			glVertex2f( x + width, y + height );

			glTexCoord2f( (float)(col + 1) / mFontWidth, (float)(row) / mFontHeight );
			glVertex2f( x + width, y );

			glTexCoord2f( (float)(col) / mFontWidth, (float)(row) / mFontHeight );
			glVertex2f( x, y );
		glEnd();
	}
}

