#ifndef __DUNGEONSTATE_H__
#define __DUNGEONSTATE_H__

#include "gamestate.h"
#include "dungeonmap.h"
#include "dungeonplayer.h"
#include "messagelist.h"
#include "inventory.h"
#include "StatusBar.h"
#include "aimer.h"
class CDungeonState : public CGameState
{
private:
	static CDungeonState m_DungeonState;

	CDungeonPlayer m_Player;
	int m_iTileSize; 
	CDungeonMap m_Map;
	CMessageList m_MessageList;
	CDungeonInventory m_Inventory;
	CDungeonStatusBar m_StatusBar;
	CAimer m_Aimer;

	/* A list containing one instance of all loaded items */
	std::vector< CItem* > m_LoadedDungeonItems;

	/* A list containing one instance of all loaded monsters */
	std::vector< CMonster* > m_LoadedDungeonMonsters;

protected:
	CDungeonState(){}
	CDungeonState(CDungeonState const&);
	void operator=(CDungeonState const&);

public:
	void Init( CGame* pGame );
	void Cleanup();

	void Draw( CGame* pGame );
	void Update( CGame* pGame );
	void HandleInput( CGame* pGame );

	void PauseState();
	void ResumeState();

	static CDungeonState* Instance()
	{
		return &m_DungeonState;
	}
};

#endif