#ifndef __MENUSTATE_H__
#define __MENUSTATE_H__

#include "gamestate.h"
#define MENUSTATE_FONT_SIZE 24

class CMenuState : public CGameState
{
private:
	static CMenuState m_MenuState;

protected:
	CMenuState(){}
	CMenuState(CMenuState const&);
	void operator=(CMenuState const&);

public:
	void Init( CGame* pGame );
	void Cleanup();

	void Draw( CGame* pGame );
	void Update();
	void HandleInput( CGame* pGame );

	void PauseState();
	void ResumeState();

	static CMenuState* Instance()
	{
		return &m_MenuState;
	}
};

#endif