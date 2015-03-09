#include "inputData.h"


CMousePosition::CMousePosition()
{
	mMouseX = 0;
	mMouseY = 0;
	mMouseXSp = 0.0f;
	mMouseYSp = 0.0f;
	mMouseLastX = 0;
	mMouseLastY = 0;
}

CMouseButtons::CMouseButtons()
{
}

CKey::CKey( SDLKey k, int kNum )
{
	this->keyCode = k;
	this->keyNumber = kNum;
}

CKey::CKey()
{

}

CKeyState::CKeyState( SDLKey k, int kNum )
{
	key.keyCode = k;
	key.keyNumber = kNum;
}

/*	Do a quick search for the key and see if it is currently pressed	*/
bool CInputState::IsKeyPressed( int keyNumber )
{
	CListEntry<CKeyState>* key = keys.GetFirst();
	while ( key && key->data->key.keyNumber != keyNumber )
		key = key->GetNext();

	if ( key )
	{
		if ( key->data->status.GetLast() && key->data->status.GetLast()->data->status )
		{
			return true;
		}
	}

	return false;
}
/*
bool CInputState::IsButtonPressed( int buttonNumber )
{
	CListEntry<CPadButton>* button = gamepad.buttons.GetFirst();
	while ( key && key->data->key.keyNumber != keyNumber )
		key = key->GetNext();

	if ( key )
	{
		if ( key->data->status.GetLast() && key->data->status.GetLast()->data->status )
		{
			return true;
		}
	}

	return false;
}
*/
/*
	int AxisValue( int axisNumber );
*/