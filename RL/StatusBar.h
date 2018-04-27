#ifndef __STATUSBAR_H__
#define __STATUSBAR_H__

class CGame;
class CDungeonInventory;
class CDungeonPlayer;
#define DUNGEON_STATUS_BAR_TEXT_SIZE 16

class CDungeonStatusBar
{
public:
	void Draw( CGame* pGame, CDungeonInventory* pInventory, CDungeonPlayer* pPlayer );
};


#endif