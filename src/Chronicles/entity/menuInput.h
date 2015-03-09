#ifndef _MENUINPUT_H
#define _MENUINPUT_H

#include "../../engine/entity/entity.h"
#include "../../engine/entity/inputtingObj.h"

class CMenuInput: public CEntity, public IInputtingObj
{
public:
	CMenuInput();

	void Update( float );
	CTimer t;
};

#endif
