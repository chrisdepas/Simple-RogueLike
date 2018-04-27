#include "stdafx.h"
#include "aimer.h"
#include "game.h"
#include "dungeonplayer.h"

CAimer::CAimer()
{
	m_bIsAiming = false;
}
void CAimer::Draw( CGame* pGame, CDungeonPlayer* pPlayer, int iTileSize )
{
	/* Not aiming - don't draw the aiming reticule */
	if ( !m_bIsAiming )
		return;

	if ( iTileSize <= 0 )
		return;

	Vector2i PlayerPos = pPlayer->GetPosition();

	if ( PlayerPos == m_vCurrentPosition )
	{
		pGame->m_Drawing.DrawSquare( &pGame->m_WindowManager, PlayerPos*iTileSize, 
			 		iTileSize, 255, 0, 0, 100 );
		return;
	}
	
	Vector2i delta = m_vCurrentPosition - PlayerPos;
	Vector2f Ray( (float)delta.X, (float)delta.Y );
	Vector2f RayUnit = Ray / Ray.Length(); 
	Ray.X = (float)PlayerPos.X;
	Ray.Y = (float)PlayerPos.Y;

	/* Ray position rounded to nearest index */
	Vector2i CurIndex( (int)Ray.X, (int)Ray.Y );

	/* Draw ray from player position to target */
	while ( CurIndex != m_vCurrentPosition )
	{
		/* Round ray vector to nearest index */
		CurIndex = Vector2i( (int)floor(Ray.X + 0.5f), (int)floor(Ray.Y + 0.5f) );
		
		/* Draw ray */
		pGame->m_Drawing.DrawSquare( &pGame->m_WindowManager, CurIndex*iTileSize, 
			 		iTileSize, 255, 0, 0, 100 );

		/* Add a single step to the ray */
		Ray += RayUnit;
	}
	
}
void CAimer::SetPosition ( Vector2i vPosition )
{
	m_vCurrentPosition = vPosition;
}
Vector2i CAimer::GetPosition() 
{
	return m_vCurrentPosition;
}
bool CAimer::IsAiming()
{
	return m_bIsAiming;
}

void CAimer::StartAiming( Vector2i Pos )
{
	m_bIsAiming = true;
	m_vCurrentPosition = Pos;
}
void CAimer::StopAiming()
{
	m_bIsAiming = false;
}
