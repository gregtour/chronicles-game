#include "menuInput.h"
#include "../game/gameInput.h"
#include "../main.h"
#include "../game/game.h"
#include "../../engine/engine.h"

CMenuInput::CMenuInput()
{
	t.Clear();
	t.Step();
}

void CMenuInput::Update( float dt )
{
	if ( t.Time() > 2.5f )
	{
		if ( mInputState->IsKeyPressed(K_SWING) )
		{
			gLevelNumber = 0;
			gPlayerScore = 0;
			ChangeGameState( NEXT_LEVEL_STATE );
		}
		else if ( mInputState->IsKeyPressed(K_CONTINUE) && gPlayerScore >= POINTS_TO_CONTINUE )
		{
			gLevelNumber--;
			gPlayerScore -= POINTS_TO_CONTINUE;
			ChangeGameState( NEXT_LEVEL_STATE );
		}
	}
}
