
#include "physicalObj.h"
#include "../engine.h"
/*
int PhysicalObjectCollision( CPObject* a, CPObject* b )
{
	IPhysicalObj* object1 = (IPhysicalObj*)a->GetOwner();
	IPhysicalObj* object2 = (IPhysicalObj*)b->GetOwner();

	if ( object1 && object2 )
	{
		object1->Collision( object2 );
		object2->Collision( object1 );
	}

	return 1;
}
*/


IPhysicalObj::IPhysicalObj()
{
	mObject = NULL;
	mWorld = NULL;
	mRot = 0.0f;
	mY = 0.0f;
}

IPhysicalObj::~IPhysicalObj()
{
	if ( mObject )
	{
		SetWorld( NULL );
		delete mObject;
	}
}

void IPhysicalObj::CreatePhysical( CPhysics* world, SPoint* position, SVector* velocity, float radius, float mass, float friction, float bounce, bool dynamic, int objType )
{
	mObject = new CPCircle( position, velocity, radius, dynamic, objType );
	mObject->SetMass( mass );
	mObject->SetFriction( friction );
	mObject->SetOwner( (void*)this );
	if ( bounce == 0.0f )
		mObject->SetBounce( false );
	else
		mObject->SetBounceEfficiency( bounce );

	mWorld = world;
	if ( mWorld )
		mWorld->Add( mObject );

	OnCreatePhysical();
}

void IPhysicalObj::OnCreatePhysical()
{

}

void IPhysicalObj::SetWorld( CPhysics* world )
{
	if ( mObject && mWorld )
		mWorld->Remove( mObject );

	mWorld = world;
	
	if ( mObject && mWorld )
		mWorld->Add( mObject );
}

void IPhysicalObj::Collision( IPhysicalObj* o )
{
}

CPObject* IPhysicalObj::GetPhysicalObject()
{
	return mObject;
}

float IPhysicalObj::GetRot()
{
	return mRot;
}

float IPhysicalObj::GetY()
{
	return mY;
}

void IPhysicalObj::SetY( float y )
{
	mY = y;
}

void IPhysicalObj::SetRot( float r )
{
	mRot = r;
}
