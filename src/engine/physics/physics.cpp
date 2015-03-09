/**
 * Physics.cpp
 * CHRONICLES PHYSICS ENGINE
 *	by Greg Tourville
 *	Copyright (c) 2007
 *	Tuesday Jan 23 -
 *	All Rights Reserved.
**/

#include "physics.h"
#include <stdio.h>

CPhysics::CPhysics()
{
	mBounds = false;
	SetCollisionFunction( NULL );
	mObjectTypeCollisions = NULL;
	mObjectTypeCollisionsSize = 0;

#ifdef PHYSICS_LOGGING	
	gLog.LogItem( new CLogMessage( "New Physics Simulation Created" ) );
#endif
}

CPhysics::~CPhysics( )
{
	if ( mObjectTypeCollisions )
	{
		delete mObjectTypeCollisions;
	}

	if ( mObjects.length > 0 )
	{
#ifdef PHYSICS_LOGGING
		std::stringstream stream;
		stream << "Physics Warning: " << mObjects.length << " physics object(s) left in simulation during object destruction.";
		gLog.LogItem( new CLogMessage( stream.str() ) );
#endif

		CListEntry<CPObject>* object = mObjects.GetFirst();
		while ( object )
		{
			delete object->data;
			mObjects.RemoveEntry( object );
			object = mObjects.GetFirst();
		}
	}
	
#ifdef PHYSICS_LOGGING
	gLog.LogItem( new CLogMessage( "Physics Simulation Destroyed" ) );
#endif
}


#ifdef ALLOW_OBJECT_ACCESS
CLinkedList<CPObject>*	CPhysics::GetObjectList()
{
	return &mObjects;
}
#endif


void CPhysics::SetObjectTypeCollisionsArraySize( int size, bool defaultOption )
{
	mObjectTypeCollisionsSize = size;
	
	if ( mObjectTypeCollisions )
	{
		delete mObjectTypeCollisions;
	}

	mObjectTypeCollisions = new bool[size*size];

	for ( int x = 0; x < size; x++ )
		for ( int y = 0; y < size; y++ )
			SetObjectTypeCollision( x, y, defaultOption );
}

void CPhysics::SetObjectTypeCollision( int object1, int object2, bool setting )
{
	if ( mObjectTypeCollisions && object1 < mObjectTypeCollisionsSize &&
		object2 < mObjectTypeCollisionsSize )
	{
		mObjectTypeCollisions[ object1 + object2*mObjectTypeCollisionsSize ] = setting;
		mObjectTypeCollisions[ object2 + object1*mObjectTypeCollisionsSize ] = setting;
	}
}

bool CPhysics::GetObjectTypeCollision( int object1, int object2 )
{
	if ( mObjectTypeCollisions && object1 < mObjectTypeCollisionsSize &&
		object2 < mObjectTypeCollisionsSize )
	{
		return mObjectTypeCollisions[ object1 + object2 * mObjectTypeCollisionsSize ];
	}
	else
	{
		// Error
		return false;
	}
}


bool CPhysics::IsObjectInSimulation( CPObject* o )
{
	return mObjects.FindEntry( o ) != NULL;
}

void CPhysics::Update( float dt )
{
	SyncObjects( dt );
	SyncCollisions();
}


void CPhysics::Add( CPObject* obj )
{
	mObjects.Add( obj );
}

void CPhysics::Remove( CPObject* obj )
{
	CListEntry<CPObject>* e = mObjects.FindEntry( obj );
	if ( e )
	{
		mObjects.RemoveEntry( e );
	} else {
		// Object not found in Object List
		gLog.LogItem( new CLogMessage("Physics Warning: Could not find object to remove.") );
	}
}

void CPhysics::SetCollisionFunction( void (*collisionFunction)(CPObject*, CPObject*) )
{
	mCollisionFunction = collisionFunction;
}


void CPhysics::SetBoundry( float minX, float minY, float maxX, float maxY )
{
	mBounds = true;
	mMinX = minX;
	mMinY = minY;
	mMaxX = maxX;
	mMaxY = maxY;
}

void CPhysics::GetMinimumBoundry( float* x, float* y )
{
	if ( mBounds )
	{
		*x = mMinX;
		*y = mMinY;
	}
}

void CPhysics::GetMaximumBoundry( float* x, float* y )
{
	if ( mBounds )
	{
		*x = mMaxX;
		*y = mMaxY;
	}
}

void CPhysics::DisableBoundries()
{
	mBounds = false;
}

bool CPhysics::GetBounds()
{
	return mBounds;
}

void CPhysics::CheckBounds( CPObject* obj )
{
	if ( mBounds )
	{
		float moveX = 0.0f;
		float moveY = 0.0f;
		SBox box = obj->GetBoundingBox();

		if ( box.position.x < mMinX )
			moveX = mMinX - box.position.x;
		else if ( box.position.x + box.size.x > mMaxX )
			moveX = mMaxX - (box.position.x + box.size.x);
		if ( box.position.y < mMinY )
			moveY = mMinY - box.position.y;
		else if ( box.position.y + box.size.y > mMaxY )
			moveY = mMaxY - (box.position.y + box.size.y);

		if ( moveX != 0.0f || moveY != 0.0f )
		{
			obj->Translate( moveX, moveY );
			obj->CalculateBoundingBox();

			if ( moveX != 0.0f && ( moveX*obj->GetVelocity().x < 0 ) )
			{
				obj->BounceX();
			}
			if ( moveY != 0.0f && ( moveY*obj->GetVelocity().y < 0 ) )
			{
				obj->BounceY();
			}
		}
	}
}



void CPhysics::SyncObjects( float dt )
{
	// Moves Objects, Keeps in Bounds

	CListEntry<CPObject>* e = mObjects.GetFirst();
	while ( e )
	{
		if ( e->data->IsDynamic() )
		{
			e->data->Update( dt );
			CheckBounds( e->data );	
		} else {
			e->data->CalculateBoundingBox();
		}
		e = e->GetNext();
	}
}



void CPhysics::SyncCollisions()
{
	CListEntry<CPObject>*	object1;
	CListEntry<CPObject>*	object2;

	object1 = mObjects.GetFirst();
	while ( object1 )
	{
		object2 = object1->GetNext();
		while ( object2 )
		{
			if ( object1->data->IsDynamic() || object2->data->IsDynamic() )
				Collision( object1->data, object2->data );

			object2 = object2->GetNext();
		}

		object1 = object1->GetNext();
	}
}
