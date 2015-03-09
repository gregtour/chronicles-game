#include "texture.h"
#include "../common/log.h"
#include "../engine.h"


CTexture::CTexture()
{
	mLoaded = false;
}

CTexture::CTexture( std::string file )
{
	mLoaded = false;
	Load( file );
}

CTexture::~CTexture()
{
	Unload();
}

void CTexture::Load( std::string file )
{
	SDL_Surface* temp = SDL_LoadBMP( file.c_str() );

	if ( !temp )// Failed to Load
	{
		std::string message = "LoadBMP failed on file " + file;
		gLog.LogItem( new CLogMessage(message) );
		mLoaded = false;
		return;
	}

	glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );

	glGenTextures( 1, &mTextureId );
	glBindTexture( GL_TEXTURE_2D, mTextureId );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

	gluBuild2DMipmaps( GL_TEXTURE_2D, 3, temp->w, temp->h, GL_BGR_EXT,
		GL_UNSIGNED_BYTE, temp->pixels );

	SDL_FreeSurface( temp );
	mLoaded = true;
}

void CTexture::CreateBlank( int width, int height )
{
	char* temp = new char[width * height * 4];

	glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );

	glGenTextures( 1, &mTextureId );
	glBindTexture( GL_TEXTURE_2D, mTextureId );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

	gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height, GL_BGR_EXT,
		GL_UNSIGNED_BYTE, (void*)temp );

	delete[] temp;
}

void CTexture::Unload()
{
	if ( mLoaded )
	{
		glDeleteTextures( 1, &mTextureId );
		mLoaded = false;
	}
}

void CTexture::BindTexture()
{
	glBindTexture( GL_TEXTURE_2D, mTextureId );
}

bool CTexture::IsLoaded()
{
	return mLoaded;
}


CManagedTexture::CManagedTexture( std::string file )
{
	mResource = RESOURCE_TEXTURE;
	mReferences = 0;
	mLoaded = false;
	LoadResource( file );
}


CManagedTexture::~CManagedTexture()
{
	UnloadResource();
}

CTexture* CManagedTexture::Load( CResourceManager* m, std::string file )
{
	CManagedTexture* r = (CManagedTexture*)(m->FindManagedResource( file ));
	if ( !r )
	{
		r = new CManagedTexture( file );
		m->AddResource( r );
	}

	r->IncrementReferenceCount();
	return r->GetTexture();
}

bool CManagedTexture::Unload( CResourceManager* m, CTexture* t )
{
	CLinkedList<CManagedResource>* list = m->GetList();
	CListEntry<CManagedResource>* entry = list->GetFirst();
	while ( entry )
	{
		if ( entry->data->ResourceType() == RESOURCE_TEXTURE )
		{
			if ( ((CManagedTexture*)entry->data)->GetTexture() == t )
			{
				entry->data->Unload();
				return true;
			}
		}
		entry = entry->GetNext();
	}

	/*	Failed to find CTexture* t in CResourceManager* m!	*/
	return false;
}

CTexture* CManagedTexture::GetTexture()
{
	return &mData;
}

bool CManagedTexture::LoadResource( std::string file )
{
	mData.Load( file );
	mFile = file;
	mLoaded = mData.IsLoaded();
	return mLoaded;
}

void CManagedTexture::UnloadResource()
{
	mData.Unload();
	mLoaded = false;
}
