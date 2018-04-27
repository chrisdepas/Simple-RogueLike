#ifndef __DUNGEONPLAYER_H__
#define __DUNGEONPLAYER_H__

#include "vector.h"
#include "texture.h"

class CMonster;
class CDungeonInventory;
class CMessageList;
class CDungeonMap;
class CGame;

#define DEFAULT_PLAYER_NUTRITION 5000
#define DEFAULT_PLAYER_MAX_HEALTH 25
#define DEFAULT_PLAYER_MOVEMENT_COST 6

class CDungeonPlayer
{
	Vector2i m_vPosition;
	int m_iTextureID;
	int m_iMovementCost;
	
	int m_iUnarmedDamage;

	/* Condition */
	int m_iHealth;
	int m_iMaxHealth;
	
	/* */
	int m_iNutrition;
	
	/* Name */
	char* m_szName;

public:
	void Init( CTextureManager* pTextureMgr );
	void SetPosition( int X, int Y );
	void SetPosition( Vector2i Position );
	int GetTextureID();
	Vector2i GetPosition();
	
	/* Physically attack monster */
	void AttackMonster( CMonster* pMonster, CDungeonInventory* pPlayerInventory, CMessageList* pMessageList, CDungeonMap* pMap, CGame* pGame );
	
	/* Fire a weapon at an intended position. Returns if it was able to fire */
	bool Fire( CDungeonInventory* pPlayerInventory, CMessageList* pMessageList, CDungeonMap* pMap, Vector2i vTarget, CGame* pGame );
	
	int GetMovementCost();

	char* GetName();

	/* Decrease health */
	void Damage( int Damage );
	/* Increase health */
	void Heal( int HealPoints );
	/* get percent health */
	float GetHealthPercentage();

	/* Lose some nutrition*/
	void SubtractNutrition( int Amt );
	/* Add nutrition */
	void AddNutrition( int Amt );
	/* Get state of nutrition */
	char* GetNutritionStateName();
	
	bool IsDead();
};
#endif