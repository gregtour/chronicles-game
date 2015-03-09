#ifndef _TEXTURE_H
#define _TEXTURE_H

#include "../common/resource.h"
#include "graphics.h"
#include <string>

class CTexture
{
public:
	CTexture();
	CTexture( std::string file );
	~CTexture();

	void Load( std::string file );
	void CreateBlank( int width, int height );
	void Unload();
	void BindTexture();

	bool IsLoaded();

protected:
	bool mLoaded;
	GLuint mTextureId;
};

class CManagedTexture: public CManagedResource
{
public:
	CManagedTexture( std::string file );
	~CManagedTexture(); 

	static CTexture* Load( CResourceManager* m, std::string file );
	static bool Unload( CResourceManager* m, CTexture* t );

	CTexture* GetTexture();

protected:
	bool LoadResource( std::string file );
	void UnloadResource();

	CTexture mData;
};



#endif
