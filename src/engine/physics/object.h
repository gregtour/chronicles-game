/**
 * Object.h
 * CHRONICLES PHYSICS ENGINE
 *	by Greg Tourville
 *	Copyright (c) 2007
 *	Tuesday Jan 23 -
 *	All Rights Reserved.
**/

/**
 * Todo:
 *		solid / not solid
 *		collision funcitons
**/



#ifndef _OBJECT_H
#define _OBJECT_H

// Object, Sphere, Line, Triangle, CustomObject

enum CPShape
{
	CP_OBJECT,
	CP_CIRCLE,
	CP_LINE
};


class CPObject;
class CPCircle;
class CPLine;


class CPObject
{
public:
	/*	Constructors & Destructors	*/
	CPObject();
	CPObject( SPoint* p, SVector* v, bool dynamic, int objectType );
	virtual ~CPObject();
	
	void Update( float dt );
		
	virtual void CalculateBoundingBox();

	void	SetPosition( SPoint* p );
	void	SetVelocity( SVector* v );
	void	SetMass( float m );
	void	SetFriction( float f );
	void	SetBounce( bool b );
	void	SetBounceEfficiency( float b );
	void	SetOwner( void* o );
	void	SetObjectType( int type );

	void*	GetOwner();
	CPShape	GetShape();
	bool	GetBounce();
	float	GetBounceEfficiency();
	SPoint	GetPosition();
	SVector	GetVelocity();
	float	GetMass();
	SBox	GetBoundingBox();
	int		GetObjectType();
	bool	IsDynamic();

	void	ApplyForce( SVector* force );
	void	Translate( float x, float y );
	void	BounceX();
	void	BounceY();

	//static void	Collide( CPObject* o1, CPObject* o2 );
	//static void SetCollisionFunction( int (*collisionFunc)( CPObject*, CPObject* ) );

protected:

	//static int (*gOnCollision)( CPObject*, CPObject* );


	/*	Properties	*/
	SPoint	mPosition;
	SVector mVelocity;	
	SBox	mBoundingBox;
	float	mMass;	

	/*	Options	*/
	CPShape	mShape;
	int		mObjectType;
	
	/*	Behaviors	*/
	bool	mDynamic;
	bool	mBounce;
	float	mBounceEfficiency;
	SVector	mDefaultForce;
	float	mFriction;

	void*	mOwner;

};


class CPCircle: public CPObject
{
public:
	/*	Constructors & Destructors	*/
	CPCircle();
	CPCircle( SPoint* p, SVector* v, float r, bool dynamic, int objectType );
	~CPCircle();

	/*	Access Methods	*/
	void SetRadius( float r );
	float GetRadius();

	/*	Overridden Methods	*/
	void CalculateBoundingBox();

protected:
	/*	Data	*/
	float mRadius;
};


class CPLine: public CPObject
{
public:
	/*	Constructors & Destructors	*/
	CPLine();
	CPLine( SPoint* p1, SPoint* p2, SPoint* p, SVector* v, bool dynamic, int objectType );
	~CPLine();
	
	void	SetPoints( SPoint* p1, SPoint* p2 );

	SPoint	GetPoint1();
	SPoint	GetPoint2();
	SVector	GetNormal();

	void	ComputeNormal();
	void	CalculateBoundingBox();

protected:
	/*	Data	*/
	SPoint mPoint1, mPoint2;
	SVector mNormal;	
};

#endif
