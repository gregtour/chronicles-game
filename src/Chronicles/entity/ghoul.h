#ifndef _GHOUL_H
#define _GHOUL_H

#include "monster.h"

#define GHOUL_OBJ_TYPE	11
#define GHOUL_SPEED		17.0f

class CGhoul: public CMonster, public IAlphaGraphicalObj
{
public:
	CGhoul( float min );
	~CGhoul();

	void LoadTexture();

	void DeathEffect();

	void Update( float dt );
	void Render();
	void RenderAlpha();

protected:
	bool mChasing;
	float mMinDistanceToPlayer;
	CTexture* mMask;
	CTexture* mOverTex;
};


#endif
