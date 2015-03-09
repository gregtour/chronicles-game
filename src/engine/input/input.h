#ifndef _INPUT_H
#define _INPUT_H

#include "SDL.h"
#include "inputData.h"

class IInputtingObj;

void InputEvent( SDL_Event* e );
void BindEntityToInput( IInputtingObj* entity );
void UnbindEntityToInput( IInputtingObj* entity );

extern CLinkedList<CKey>	gKeyList;
extern CInputState*			gInputState;

#endif
