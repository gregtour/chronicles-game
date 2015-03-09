#include "../game/game.h"
#include "../main.h"
#include "character.h"
#include <string>
#include <sstream>

CCharacter::CCharacter()
{
	LoadHitSound();
	mHealth = 1;
	mRot = 0.0f;
	mLastHit.Clear();
}

CCharacter::~CCharacter()
{
	CManagedWaveSound::Unload( &gResourceManager, mHitSound );
}

void CCharacter::LoadHitSound()
{
	mHitSound = CManagedWaveSound::Load( &gResourceManager, "data/grunt.wav" );
}

void CCharacter::Update( float dt )
{

};

void CCharacter::Render()
{

};

void CCharacter::Collision( IPhysicalObj* o )
{

};

void CCharacter::LogState()
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

int CCharacter::GetHealth()
{
	return mHealth;
}

void CCharacter::SetHealth( int h )
{
	mHealth = h;
}

void CCharacter::Hit( CCharacter* byWhom, int damage )
{
	if ( mLastHit.Time() > MIN_HIT_TIME )
	{
		if ( byWhom == gPlayer )
			gPlayerScore += HIT_ENEMY_POINTS;
		mHitSound->Play( 0 );

		mLastHit.Step();

		SVector v;
		mObject->GetPosition().Difference( &((IPhysicalObj*)byWhom)->GetPhysicalObject()->GetPosition(), &v );
		v.Normalize();
		v.Scale( damage * 20.0f );
	//	mObject->Translate( v.x, v.y );
		mObject->SetVelocity( &v );

		mHealth -= damage;
		if ( mHealth <= 0 )
		{
			if ( byWhom == gPlayer )
				gPlayerScore += KILLED_ENEMY_POINTS;
			Die();
		}
	}
}

void CCharacter::Die()
{
	DeathEffect();

	/*	Remove it from the game	*/
	gEntities.RemoveEntry( gEntities.FindEntry( this ) );
	/*	Delete it	*/
	delete this;
}

void CCharacter::DeathEffect()
{

}

