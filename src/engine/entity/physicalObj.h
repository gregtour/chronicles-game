#ifndef _PHYSICALOBJ_H
#define _PHYSICALOBJ_H

#include "../physics/physics.h"


int PhysicalObjectCollision( CPObject* a, CPObject* b );

#ifndef NULL
#	define NULL 0L
#endif

class IPhysicalObj
{
public:
	IPhysicalObj();
	virtual ~IPhysicalObj();

	void CreatePhysical( CPhysics* world, SPoint* position, SVector* velocity, float radius, float mass, float friction, float bounce, bool dynamic, int objType );
	virtual void OnCreatePhysical();
	void SetWorld( CPhysics* world );

	virtual void Collision( IPhysicalObj* o );
	
	CPObject* GetPhysicalObject();
	float GetY();
	float GetRot();
	void SetY( float y );
	void SetRot( float r );

protected:
	CPhysics*	mWorld;
	CPObject*	mObject;
	float mRot;
	float mY;
};

#endif
