#ifndef _INPUTDATA_H
#define _INPUTDATA_H

#include "../common/list.h"
#include "../common/timer.h"
#include "SDL.h"

struct CMousePosition
{
	CMousePosition();

	int mMouseX, mMouseY;
	float mMouseXSp, mMouseYSp;
	int mMouseLastX, mMouseLastY;
	CTimer mouseTime;
};

struct CMouseButtons
{
	CMouseButtons();
};


/*	A Specific Status of the key, and its duration	*/
struct CKeyStatus
{
	bool status;
	CTimer duration;
};


/*	A single KeyPress, Used to track the order of different key presses	*/
struct CKey
{
	CKey();
	CKey( SDLKey k, int kNum );

	SDLKey keyCode;
	int keyNumber;
};


/*	Key State - Data for current status and last 3 previous statuses, as well as how long
	each one lasted			*/
class CKeyState
{
public:
	CKeyState( SDLKey k, int kNum );

	CKey key;
	CLinkedList<CKeyStatus> status;	// List of statuses of the key and times for each
};

#define MAX_KEY_STATUSES	6
#define MAX_KEY_PRESSES		8


/*
struct CPadButton
{
	int number;
	u8 buttonNumber;
	bool value;
};

struct CPadAxis
{
	int number;
	u8 axisNumber;
	u8 value;
};
*/

/*
struct CGamePadData
{
	void SetButton( u8 button, bool state );
	void SetAxis( u8 axis, u8 value );

	u8 padNumber;

	CLinkedList<CPadAxis>	axes;
	CLinkedList<CPadButton>	buttons;
};
*/

/*		Class that contains all relevant info for an object		*/
class CInputState
{
public:
	
	// Mouse Data:
	CMousePosition mousePosition;
	CMouseButtons mouseButtons;

	// Keyboard Data:
	CLinkedList<CKeyState> keys;
	CLinkedList<CKey> keyPresses;

	// Gamepad Data:
//	CGamePadData gamepad;

	bool IsKeyPressed( int keyNumber );

	// Pad:
//	bool IsButtonPressed( int buttonNumber );
//	int AxisValue( int axisNumber );
};



#endif

