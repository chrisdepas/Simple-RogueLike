#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__

#include "vector.h"
#include "game.h"

class CDungeonMap;
class CDungeonPlayer;
class CMessageList;

class CProjectile
{
	friend class CDungeonMap;
protected:
	float m_fLastUpdateTime;

	bool	 m_bFiredByPlayer;
	int		 m_iTextureID;
	bool	 m_bDead;
	Vector2i m_vOrigin;
	Vector2i m_vLastPosition;
	Vector2f m_vFloatPosition;
	Vector2f m_vDirection;	/* Unit vector in direction of fire */
	int		 m_iSpeed;		/* Speed in squares per turn */
	int		 m_iDamage;		/* Damage in HP points */

public:

	CProjectile( Vector2i Origin, Vector2f DirectionUnit, int Speed, int Damage, CGame* pGame, bool bHitLocalPlayer );
	~CProjectile();
	/* Update projectiles. Returns true if it needs to be called again. */
	void Update( CGame* pGame, CDungeonMap* pMap, CDungeonPlayer* pPlayer, CMessageList* pMessageList );
	
	/* Gets current position on the 2D map*/
	Vector2i GetPosition();
	
	/* This projectile should hit a player */
	bool ShouldHitPlayer( CDungeonPlayer* pPlayer );
	bool IsDead();
};

#endif