#ifndef __MONSTER_H__
#define __MONSTER_H__

#include "vector.h"
#include <vector>

class CDungeonMap;
class CDungeonPlayer;
class CGame;
class CItem;
class CMonster;

/* same factions don't attack each other*/
enum EMonsterFaction
{
	Faction_Wildlife,
};

enum EPlayerRelationship
{
	Relationship_Pet,
	Relationship_Friend,
	Relationship_Enemy,
	Relationship_Ignore,
	Relationship_Scared,
};

bool LoadMonstersFromMonsterList( std::vector< CMonster* >& vMonsters, char* szMonsterListPath, CGame* pGame );

/* Abstract monster class
	Inherit from this and write monster croden
*/

class CMonster
{
	friend class CDungeonMap;

protected:
	Vector2i m_vPosition;
	int m_iTextureID;
	int m_iCorpseTextureID;
	int m_iMovementCost;
	char* m_szName;

	/* */
	int m_iPlayerRelationshipPts;

	/* Condition */
	int m_iHealth;
	int m_iMaxHealth;

	/* Pool of points for moves */
	int m_iTurns;

public:
	/*CMonster();
	~CMonster();*/

	virtual EPlayerRelationship GetRelationshipToPlayer();
	
	bool IsPlayerPet();

	int GetMovementCost();

	char* GetName();

	Vector2i GetPosition();
	void SetPosition( Vector2i Pos );

	void Damage( int amt );
	void Heal( int amt );
	bool IsDead();

	/* Add turns to the pool */
	void GiveTurns( int amt );
	
	/* Think and do moves. Spends points from the pool */
	virtual void Think( CDungeonMap* pMap, CDungeonPlayer* pPlayer, CGame* pGame );

	/* Create a corpse */
	CItem* MakeCorpse( CGame* pGame );
	
	/* Get a copy of this monster */
	virtual CMonster* Copy(); 
};

#endif