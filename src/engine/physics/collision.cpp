/**
 * Collision.cpp
 * CHRONICLES PHYSICS ENGINE
 *	by Greg Tourville
 *	Copyright (c) 2007
 *	Tuesday Jan 23 -
 *	All Rights Reserved.
**/

/*	Rough Check			*/
//	if ( !s1->box->intersect( s2->box ) )
//		return;

#include "common.h"
#include "object.h"
#include "physics.h"
#include <stdio.h>
#include <math.h>

/*		****************************************************	*/


void CPhysics::Collision( CPObject* object1, CPObject* object2 )
{
	/*	Bounding Box Quick Collision Check	*/
	if ( !object1->GetBoundingBox().IntersectsWithBox
		( &object2->GetBoundingBox() ) )
	{
		return;
	}

	/*	Real Collision Detection & Response	*/
	if ( object1->GetShape() == CP_CIRCLE )
	{
		if ( object2->GetShape() == CP_CIRCLE )
		{
			CircleCircleCollision( (CPCircle*)object1, (CPCircle*)object2 );
		} 
		else if ( object2->GetShape() == CP_LINE )
		{
			CircleLineCollision( (CPCircle*)object1, (CPLine*)object2 );
		}
		else
		{
			BoundingBoxCollision( object1, object2 );
		}
	} 
	else if ( object1->GetShape() == CP_LINE )
	{
		if ( object2->GetShape() == CP_CIRCLE )
		{
			CircleLineCollision( (CPCircle*)object2, (CPLine*)object1 );
		} 
		else if ( object2->GetShape() == CP_LINE )
		{
			LineLineCollision( (CPLine*)object1, (CPLine*)object2 );
			//BoundingBoxCollision( object1, object2 );
		}
		else
		{
			BoundingBoxCollision( object1, object2 );
		}
	} 
	else 
	{
		BoundingBoxCollision( object1, object2 );
	}
}



/*		****************************************************	*/


void CPhysics::CircleCircleCollision( CPCircle* circle1, CPCircle* circle2 )
{
	if ( !mObjectTypeCollisions || 
		GetObjectTypeCollision( circle1->GetObjectType(), circle2->GetObjectType() ) )
	{
		if ( !circle2->IsDynamic() )
		{
			CPCircle* tmp = circle1;
			circle1 = circle2;
			circle2 = tmp;
		}

		SVector distanceVector;
		SVector movementVector;
		SVector	bounceVector1;
		SVector	bounceVector2;
		float distance;
		float movementDistance;
		float energyExchange;

		/*	Absolute Check			*/
		circle1->GetPosition().Difference( &circle2->GetPosition(), &distanceVector );
		distance = distanceVector.Length() - circle1->GetRadius() - circle2->GetRadius();

		if ( distance > 0.0f )
		{
			return;
		}


		/*	Fix Collision			*/
		distanceVector.Normalize();
		
		// Scale it to half of the distance overlap
		movementDistance = -distance;
		distanceVector.Scale( movementDistance, &movementVector );

		// Move Spheres Along the Vector
		if ( circle1->IsDynamic() )
		{
			if ( circle2->IsDynamic() )
			{
				movementVector.Scale( 0.5f );
				
				circle1->Translate( movementVector.x, movementVector.y );
				circle2->Translate( -movementVector.x, -movementVector.y );			
			}
		} else {
			circle2->Translate( -movementVector.x, -movementVector.y );			
		}

	//		Collision Response		

		if ( circle1->IsDynamic() && circle1->GetBounce() )
		{
			if ( circle2->GetBounce() )
			{
				// Find Length of the Movement Vectors Along the Vector
				float a1 = circle1->GetVelocity().DotProduct( &distanceVector );	
				float a2 = circle2->GetVelocity().DotProduct( &distanceVector );

				// Energy Exchange
				energyExchange = (a1 - a2);
				distanceVector.Scale( energyExchange );

				distanceVector.Scale( circle2->GetBounceEfficiency(), &bounceVector1 );
				circle2->ApplyForce( &bounceVector1 );

				distanceVector.Scale( circle1->GetBounceEfficiency(), &bounceVector2 );
				bounceVector2.Flip();
				circle1->ApplyForce( &bounceVector2 );
			} else {
				// r is Reflected, i is Angle, n is Normal of Plane of Reflection
				//r = i - (2 * n * dot(i, n))
				SVector angle = circle1->GetVelocity();
				distanceVector.Scale( angle.DotProduct( &distanceVector ) * 2 * circle1->GetBounceEfficiency() );
				angle.Subtract( &distanceVector );
				circle1->SetVelocity( &angle );
			}
		} else {
			// r is Reflected, i is Angle, n is Normal of Plane of Reflection
			//r = i - (2 * n * dot(i, n))
			SVector angle = circle2->GetVelocity();
			distanceVector.Scale( angle.DotProduct( &distanceVector ) * 2 * circle2->GetBounceEfficiency() );
			angle.Subtract( &distanceVector );
			circle2->SetVelocity( &angle );
		}
	}

	/*	Call Functions	*/
	//CPObject::Collide( (CPObject*)circle1, (CPObject*)circle2 );
	if ( mCollisionFunction != NULL )
		mCollisionFunction( (CPObject*)circle1, (CPObject*)circle2 );
}

/*		****************************************************	*/

void CPhysics::LineLineCollision( CPLine* line1, CPLine* line2 )
{
	SPoint p1point1 = line1->GetPoint1();
	p1point1.Add( &line1->GetPosition() );
	SPoint p1point2 = line1->GetPoint2();
	p1point2.Add( &line1->GetPosition() );
	
	SPoint p2point1 = line2->GetPoint1();
	p2point1.Add( &line2->GetPosition() );
	SPoint p2point2 = line2->GetPoint2();
	p2point2.Add( &line2->GetPosition() );

	SPoint closestPoint1 = ClosestPointOnLineSegment( &p1point1, &p1point2, &p2point1 );
	SPoint closestPoint2 = ClosestPointOnLineSegment( &p1point1, &p1point2, &p2point2 );
	SVector direction1;
	p2point1.Difference( &closestPoint1, &direction1 );
	SVector direction2;
	p2point2.Difference( &closestPoint2, &direction2 );

	float angle1 = atan( &direction1 );
	float angle2 = atan( &direction2 );
	angle2 += PI;

	while ( angle1 >= 2*PI )
		angle1 -= (2*PI);
	while ( angle2 >= 2*PI )
		angle2 -= (2*PI);

	/*	Collision!	*/
	if ( fabs( angle1 - angle2 ) <= 0.01f )
	{
		if ( mCollisionFunction != NULL )
			mCollisionFunction( (CPObject*)line1, (CPObject*)line2 );
	}
}


/*		****************************************************	*/


void CPhysics::BoundingBoxCollision( CPObject* object1, CPObject* object2 )
{
	if ( !mObjectTypeCollisions || 
		GetObjectTypeCollision( object1->GetObjectType(), object2->GetObjectType() ) )
	{
		SBox box1 = object1->GetBoundingBox();
		SBox box2 = object2->GetBoundingBox();

		if ( box1.IntersectsWithBoxX( &box2 ) )
		{
			float moveX = 0.0f;

			if ( box1.position.x < box2.position.x )
				moveX = ( box2.position.x - (box1.position.x + box1.size.x) ) / 2.0f;
			else
				moveX = ( box1.position.x - (box2.position.x + box2.size.x) ) / 2.0f;

			if ( object1->IsDynamic() )
			{
				if ( object2->IsDynamic() )
				{	// Both Dynamic
					object1->Translate( moveX, 0.0f );
					object2->Translate( -moveX, 0.0f );
					object1->BounceX();
					object2->BounceX();
				} else {
					// o1 is dynamic
					object1->Translate( moveX * 2, 0.0f );
					object1->BounceX();
				}
			} else {
				if ( object2->IsDynamic() )
				{	// o2 is dynamic
					object2->Translate( -moveX * 2, 0.0f );
					object2->BounceX();
				}
			}
		}

		if ( box1.IntersectsWithBoxY( &box2 ) )
		{
			float moveY = 0.0f;

			if ( box1.position.y < box2.position.y )
				moveY = ( box2.position.y - (box1.position.y + box1.size.y) ) / 2.0f;
			else
				moveY = ( box1.position.y - (box2.position.y + box2.size.y) ) / 2.0f;

			if ( object1->IsDynamic() )
			{
				if ( object2->IsDynamic() )
				{	// Both Dynamic
					object1->Translate( moveY, 0.0f );
					object2->Translate( -moveY, 0.0f );
					object1->BounceY();
					object2->BounceY();
				} else {
					// o1 is dynamic
					object1->Translate( moveY * 2, 0.0f );
					object1->BounceY();
				}
			} else {
				if ( object2->IsDynamic() )
				{	// o2 is dynamic
					object2->Translate( -moveY * 2, 0.0f );
					object2->BounceY();
				}
			}
		}
	}

	if ( mCollisionFunction != NULL )
		mCollisionFunction( object1, object2 );

	return;
}


/*		****************************************************	*/

void CPhysics::CircleLineCollision( CPCircle* circle, CPLine* line )
{
	if ( !mObjectTypeCollisions || 
		GetObjectTypeCollision( circle->GetObjectType(), line->GetObjectType() ) )
	{
		SPoint closestPoint;
		SPoint newPoint;
		SVector normal;
		SVector movementVector;
		float distance;

		SPoint a = line->GetPoint1();
		a.x += line->GetPosition().x;
		a.y += line->GetPosition().y;

		SPoint b = line->GetPoint2();
		b.x += line->GetPosition().x;
		b.y += line->GetPosition().y;

		closestPoint = ClosestPointOnLineSegment( &a, &b, &circle->GetPosition() );
		circle->GetPosition().Difference( &closestPoint, &normal );
		distance = normal.Length();

		if ( distance - circle->GetRadius() > 0.0f )
			return;

		normal.Normalize();
		normal.Scale( circle->GetRadius(), &movementVector );
		newPoint = closestPoint;
		newPoint.Translate( &movementVector );

		/*	Position Correction	*/
		circle->SetPosition( &newPoint );

		/*	Bounce	*/
		if ( circle->GetBounce() )
		{	//r = i - (2 * n * dot(i, n))
	//		float reflection = atan( &normal );
			SVector angle = circle->GetVelocity();		
			normal.Scale( angle.DotProduct( &normal ) * 2 * circle->GetBounceEfficiency() );
			angle.Subtract( &normal );
			circle->SetVelocity( &angle );
		}
	}

	if ( mCollisionFunction != NULL )
		mCollisionFunction( (CPObject*)circle, (CPObject*)line );
}


/*		****************************************************	*/

