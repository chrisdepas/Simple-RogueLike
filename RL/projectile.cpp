#include "stdafx.h"
#include "projectile.h"
#include "dungeonmap.h"
#include "dungeonplayer.h"
#include "messagelist.h"

CProjectile::CProjectile( Vector2i Origin, Vector2f DirectionUnit, int Speed, int Damage, CGame* pGame, bool bHitLocalPlayer )
{
	m_iTextureID = pGame->m_TextureManager.GetID( ".\\Graphics\\DefaultBullet.tga" );
	m_vDirection = DirectionUnit;
	m_vOrigin = Origin;
	m_vLastPosition = Origin;
	m_vFloatPosition.X = (float)Origin.X;
	m_vFloatPosition.Y = (float)Origin.Y;
	m_iSpeed = Speed;
	m_iDamage = Damage;
	m_bDead = false;

	m_fLastUpdateTime = pGame->m_Time.Time();

	m_bFiredByPlayer = !bHitLocalPlayer;
}
void CProjectile::Update( CGame* pGame, CDungeonMap* pMap, CDungeonPlayer* pPlayer, CMessageList* pMessageList )
{
	/* If we are dead, don't continue updating */
	if ( m_bDead )
		return;

	float thistime = pGame->m_Time.Time();
	float delta = (thistime - m_fLastUpdateTime) * 200;
	if ( delta < 0.1f )
		return; 
	
	//Update every 0.1s

	Vector2f thisstep = m_vDirection * delta;
	m_fLastUpdateTime = thistime;
	m_vFloatPosition += thisstep;

	Vector2i CurPosition = GetPosition();

	if ( CurPosition != m_vLastPosition )
	{
		Vector2i PlayerPos = pPlayer->GetPosition();
		Vector2i delta = CurPosition - m_vLastPosition;
		Vector2f Ray( (float)delta.X, (float)delta.Y );
		Vector2f RayUnit = Ray / Ray.Length(); 
		Ray.X = (float)m_vLastPosition.X;
		Ray.Y = (float)m_vLastPosition.Y;

		/* Ray position rounded to nearest index */
		Vector2i CurIndex( (int)Ray.X, (int)Ray.Y );

		while ( CurIndex != CurPosition )
		{
			/* Round ray vector to nearest index */
			CurIndex = Vector2i( (int)floor(Ray.X + 0.5f), (int)floor(Ray.Y + 0.5f) );
		
			/* Check if we hit a wall */
			if ( pMap->WallAtPosition( CurIndex ) )
			{
				m_bDead = true;
				return;
			}

			/* Check if we hit a monster */
			CMonster* pMonster = pMap->GetMonsterAtTile( CurIndex );
			if ( pMonster )
			{
				/* damage the monster */
				pMonster->Damage( m_iDamage );
				m_bDead = true;

				/* Inform user that we hit the monster, if we can see that it did */
				if ( pMap->IsTileVisible( CurIndex ) )
				{
					char szMessage[256];
					sprintf_s( szMessage, "The %s is hit", pMonster->GetName() );
					pMessageList->AddMessage( szMessage );
					if ( pMonster->IsDead() )
					{
						sprintf_s( szMessage, "The %s dies", pMonster->GetName() );
						pMessageList->AddMessage( szMessage );
					}
				}
				else
				{
					//TODO: PLAY A SOUND HERE
				}
				
				return;
			}

			/* Check if ray hits player */
			if ( ShouldHitPlayer( pPlayer) )
			{
				if ( CurIndex == PlayerPos )
				{
					printf("\t\t-RayTracing hit the player at (%i, %i)\n", CurIndex.X, CurIndex.Y ); 
					char szMessage[256];
					sprintf_s( szMessage, "You are hit" );
					pMessageList->AddMessage(  szMessage );
					pPlayer->Damage( m_iDamage );
					m_bDead = true;
				}

			}

			/* Add a single step to the ray */
			Ray += RayUnit;
		}
	}
}
Vector2i CProjectile::GetPosition()
{
	/* Get position to nearest map tile index */
	int rx = (int)floor( m_vFloatPosition.X + 0.5f );
	int ry = (int)floor( m_vFloatPosition.Y + 0.5f );
	return Vector2i( rx, ry );
}
bool CProjectile::ShouldHitPlayer( CDungeonPlayer* pPlayer )
{
	/* Projectile was launched from player */
	return ( !m_bFiredByPlayer );
}
bool CProjectile::IsDead()
{
	return m_bDead;
}