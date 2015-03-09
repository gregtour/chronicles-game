/**
 * Object.cpp
 * CHRONICLES PHYSICS ENGINE
 *	by Greg Tourville
 *	Copyright (c) 2007
 *	Tuesday Jan 23 -
 *	All Rights Reserved.
**/

#include <math.h>
#include "common.h"
#include "object.h"

#ifndef NULL
#define NULL 0
#endif

CPObject::CPObject()
{
	mMass = 1.0f;
	mObjectType = 0;
	mDynamic = false;

	mBounce = true;
	mBounceEfficiency = 0.9f;
	mDefaultForce.Set( 0.0f, 0.0f );
	mFriction = 0.1f;

	mShape = CP_OBJECT;
	mOwner = NULL;
}

CPObject::CPObject( SPoint* p, SVector* v, bool dynamic, int objectType )
{
	SetPosition( p );
	SetVelocity( v );
	SetMass( 0.0f );
	mDynamic = dynamic;
	mObjectType = objectType;
	CalculateBoundingBox();

	mBounce = true;
	mBounceEfficiency = 0.9f;
	mDefaultForce.Set( 0.0f, 0.0f );
	mFriction = 0.1f;

	mShape = CP_OBJECT;
	mOwner = NULL;
}

CPObject::~CPObject()
{
}

void CPObject::SetObjectType( int type )
{
	mObjectType = type;
}

void* CPObject::GetOwner()
{
	return mOwner;
}

void CPObject::SetOwner( void* o )
{
	mOwner = o;
}

bool CPObject::GetBounce()
{
	return mBounce;
}
float CPObject::GetBounceEfficiency()
{
	return mBounceEfficiency;
}

void CPObject::CalculateBoundingBox()
{

}

CPShape	CPObject::GetShape()
{
	return mShape;
}

void CPObject::SetPosition( SPoint* p )
{
	p->Copy( &mPosition );
}

void CPObject::SetVelocity( SVector* v )
{
	v->Copy( &mVelocity );
}

void CPObject::SetMass( float m )
{
	mMass = m;
}

void CPObject::SetFriction( float f )
{
	mFriction = f;
}

void CPObject::SetBounce( bool b )
{
	mBounce = b;
}

void CPObject::SetBounceEfficiency( float b )
{
	mBounceEfficiency = b;
}

SPoint CPObject::GetPosition()
{
	return mPosition;
}

SVector CPObject::GetVelocity()
{
	return mVelocity;
}

float CPObject::GetMass()
{
	return mMass;
}

SBox CPObject::GetBoundingBox()
{
	return mBoundingBox;
}

bool CPObject::IsDynamic()
{
	return mDynamic;
}

int CPObject::GetObjectType()
{
	return mObjectType;
}

void CPObject::ApplyForce( SVector* force )
{
	mVelocity.Add( force );
}

void CPObject::Update( float dt )
{
	/*	Move Object		*/
	SVector movementVector;
	SVector defForce;

	mDefaultForce.Scale( dt, &defForce );

	mVelocity.Add( &defForce );
	mVelocity.Scale( dt, &movementVector );
	mPosition.Translate( &movementVector );

	mVelocity.Scale( 1.0f - mFriction * dt );

	/*	Calculate Bounding Box	*/
	CalculateBoundingBox();
}

/*
void CPObject::Collide( CPObject* o1, CPObject* o2 )
{
	gOnCollision( o1, o2 );
}
*/

void CPObject::Translate( float x, float y )
{
	mPosition.x += x;
	mPosition.y += y;
}

void CPObject::BounceX()
{
	if ( mBounce )
		mVelocity.x = -mVelocity.x * mBounceEfficiency;
}

void CPObject::BounceY()
{
	if ( mBounce )
		mVelocity.y = -mVelocity.y * mBounceEfficiency;
}

/*
void CPObject::SetCollisionFunction( int (*collisionFunc)( CPObject*, CPObject* ) )
{
	gOnCollision = collisionFunc;
}
*/


/*	****************************************************************	*/

CPCircle::CPCircle() : CPObject()
{
	mShape = CP_CIRCLE;
	SetRadius( 0.0f );
}

CPCircle::CPCircle( SPoint* p, SVector* v, float r, bool dynamic, 
				   int objectType )
				   : CPObject( p, v, dynamic, objectType )
{
	mShape = CP_CIRCLE;
	SetRadius( r );
}

CPCircle::~CPCircle()
{
}

void CPCircle::SetRadius( float r )
{
	mRadius = r;
}

float CPCircle::GetRadius()
{
	return mRadius;
}

void CPCircle::CalculateBoundingBox()
{
	mBoundingBox.position.x = mPosition.x - mRadius;
	mBoundingBox.position.y = mPosition.y - mRadius;
	mBoundingBox.size.x = mRadius * 2;
	mBoundingBox.size.y = mRadius * 2;
}

/*	****************************************************************	*/

CPLine::CPLine() : CPObject()
{
	mShape = CP_LINE;
}

CPLine::CPLine( SPoint* p1, SPoint* p2, SPoint* p, SVector* v, bool dynamic, 
			   int objectType )
			   : CPObject( p, v, dynamic, objectType )
{
	mShape = CP_LINE;
	SetPoints( p1, p2 );
}

CPLine::~CPLine()
{
}

void CPLine::SetPoints( SPoint* p1, SPoint* p2 )
{
	p1->Copy( &mPoint1 );
	p2->Copy( &mPoint2 );
	ComputeNormal();
}

SPoint CPLine::GetPoint1()
{
	return mPoint1;
}

SPoint CPLine::GetPoint2()
{
	return mPoint2;
}

void CPLine::ComputeNormal()
{
	mPoint1.Difference( &mPoint2, &mNormal );
	float a = (float)(atan2( mNormal.y, mNormal.x ) + HALF_PI);
	mNormal.x = cos( a );
	mNormal.y = sin( a );	
}

SVector CPLine::GetNormal()
{
	return mNormal;
}

void CPLine::CalculateBoundingBox()
{
	SPoint point1, point2;
	point1.x = mPoint1.x + mPosition.x;
	point1.y = mPoint1.y + mPosition.y;
	point2.x = mPoint2.x + mPosition.x;
	point2.y = mPoint2.y + mPosition.y;
	mBoundingBox.FindBoxFromPoints( &point1, &point2 );
}


/*	****************************************************************	*/

