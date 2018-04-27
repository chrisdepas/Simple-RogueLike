#ifndef __MONSTER_RAT_H__
#define __MONSTER_RAT_H__

#include "monster.h"

/* 'Vermin' type monsters, run away easily  
*/


class CGame;
class CMonsterVermin : public CMonster
{
	virtual EPlayerRelationship GetRelationshipToPlayer(); 
	int m_iTurnsSinceSeen;
	bool m_bHasSeenPlayer;
	Vector2i m_vLastSeenPlayerPosition;
public:
	CMonsterVermin();
	CMonsterVermin( CGame* pGame, char* szTextureName, char* szCorpseTexture, int iHealth, int iMovementCost, char* szName );
	~CMonsterVermin();

	virtual void Think( CDungeonMap* pMap, CDungeonPlayer* pPlayer, CGame* pGame );
	virtual CMonster* Copy(); 
}; 
#endif