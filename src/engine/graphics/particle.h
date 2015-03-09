#ifndef _PARTICLE_H
#define _PARTICLE_H

#include "../common/list.h"
#include "../entity/entity.h"
#include "../entity/graphicalObj.h"
#include "texture.h"

class CParticle: public CEntity, public IAlphaGraphicalObj
{
public:
	CParticle( std::string texture, float life, float x, float y, float z, float startSize, float endSize );
	~CParticle();

	void SetStartColor( float startR, float startG, float startB, float startA );
	void SetEndColor( float r, float g, float b, float a );
	void SetSpeed( float xSpeed, float ySpeed, float zSpeed );
	void GetSpeed( float* xSpeed, float* ySpeed, float* zSpeed );

	void Update( float dt );
	void RenderAlpha();
	bool IsAlive();
	void Accelerate( float xSpeed, float ySpeed, float zSpeed );

private:
	CTexture* mTexture;
	float mLife;
	float mAge;
	float mStartR, mStartG, mStartB, mStartA, mStartSize;
	float mEndR, mEndG, mEndB, mEndA, mEndSize;
	float mX, mY, mZ;
	float mXSpeed, mYSpeed, mZSpeed;
};



/* Virtual Class CParticleEmitter	*/
class CParticleEmitter: public CEntity, public IAlphaGraphicalObj
{
public:
	CParticleEmitter();
	CParticleEmitter( float rate, int particles );
	virtual ~CParticleEmitter();

	void SetEmissionRate( float rate );
	void SetParticlesLeft( int particles );
	void SetPosition( float x, float y, float z );

	int GetParticlesLeft();

	virtual void CreateParticle()=0;
	virtual void EffectParticle( CParticle* p, float dt )=0;
	virtual void Logic( float dt );
	virtual void Update( float dt );
	void RenderAlpha();
protected:
	int mParticlesLeft;		// Total # of particles to emmit over time, < 0 means infinite
	float mEmissionRate;	// Particles per Second
	float mTimer;
	float mX, mY, mZ;
	CLinkedList<CParticle> mParticles;
};


#endif
