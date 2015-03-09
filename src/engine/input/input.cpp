#include "input.h"
#include "inputData.h"
#include "../entity/inputtingObj.h"
#include "../common/log.h"
#include "../engine.h"

CLinkedList<IInputtingObj>	gInputtingEntityQueue;
CInputState*				gInputState;
CInputState					gBlankInputState;
CLinkedList<CKey>			gKeyList;

void MousePress( SDL_MouseButtonEvent m );
void MouseMovement( SDL_MouseMotionEvent m );
void PressKey( SDL_KeyboardEvent k ); 
void KeyStatus( SDL_KeyboardEvent k ); 
//void JoyInput( SDL_Event* e );

void InputEvent( SDL_Event* e )
{
	switch ( e->type )
	{
		case SDL_MOUSEMOTION:
			MouseMovement( e->motion );
			break;

		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			MousePress( e->button );
			break;

		case SDL_JOYBUTTONDOWN:
		case SDL_JOYBUTTONUP:
		case SDL_JOYAXISMOTION:
//			JoyInput( e );
			break;

		case SDL_KEYDOWN:
			PressKey( e->key );
		case SDL_KEYUP:
			KeyStatus( e->key );
			break;		
	}
}
/*
void JoyInput( SDL_Event* e )
{
	switch ( e->type )
	{
		case SDL_JOYBUTTONDOWN:
		case SDL_JOYBUTTONUP:



		case SDL_JOYAXISMOTION:
			JoyInput( e );
			break;
	}
}
*/
void MousePress( SDL_MouseButtonEvent m )
{

}

void MouseMovement( SDL_MouseMotionEvent m )
{
	gInputState->mousePosition.mMouseLastX = gInputState->mousePosition.mMouseX;
	gInputState->mousePosition.mMouseLastY = gInputState->mousePosition.mMouseY;
	gInputState->mousePosition.mouseTime.Step();
	gInputState->mousePosition.mMouseX = m.x;
	gInputState->mousePosition.mMouseY = m.y;
	gInputState->mousePosition.mMouseXSp = (gInputState->mousePosition.mMouseX - gInputState->mousePosition.mMouseLastX)/gInputState->mousePosition.mouseTime.DT();
	gInputState->mousePosition.mMouseYSp = (gInputState->mousePosition.mMouseY - gInputState->mousePosition.mMouseLastY)/gInputState->mousePosition.mouseTime.DT();
}

void PressKey( SDL_KeyboardEvent k )
{
	SDLKey key = k.keysym.sym;
	CListEntry<CKey>* e = gKeyList.GetFirst();

#ifdef ESCAPE_QUIT
	if ( key == SDLK_ESCAPE )
		EndGame();
#endif
	
	while ( e && e->data->keyCode != key )
	{
		e = e->GetNext();
	}

	if ( e )
	{
//		gLog.LogItem( new CLogMessage("Recognized Input Press") );

		gInputState->keyPresses.Add( e->data );

		// If the list exceeds the maximum length, remove the first entry
		if ( gInputState->keyPresses.GetLength() > MAX_KEY_PRESSES )
			gInputState->keyPresses.RemoveEntry( gInputState->keyPresses.GetFirst() );
	}
}

void KeyStatus( SDL_KeyboardEvent k )
{
	/*	Find Key in the Key Status List	*/
	SDLKey key = k.keysym.sym;
	CListEntry<CKeyState>* e = gInputState->keys.GetFirst();
	
	while ( e && e->data->key.keyCode != key )
	{
		e = e->GetNext();
	}

	if ( e )	/*	Key was found	*/
	{
		bool pressed = (k.state == SDL_PRESSED);
		
		/*	If there have been previous statuses	*/
		if ( e->data->status.GetLast() )
		{
			/*	Check that the new status is different than the past one	*/
			CKeyStatus* curStatus = e->data->status.GetLast()->data;
			if ( pressed != curStatus->status )
			{
				curStatus->duration.Step();	// Stop the timer on the old one

				/*	Create the new one	*/
				CKeyStatus* newStatus = new CKeyStatus();
				newStatus->status = pressed;
				newStatus->duration.Clear();

				/*	Add it to the list & make sure the list isn't too long	*/
				e->data->status.Add( newStatus );
				if ( e->data->status.GetLength() > MAX_KEY_STATUSES )
					e->data->status.RemoveEntry( e->data->status.GetFirst() );
			}
		} else {
			/*	If there are no previous ones, just add the current one	*/
			CKeyStatus* newStatus = new CKeyStatus();
			newStatus->status = pressed;
			newStatus->duration.Clear();
			e->data->status.Add( newStatus );
		}
	}
}


void BindEntityToInput( IInputtingObj* entity )
{
	CListEntry<IInputtingObj>* otherObjects = gInputtingEntityQueue.GetFirst();
	while ( otherObjects )
	{
		otherObjects->data->SetInputState( &gBlankInputState );
		otherObjects = otherObjects->GetNext();
	}

	gInputtingEntityQueue.Add( entity );
	gInputtingEntityQueue.GetLast()->data->SetInputState( gInputState );
}

void UnbindEntityToInput( IInputtingObj* entity )
{
	CListEntry<IInputtingObj>* objects = gInputtingEntityQueue.GetFirst();
	while ( objects )
	{
		if ( entity == objects->data )
		{
			gInputtingEntityQueue.RemoveEntry( objects );
			entity->SetInputState( &gBlankInputState );
			break;
		}

		objects = objects->GetNext();
	}

	if ( gInputtingEntityQueue.GetLast() )
		gInputtingEntityQueue.GetLast()->data->SetInputState( gInputState );
}
