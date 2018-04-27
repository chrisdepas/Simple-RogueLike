#include "stdafx.h"
#include "monster_vermin.h"
#include "game.h"
#include "dungeonmap.h"
#include "dungeonplayer.h"

EPlayerRelationship CMonsterVermin::GetRelationshipToPlayer()
{
	if ( m_iPlayerRelationshipPts > 10 )
		return Relationship_Pet;
	return Relationship_Scared;
}
CMonsterVermin::CMonsterVermin()
{
}
CMonsterVermin::CMonsterVermin( CGame* pGame, char* szTexture, char* szCorpseTexture, int iHealth, int iMovementCost, char* szName )
{
	m_iPlayerRelationshipPts = 10;
	m_iTurnsSinceSeen = 0;
	m_bHasSeenPlayer = false;

	m_iTextureID = pGame->m_TextureManager.GetID( szTexture );
	m_iCorpseTextureID = pGame->m_TextureManager.GetID( szCorpseTexture );

	m_iHealth = iHealth;
	m_iMaxHealth = iHealth;
	m_iMovementCost = iMovementCost;
	m_iTurns = 0;
	m_szName = szName;
}
void CMonsterVermin::Think( CDungeonMap* pMap, CDungeonPlayer* pPlayer, CGame* pGame )
{
	while ( m_iTurns > 0 )
	{
		if ( pMap->CalcVisibleTile( m_vPosition, pPlayer->GetPosition() ) )
		{
			m_iTurnsSinceSeen = 0;
			m_bHasSeenPlayer = true;
			m_vLastSeenPlayerPosition = pPlayer->GetPosition();
		}
		else
		{
			m_iTurnsSinceSeen++;
			if ( m_iTurnsSinceSeen > 20 ) 
			{
				/* Forget about player */
				m_bHasSeenPlayer = false;
			}
		}

		if ( !m_bHasSeenPlayer ) 
		{
			/* Hasn't seen player, move randomly or something */
			int dir = pGame->m_Random.RandInt( 0, 3 );
			switch ( dir )
			{
			case 0:
				{ // Left
					Vector2i ProjPos = m_vPosition;
					ProjPos.X -= 1;
					if ( pMap->CanMoveInto( ProjPos, pPlayer ) )
						SetPosition( ProjPos );
					m_iTurns -= GetMovementCost();
				}
				break;
			case 1:
				{ // Right
					Vector2i ProjPos = m_vPosition;
					ProjPos.X += 1;
					if ( pMap->CanMoveInto( ProjPos, pPlayer ) )
						SetPosition( ProjPos );
					m_iTurns -= GetMovementCost();
				}
				break;
			case 2:
				{ // Up
					Vector2i ProjPos = m_vPosition;
					ProjPos.Y += 1;
					if ( pMap->CanMoveInto( ProjPos, pPlayer ) )
						SetPosition( ProjPos );
					m_iTurns -= GetMovementCost();
				}
				break;
			case 3:
				{ // Down
					Vector2i ProjPos = m_vPosition;
					ProjPos.Y -= 1;
					if ( pMap->CanMoveInto( ProjPos, pPlayer ) )
						SetPosition( ProjPos );
					m_iTurns -= GetMovementCost();
				}
				break;
			}
		}
		else
		{
			/* Has seen player -> Run away */

			float lastplayerdist = 0;
			if ( IsPlayerPet() )
			{
				lastplayerdist = 100000;
			}

			bool bCanMove = false;
			Vector2i NewPos( -1, -1 );

			Vector2i LeftPos = m_vPosition;
			LeftPos.X -= 1;
			if ( pMap->CanMoveInto( LeftPos, pPlayer ) )
			{
				Vector2i Delta = LeftPos - m_vLastSeenPlayerPosition;
				float DeltaLen = Delta.Length();
				if ( !IsPlayerPet() )
				{
					if ( DeltaLen > lastplayerdist )
					{
						lastplayerdist = DeltaLen;
						NewPos = LeftPos;
						bCanMove = true;
					}
				}
				else
				{
					if ( DeltaLen < lastplayerdist )
					{
						lastplayerdist = DeltaLen;
						NewPos = LeftPos;
						bCanMove = true;
					}
				}
			}

			Vector2i RightPos = m_vPosition;
			RightPos.X += 1;
			if ( pMap->CanMoveInto( RightPos, pPlayer ) )
			{
				Vector2i Delta = RightPos - m_vLastSeenPlayerPosition;
				float DeltaLen = Delta.Length();
				if ( !IsPlayerPet() )
				{
					if ( DeltaLen > lastplayerdist )
					{
						lastplayerdist = DeltaLen;
						NewPos = RightPos;
						bCanMove = true;
					}
				}
				else
				{
					if ( DeltaLen < lastplayerdist )
					{
						lastplayerdist = DeltaLen;
						NewPos = RightPos;
						bCanMove = true;
					}
				}
			}

			Vector2i UpPos = m_vPosition;
			UpPos.Y += 1;
			if ( pMap->CanMoveInto( UpPos, pPlayer ) )
			{
				Vector2i Delta = UpPos - m_vLastSeenPlayerPosition;
				float DeltaLen = Delta.Length();
				if ( !IsPlayerPet() )
				{
					if ( DeltaLen > lastplayerdist )
					{
						lastplayerdist = DeltaLen;
						NewPos = UpPos;
						bCanMove = true;
					}
				}
				else
				{
					if ( DeltaLen < lastplayerdist )
					{
						lastplayerdist = DeltaLen;
						NewPos = UpPos;
						bCanMove = true;
					}
				}
			}

			Vector2i DownPos = m_vPosition;
			DownPos.Y -= 1;
			if ( pMap->CanMoveInto( DownPos, pPlayer ) )
			{
				Vector2i Delta = DownPos - m_vLastSeenPlayerPosition;
				float DeltaLen = Delta.Length();
				if ( !IsPlayerPet() )
				{
					if ( DeltaLen > lastplayerdist )
					{
						lastplayerdist = DeltaLen;
						NewPos = DownPos;
						bCanMove = true;
					}
				}
				else
				{
					if ( DeltaLen < lastplayerdist )
					{
						lastplayerdist = DeltaLen;
						NewPos = DownPos;
						bCanMove = true;
					}
				}
			}
					
			if ( bCanMove )
				SetPosition( NewPos );
			
			/* If we can't move for some reason, waste turns */
			m_iTurns -= GetMovementCost();
		}
	}
}

CMonster* CMonsterVermin::Copy()
{
	/* Copy most values */
	CMonsterVermin* ret = new CMonsterVermin;
	memcpy( ret, this, sizeof(CMonsterVermin) );
	
	/* Copy name, so freeing one item doesnt free name of all */
	int namelen = strlen( m_szName );
	ret->m_szName = (char*)malloc( namelen+1 );
	memcpy( ret->m_szName, m_szName, namelen+1 );

	return ret;

}
