#ifndef _ENTITY_H
#define _ENTITY_H

class CEntity
{
public:
	virtual ~CEntity();

	virtual void Update( float dt );
	void RenderDebug();
};

#endif
