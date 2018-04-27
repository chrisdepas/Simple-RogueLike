#ifndef __GAMESTATE_H__
#define __GAMESTATE_H__

#include "drawing.h"
#include "texture.h"
class CGame;
class CGameState;

/*	Handle game states and allow switching
	TODO: Possibly make states have a stack,
	So you can return etc
*/
class CGameStateHandler
{
	/* Current game state */
	CGameState* m_State;
public:
	CGameStateHandler();
	~CGameStateHandler();

	/* Returns the current game state*/
	CGameState* GetCurrentState();
	
	/* Changes to specified state */
	void ChangeState( CGameState* State, CGame* pGame );

	void Draw( CGame* pGame );
	void Update( CGame* pGame );
	void HandleInput( CGame* pGame );
};

/*	A state in the game, such as main menu
	Handled by CGameStateHandler
*/
class CGameState
{
public:
	CGameState* m_pgsReturnState;
	void SetReturnState( CGameState* pReturnState );
	virtual void Init( CGame* pGame );
	virtual void Cleanup();

	virtual void Draw( CGame* pGame );
	virtual void Update( CGame* pGame );
	virtual void HandleInput( CGame* pGame );

	void ChangeState( CGameState* pState, CGame* pGame );
	virtual void PauseState();
	virtual void ResumeState();
};


#endif