#include "engine.h"
#include "update.h"
#include "entity/entity.h"

int GameUpdate( float dt )
{
	/*	Update Entities	*/
	CListEntry<CEntity>* entity = gEntities.GetFirst();
	while ( entity )
	{
		if ( entity->data )
		{
			entity->data->Update( dt );
			entity = entity->GetNext();
		} 
		else 
		{	/* Remove NULL entries	*/
			CListEntry<CEntity>* nextEntity = entity->GetNext();
			gEntities.RemoveEntry( entity );
			entity = nextEntity;
		}
	}

	return 1;
}


void GameCollisionFunction( CPObject* object1, CPObject* object2 )
{
	CCollision* c = new CCollision();
	c->object1 = object1;
	c->object2 = object2;
	gCollisions.Add( c );
}
