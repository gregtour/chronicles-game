#ifndef _SPIKE_H
#define _SPIKE_H

#include "monster.h"

#define SPIKE_OBJ_TYPE			14

class CSpike: public CMonster
{
public:
	CSpike( float frequency );
	~CSpike();

	void CreatePhysical( CPhysics* world, SPoint* p1, SPoint* p2 );
	void Update( float dt );
	void Render();
	bool IsUp();

	void LoadTexture();

private:
	CWaveSound* mSound;
	float mFrequency;
	CTimer mTime;
	bool mUp;

	GLUquadricObj* mQuadric;
};

#endif
