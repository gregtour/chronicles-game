#ifndef _GATE_H
#define _GATE_H

#include "../../engine/entity/entity.h"
#include "../../engine/entity/graphicalObj.h"
#include "../../engine/entity/physicalObj.h"
#include "../../engine/graphics/texture.h"
#include "../../engine/engine.h"

#define GATE_OBJ_TYPE	13

class CGate: public CEntity, public IAlphaGraphicalObj, public IPhysicalObj
{
public:
	CGate();
	~CGate();

	void CreatePhysical( CPhysics* world, SPoint* p1, SPoint* p2 );
	void Update( float dt );
	void RenderAlpha();
	void Open();
	bool IsOpen();

private:
	CTexture* mTexture;
	CTexture* mMask;
	int mOpen;
	bool mOpening;
};

#endif
