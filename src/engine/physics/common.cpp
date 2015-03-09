/**
 * common.cpp
 * CHRONICLES PHYSICS ENGINE
 *	by Greg Tourville
 *	Copyright (c) 2007
 *	Tuesday Jan 23 -
 *	All Rights Reserved.
**/

#include <math.h>
#include "common.h"

#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))



SPoint::SPoint()
{
	x = 0.0f;
	y = 0.0f;
}

SPoint::SPoint( float x, float y )
{
	Set( x, y );
}

void SPoint::Set( float x, float y )
{
	this->x = x;
	this->y = y;
}

void SPoint::Translate( SVector* v )
{
	x += v->x;
	y += v->y;
}

void SPoint::Add( SPoint* p )
{
	x += p->x;
	y += p->y;
}

void SPoint::Difference( SPoint* p2, SVector* ret )
{
	ret->x = x - p2->x;
	ret->y = y - p2->y;
}


void SPoint::Copy( SPoint* dest )
{
	dest->Set( x, y );
}





SVector::SVector()
{
	x = 0.0f;
	y = 0.0f;
} 

SVector::SVector( float x, float y )
{
	Set( x, y );
}

void SVector::Set( float x, float y )
{
	this->x = x;
	this->y = y;
}

void SVector::Copy( SVector* ret )
{
	ret->Set( x, y );
}

void SVector::Flip()
{
	x = -x;
	y = -y;
}

void SVector::Add( SVector* v )
{
	x += v->x;
	y += v->y;
}

void SVector::Subtract( SVector* v )
{
	x -= v->x;
	y -= v->y;
}

float SVector::Length()
{
	return sqrt( x*x + y*y );
}

float SVector::LengthSquared()
{
	return ( x*x + y*y );
}

float SVector::SquareLength()
{
	return fabs( x ) + fabs( y );
}

void SVector::Scale( float s, SVector* result )
{
	Copy( result );
	result->Scale( s );
}

void SVector::Scale( float s )
{
	x *= s;
	y *= s;
}

void SVector::Normalize()
{
	float len = Length();
	if ( len > 0.000001f )
	{
		x /= len;
		y /= len;
	}
}

float SVector::DotProduct( SVector* v2 )
{
	return v2->x*x + v2->y*y;
}

void SVector::Rotate( float degs )
{
	float a = atan2( y, x );
	float l = Length();
	x = (float)(l * cos( a + (degs*RADS) ));
	y = (float)(l * sin( a + (degs*RADS) ));	
}





SBox::SBox()
{
	position.Set( 0.0f, 0.0f );
	size.Set( 0.0f, 0.0f );
}

SBox::SBox( SPoint* pos, SPoint* sz )
{
	pos->Copy( &position );
	sz->Copy( &size );
}

bool SBox::IntersectsWithBoxX( SBox* box )
{
	if ( this->position.x < box->position.x )
	{
		return ( box->position.x < (this->position.x + this->size.x) );
	} else {
		return ( this->position.x < (box->position.x + box->size.x) );
	}
}

bool SBox::IntersectsWithBoxY( SBox* box )
{
	if ( this->position.y < box->position.y )
	{
		return ( box->position.y < (this->position.y + this->size.y) );
	} else {
		return ( this->position.y < (box->position.y + box->size.y) );
	}
}

bool SBox::IntersectsWithBox( SBox* box )
{
	return IntersectsWithBoxX( box ) && IntersectsWithBoxY( box );
}

void SBox::FindBoxFromPoints( SPoint* p1, SPoint* p2 )
{
	position.x = MIN( p1->x, p2->x );
	position.y = MIN( p1->y, p2->y );
	size.x = MAX( p1->x, p2->x ) - position.x;
	size.y = MAX( p1->y, p2->y ) - position.y;
}



SPoint ClosestPointOnLineSegment( SPoint* line1, SPoint* line2, SPoint* point )
{
	SPoint a, b, p;
	a = *line1;
	b = *line2;
	p = *point;

	SVector c, v;
	float d, t;

	p.Difference( &a, &c );
	b.Difference( &a, &v );
	
	d = v.Length();
	v.Normalize();
	t = v.DotProduct( &c );

	if ( t < 0 ) return a;
	if ( t > d ) return b;

	v.Scale( t );
	a.Translate( &v );
	return a;
}




float atan( SPoint* p )
{
	return atan2( p->y, p->x );
}



float atan( SVector* v )
{
	return atan2( v->y, v->x );
}
