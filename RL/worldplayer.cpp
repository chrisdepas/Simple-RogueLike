#include "stdafx.h"
#include "worldplayer.h"


#define PLAYER_TEXTURE ".\\Graphics\\Player.tga"
void CWorldPlayer::LookAt( Vector2i MousePosition, CGame* pGame )
{
	/* Player is centred at all times */

}
void CWorldPlayer::Initialise( CGame* pGame )
{
	m_iTextureID = pGame->m_TextureManager.GetID( PLAYER_TEXTURE );
}
void CWorldPlayer::SetPosition( Vector2f vPos )
{
	m_vfPosition = vPos;
}
void CWorldPlayer::SetPosition( Vector2i vPos )
{
	m_vfPosition.X = (float)vPos.X;
	m_vfPosition.Y = (float)vPos.Y;
}
Vector2f CWorldPlayer::GetPosition()
{
	return m_vfPosition;
}
Vector2i CWorldPlayer::GetPositionI()
{
	return Vector2i( (int)m_vfPosition.X, (int)m_vfPosition.Y );
}