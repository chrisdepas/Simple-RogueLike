#ifndef __MONSTER_RUDY_H__
#define __MONSTER_RUDY_H__

#include "monster.h"
/*
	pet
	barks at items
	attacks hostile creatures
*/

#define MIN_RUDY_DISTANCE 5
class CGame;

class CMonsterRudy : public CMonster
{
	virtual EPlayerRelationship GetRelationshipToPlayer();
	int m_iTurnsSinceSeen;
	bool m_bHasSeenPlayer;
	bool m_bLookingForItems;
	Vector2i m_vLastSeenPlayerPosition;
public:
	CMonsterRudy( CGame* pGame );
	~CMonsterRudy();
	virtual void Think( CDungeonMap* pMap, CDungeonPlayer* pPlayer, CGame* pGame );
}; 

#endif