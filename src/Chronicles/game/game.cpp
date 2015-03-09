#include "../entity/level.h"
#include "../entity/gate.h"
#include "../entity/player.h"
#include "../entity/monster.h"
#include "../entity/spike.h"
#include "../entity/item.h"
#include "../entity/bat.h"
#include "../entity/portal.h"
#include "../../engine/engine.h"
#include "../main.h"
#include "../entity/pickup.h"

int gPlayerScore;
int gPlayerSwordLevel;

void InitPlayerStats()
{
	gPlayerScore = 0;
	gPlayerSwordLevel = 1;
}

void HandleCollisions()
{
/*	Object Types
	0
	
	2		Item
	3		Monster
	7		Bat Monster

	4		World Walls
	6		Static Physics

	1		Player
	5		Sword
	8		Sword Blocking

	9		End of Level Portal

	10		"Pickup" (Health, Key, etc)

	11		Ghoul/Phantom Enemy 

	12		PLAYER_LEVITATE_OBJ

	13		Gate object

	?		Use Button Sensor
				- A circle slightly larger than the player, slightly in front of player,
				that detects which objects should be triggered when the use button is pressed,
				such as items like Treasure Chests, or Doors.
*/
	CListEntry<CCollision>* collisionEntry = gCollisions.GetFirst();
	while ( collisionEntry )
	{
		CPObject* object1 = collisionEntry->data->object1;
		CPObject* object2 = collisionEntry->data->object2;
		
		if ( gWorld->IsObjectInSimulation( object1 ) && gWorld->IsObjectInSimulation( object2 ) )
		{
			if ( object2->GetObjectType() == PLAYER_OBJ_TYPE || object2->GetObjectType() == PLAYER_LEVITATE_OBJ )
			{
				CPObject* temp = object1;
				object1 = object2;
				object2 = temp;
			}
			else if ( object2->GetObjectType() == SWORD_OBJ_TYPE )
			{
				CPObject* temp = object1;
				object1 = object2;
				object2 = temp;
			}
			else if ( object1->GetObjectType() == SPIKE_OBJ_TYPE )
			{
				CPObject* temp = object1;
				object1 = object2;
				object2 = temp;
			}

			/*	Player Collision	*/
			if ( object1->GetObjectType() == PLAYER_OBJ_TYPE && gPlayer != NULL )
			{
				if ( object2->GetObjectType() == MONSTER_OBJ_TYPE )
				{
					CMonster* monster = (dynamic_cast<CMonster*>((IPhysicalObj*)object2->GetOwner()));
					gPlayer->Hit( (dynamic_cast<CCharacter*>((IPhysicalObj*)object2->GetOwner())), monster->AttackStrength() );
				}
				else if ( object2->GetObjectType() == BAT_OBJ_TYPE )
				{
					CCharacter* bat = (dynamic_cast<CCharacter*>((IPhysicalObj*)object2->GetOwner()));
					if ( bat->GetY() < PLAYER_HEIGHT )
						gPlayer->Hit( bat, 1 );
				}
				else if ( object2->GetObjectType() == PORTAL_OBJECT_TYPE )
				{
					SVector v;
					object2->GetPosition().Difference( &object1->GetPosition(), &v );
					v.Scale( 6.0f );
					object1->SetVelocity( &v );

					CPortal* portal = (dynamic_cast<CPortal*>((IPhysicalObj*)object2->GetOwner()));
					if ( portal )
						portal->Glow();
					gPlayer->Levitate();

					/*	Next Level	*/
					if ( v.LengthSquared() < 0.1f && gPlayer->GetY() > 8.0f )
						ChangeGameState( NEXT_LEVEL_STATE );
					
				}
				else if ( object2->GetObjectType() == PICKUP_OBJ_TYPE )
				{
					CPickup* pickup = (dynamic_cast<CPickup*>((IPhysicalObj*)object2->GetOwner()));
					
					if ( pickup )
						pickup->PickUp();
				}
				else if ( object2->GetObjectType() == GATE_OBJ_TYPE )
				{
					CGate* gate = (dynamic_cast<CGate*>((IPhysicalObj*)object2->GetOwner()));
					if ( !gate->IsOpen() && gPlayer->RemoveKey() )
					{						
						gate->Open();
					}
				}
				else if ( object2->GetObjectType() == SPIKE_OBJ_TYPE )
				{
					CMonster* monster = (dynamic_cast<CMonster*>((IPhysicalObj*)object2->GetOwner()));
					if ( ((CSpike*)monster)->IsUp() )
					{
						gPlayer->Hit( (dynamic_cast<CCharacter*>((IPhysicalObj*)object2->GetOwner())), 2 );
					}
				}
			}
			else if ( object1->GetObjectType() == PLAYER_LEVITATE_OBJ && gPlayer != NULL )
			{
				if ( object2->GetObjectType() == PORTAL_OBJECT_TYPE )
				{
					SVector v;
					object2->GetPosition().Difference( &object1->GetPosition(), &v );
					v.Scale( 6.0f );
					object1->SetVelocity( &v );

					CPortal* portal = (dynamic_cast<CPortal*>((IPhysicalObj*)object2->GetOwner()));
					if ( portal )
						portal->Glow();
					gPlayer->Levitate();

					/*	Next Level	*/
					if ( gPlayer->GetY() > 8.0f )
						ChangeGameState( NEXT_LEVEL_STATE );
								
				}
			}
			/*	Sword Collision		*/
			else if ( object1->GetObjectType() == SWORD_OBJ_TYPE && gPlayer != NULL )
			{
				if ( object2->GetObjectType() == MONSTER_OBJ_TYPE )
				{
					(dynamic_cast<CCharacter*>((IPhysicalObj*)object2->GetOwner()))->Hit( gPlayer, SWORD_DAMAGE );
				}
				else if ( object2->GetObjectType() == BAT_OBJ_TYPE )
				{
					CCharacter* bat = (dynamic_cast<CCharacter*>((IPhysicalObj*)object2->GetOwner()));
					if ( bat->GetY() < PLAYER_HEIGHT + 1.0f )
						bat->Hit( gPlayer, SWORD_DAMAGE );
				}
				else if ( object2->GetObjectType() == ITEM_OBJ_TYPE )
				{
					(dynamic_cast<CItem*>((IPhysicalObj*)object2->GetOwner()))->Hit( (IPhysicalObj*)gPlayer, PLAYER_SWORD_FORCE );
				}
				else if ( object2->GetObjectType() == WALL_OBJ_TYPE )
				{
					gPlayer->StopSwing();
				}
			}

			if ( object2->GetObjectType() == SPIKE_OBJ_TYPE )
			{
				if ( object1->GetObjectType() == MONSTER_OBJ_TYPE )
				{
					CCharacter* by = (dynamic_cast<CCharacter*>((IPhysicalObj*)object2->GetOwner()));
					if ( ((CSpike*)by)->IsUp() )
					{
						(dynamic_cast<CCharacter*>((IPhysicalObj*)object1->GetOwner()))->Hit( by, 2 );
					}
				}
			}
		}
		
		delete collisionEntry->data;
		gCollisions.RemoveEntry( collisionEntry );
		collisionEntry = gCollisions.GetFirst();
	}

}


