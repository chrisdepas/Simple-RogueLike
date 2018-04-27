#include "stdafx.h"
#include "worldstate.h"
#include "game.h"

CWorldState CWorldState::m_WorldState;

void CWorldState::Init( CGame* pGame )
{
	m_iTileSize = 5;

	/* Generate a world */
	m_Map.Generate( pGame );

	/* Set spawn point */
	m_Player.SetPosition( m_Map.CalcSpawn() );
}
void CWorldState::Cleanup()
{
}

void CWorldState::Draw( CGame* pGame )
{
	/* Centre camera on player */
	pGame->m_WindowManager.MoveCamera( m_Player.GetPositionI(), m_iTileSize );

	m_Map.Draw( pGame, m_iTileSize );
}
void CWorldState::Update( CGame* pGame )
{
	m_Player.LookAt( pGame->m_Input.GetMousePosition(), pGame );
}
void CWorldState::HandleInput( CGame* pGame )
{
	EAction ThisAction = Action_Unknown;
	while ( (ThisAction = pGame->m_Input.GetNextAction()) != Action_None )
	{
		switch( ThisAction )
		{
		case Action_Move_Forward:
			break;
		case Action_Move_Back:
			break;
		case Action_Move_Left:
			break;
		case Action_Move_Right:
			break;
		default:
			break;
		}
	}
}
void CWorldState::PauseState()
{
}
void CWorldState::ResumeState()
{
}	
