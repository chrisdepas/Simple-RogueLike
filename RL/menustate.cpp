#include "stdafx.h"
#include "menustate.h"
#include "Dungeonstate.h"
#include "worldstate.h"
#include "game.h" 

CMenuState CMenuState::m_MenuState;

void CMenuState::Init( CGame* pGame )
{
}
void CMenuState::Cleanup()
{
}
void CMenuState::Draw( CGame* pGame )
{
	Vector2i Centre = pGame->m_WindowManager.GetScreenCentre();
	pGame->m_Drawing.DrawTextCentred( &pGame->m_WindowManager, "RL", Centre.X, Centre.Y - MENUSTATE_FONT_SIZE/2, 
		MENUSTATE_FONT_SIZE, 255, 255, 255, 255 );
	pGame->m_Drawing.DrawTextCentred( &pGame->m_WindowManager, "Press enter to play", Centre.X, Centre.Y + MENUSTATE_FONT_SIZE/2, 
		MENUSTATE_FONT_SIZE, 255, 255, 255, 255 );
}
void CMenuState::Update()
{
}
void CMenuState::HandleInput( CGame* pGame )
{
	EAction ThisAction = Action_Unknown;
	while ( (ThisAction = pGame->m_Input.GetNextAction()) != Action_None )
	{
		switch ( ThisAction )
		{
		case Action_Menu_Select:
			pGame->m_StateHandler.ChangeState( CDungeonState::Instance(), pGame );
		default:
			return;
		}
	}
}
void CMenuState::PauseState()
{
}
void CMenuState::ResumeState()
{
}
