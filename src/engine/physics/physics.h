/**
 * Physics.h
 * CHRONICLES PHYSICS ENGINE
 *	by Greg Tourville
 *	Copyright (c) 2007
 *	Tuesday Jan 23 -
 *	All Rights Reserved.
**/
/*
	Todo:
		fix boundingbox collision
		fix circlecircle collision
		rewrite "staticcollisions" stuff
		mSolid - (moves other objects) and mMovable - (moved by solid objects)
		write CPBox
		remove grid?
		object rotation?
		clean up
*/

#ifndef _PHYSICS_H
#define _PHYSICS_H

#define ALLOW_OBJECT_ACCESS
#define PHYSICS_LOGGING


#include "common.h"
#include "object.h"
#include "../common/list.h"

#ifdef PHYSICS_LOGGING
	#include "../common/log.h"
	extern CLog gLog;
	#include <string>
	#include <sstream>
#endif

class CPhysics
{
public:
	CPhysics();
	~CPhysics();

	/* "ObjectTypeCollisions" Functions	*/
	void SetObjectTypeCollisionsArraySize( int size, bool defaultOption );
	void SetObjectTypeCollision( int object1, int object2, bool setting );
	bool GetObjectTypeCollision( int object1, int object2 );

	bool IsObjectInSimulation( CPObject* o );

	void Update( float dt );
	void Add( CPObject* o );
	void Remove( CPObject* o );

#ifdef ALLOW_OBJECT_ACCESS
	CLinkedList<CPObject>*	GetObjectList();
#endif

	void DisableBoundries();
	bool GetBounds();
	void SetBoundry( float minX, float minY, float maxX, float maxY );
	void GetMinimumBoundry( float* x, float* y );
	void GetMaximumBoundry( float* x, float* y );

	void SetCollisionFunction( void (*collisionFunction)(CPObject*, CPObject*) );

protected:
	CLinkedList<CPObject>		mObjects;		// Created at beginning and changed throughout

	void SyncObjects( float );
	void SyncCollisions();
	void CheckBounds( CPObject* obj );

	/*	Collisions	*/
	void Collision( CPObject* object1, CPObject* object2 );

	void BoundingBoxCollision( CPObject* object1, CPObject* object2 );
	void LineLineCollision( CPLine* line1, CPLine* line2 );
	void CircleLineCollision( CPCircle* circle, CPLine* line );
	void CircleCircleCollision( CPCircle* circle1, CPCircle* circle2 );

	void (*mCollisionFunction)(CPObject*, CPObject*);

	
	bool mBounds;
	float mMinX, mMinY;
	float mMaxX, mMaxY;

	/*		Array that may be used by programmer using this Physics code
				in order to set which objects will cause collision corrections
				and bounces when they come in contact. CPhysics will still call
				the collision callback even if two objects are set not to collide
	*/
	bool* mObjectTypeCollisions;
	int mObjectTypeCollisionsSize;


};

#endif
