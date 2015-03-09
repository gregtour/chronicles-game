
#include "gameInput.h"


int SetupInput()
{
	/*	Create the Global Input State	*/
	gInputState = new CInputState();

	/*	Create the List of Recognized Keys	*/
	CKey* k;
	k = new CKey( SDLK_UP, K_UP );
	gKeyList.Add( k );

	k = new CKey( SDLK_DOWN, K_DOWN );
	gKeyList.Add( k );

	k = new CKey( SDLK_LEFT, K_LEFT );
	gKeyList.Add( k );

	k = new CKey( SDLK_RIGHT, K_RIGHT );
	gKeyList.Add( k );

//	k = new CKey( SDLK_SPACE, K_RESTART );
//	gKeyList.Add( k );

	k = new CKey( SDLK_RETURN, K_CONTINUE );
	gKeyList.Add( k );

	gKeyList.Add( new CKey( SDLK_SPACE, K_SWING ) );

	gKeyList.Add( new CKey( SDLK_s, K_BLOCK ) );

	gKeyList.Add( new CKey( SDLK_d, K_CAST ) );

	/*	Set up gInputState with the Recognized Keys	*/
	CListEntry<CKey>* e = gKeyList.GetFirst();
	while ( e )
	{
		gInputState->keys.Add( new CKeyState( e->data->keyCode, e->data->keyNumber ) );
		e = e->GetNext();
	}

	return 1;
}

