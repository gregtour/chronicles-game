#ifndef _CAMERA_H
#define _CAMERA_H

#include "entity.h"

class CCamera : public CEntity
{
public:
	CCamera();
	~CCamera();

	void SetPosition( float x, float y, float z );
	void LookAt( float x, float y, float z );
	void SetUp( float x, float y, float z );

	void GetPosition( float* x, float* y, float* z );

	void Update( float );
	void Render();

protected:
	float mX, mY, mZ;
	float mLookX, mLookY, mLookZ;
	float mUpX, mUpY, mUpZ;
};

#endif
