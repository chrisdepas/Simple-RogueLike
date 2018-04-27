#include "stdafx.h"
#include "monster_rudy.h"
#include "game.h"
#include "dungeonmap.h"
#include "dungeonplayer.h"

EPlayerRelationship CMonsterRudy::GetRelationshipToPlayer()
{
	return Relationship_Pet;
}
CMonsterRudy::CMonsterRudy( CGame* pGame )
{
	m_iPlayerRelationshipPts = 100;
	m_iTurnsSinceSeen = 0;
	m_bHasSeenPlayer = false;
	m_bLookingForItems = false;

	m_iTextureID = pGame->m_TextureManager.GetID( ".\\Graphics\\Rudy.tga" );
	m_iCorpseTextureID = INVALID_TEXTURE_ID;

	m_iHealth = 10000000;
	m_iMaxHealth = 10;
	m_iMovementCost = 3;
	m_iTurns = 0;
	m_szName = "Rudy";
}
void CMonsterRudy::Think( CDungeonMap* pMap, CDungeonPlayer* pPlayer, CGame* pGame )
{
	while ( m_iTurns > 0 )
	{
		bool bPlayerVisible = pMap->CalcVisibleTile( m_vPosition, pPlayer->GetPosition() );
		if ( bPlayerVisible )
		{
			m_iTurnsSinceSeen = 0;
			m_bHasSeenPlayer = true;
			m_vLastSeenPlayerPosition = pPlayer->GetPosition();
		}
		else
		{
			m_iTurnsSinceSeen++;
			if ( m_iTurnsSinceSeen > 100 ) 
			{
				/* Forget about player */
				m_bHasSeenPlayer = false;
			}
		}

		/* If hasn't seen player, sit and wait */
		if ( !m_bHasSeenPlayer )
		{
			m_iTurns = 0;
			return;
		}
		
		/* Has seen player. stay close & in sight, bark at items, have swag */

		float lastplayerdist = 100000;

		bool bCanMove = false;
		Vector2i NewPos( -1, -1 );

		Vector2i LeftPos = m_vPosition;
		LeftPos.X -= 1;
		if ( pMap->CanMoveInto( LeftPos, pPlayer ) )
		{
			Vector2i Delta = LeftPos - m_vLastSeenPlayerPosition;
			float DeltaLen = Delta.Length();
			
			if ( DeltaLen < lastplayerdist )
			{
				lastplayerdist = DeltaLen;
				NewPos = LeftPos;
				bCanMove = true;
			}
		}

		Vector2i RightPos = m_vPosition;
		RightPos.X += 1;
		if ( pMap->CanMoveInto( RightPos, pPlayer ) )
		{
			Vector2i Delta = RightPos - m_vLastSeenPlayerPosition;
			float DeltaLen = Delta.Length();
				
			if ( DeltaLen < lastplayerdist )
			{
				lastplayerdist = DeltaLen;
				NewPos = RightPos;
				bCanMove = true;
			}
		}

		Vector2i UpPos = m_vPosition;
		UpPos.Y += 1;
		if ( pMap->CanMoveInto( UpPos, pPlayer ) )
		{
			Vector2i Delta = UpPos - m_vLastSeenPlayerPosition;
			float DeltaLen = Delta.Length();
				
			if ( DeltaLen < lastplayerdist )
			{
				lastplayerdist = DeltaLen;
				NewPos = UpPos;
				bCanMove = true;
			}
		}

		Vector2i DownPos = m_vPosition;
		DownPos.Y -= 1;
		if ( pMap->CanMoveInto( DownPos, pPlayer ) )
		{
			Vector2i Delta = DownPos - m_vLastSeenPlayerPosition;
			float DeltaLen = Delta.Length();
				
			if ( DeltaLen < lastplayerdist )
			{
				lastplayerdist = DeltaLen;
				NewPos = DownPos;
				bCanMove = true;
			}
		}
					
		if ( bCanMove && ( !bPlayerVisible || lastplayerdist > MIN_RUDY_DISTANCE ) )
		{
			SetPosition( NewPos );
		}
		else
		{
			/* Within min distance - now look for food */
			CItem* pLookItem;
			float fDist;
			if ( pMap->GetNearestItem( m_vPosition, pLookItem, fDist ) )
			{
				if (fDist < MIN_RUDY_DISTANCE)
				{
					if ( !m_bLookingForItems )
					{
						m_bLookingForItems = true;
						// TODO: Bark here
					}
					Vector2i Delta = pLookItem->GetLocation() - m_vPosition;
					Vector2f dir = Vector2f( Delta.X / Delta.Length(), Delta.Y / Delta.Length() );
					Vector2i MoveDir( 0, 0 );
					if ( dir.X*dir.X > dir.Y*dir.Y )
					{
						// move in x direction
						if ( dir.X < 0 )
							MoveDir.X = -1;
						else
							MoveDir.X = 1;
					}
					else
					{
						// move in y direction
						if ( dir.Y < 0 )
							MoveDir.Y = -1;
						else
							MoveDir.Y = 1;
					}
					MoveDir.X += m_vPosition.X;
					MoveDir.Y += m_vPosition.Y;

					if ( pMap->CanMoveInto( MoveDir ) )
						SetPosition( MoveDir );
				}
			}
		}
		/* If we can't move for some reason, waste turns  */
		m_iTurns -= GetMovementCost();
	}
}