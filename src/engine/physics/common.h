/**
 * common.h
 * CHRONICLES PHYSICS ENGINE
 *	by Greg Tourville
 *	Copyright (c) 2007
 *	Tuesday Jan 23 -
 *	All Rights Reserved.
**/

#ifndef _PHYSICS_COMMON_H
#define _PHYSICS_COMMON_H

#ifndef	RADS
#	define		RADS			(0.017453f)
#endif
#define		DEGS			(57.29578f)
#ifndef PI
#	define		PI				(3.141593f)
#endif
#define		HALF_PI			(1.570796f)

#ifndef ROOT_TWO
#	define ROOT_TWO	(1.414214f)
#endif

struct SPoint;
struct SVector;
struct SBox;

SPoint ClosestPointOnLineSegment( SPoint* line1, SPoint* line2, SPoint* point );

float atan( SPoint* );
float atan( SVector* );

struct SPoint
{
	SPoint();
	SPoint( float, float );

	float x, y;
	
	void	Set( float x, float y );
	void	Translate( SVector* v );
	void	Difference( SPoint* p2, SVector* ret );
	void	Add( SPoint* p );
	void	Copy( SPoint* dest );
};

struct SVector
{
	SVector();
	SVector( float, float );

	float x, y;
	
	void	Set( float, float );
	void	Copy( SVector* ret );
	void	Flip();
	void 	Add( SVector* v );
	void 	Subtract( SVector* v );
	float 	Length();			// Real Length
	float	LengthSquared();	// Length without sqrt()
	float	SquareLength();		// Fast Cartesian Length
	void 	Scale( float s, SVector* ret );
	void	Scale( float s );
	void 	Normalize();
	float 	DotProduct( SVector* );
	void	Rotate( float deg );
};

struct SBox
{
	SBox();
	SBox( SPoint*, SPoint* );

	bool IntersectsWithBox( SBox* box );
	bool IntersectsWithBoxX( SBox* box );
	bool IntersectsWithBoxY( SBox* box );
	void FindBoxFromPoints( SPoint* p1, SPoint* p2 );

	SPoint position;
	SPoint size;

};


#endif
