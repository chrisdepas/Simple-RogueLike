#ifndef __WORLD_PLAYER_H__
#define __WORLD_PLAYER_H__

#include "vector.h"
#include "game.h"

class CWorldPlayer
{
	Vector2f m_vfPosition;
	float m_fRotation; 
	int m_iTextureID;
	float m_fMoveSpeed;
	
public:
	void LookAt( Vector2i MousePosition, CGame* pGame );
	void Initialise( CGame* pGame );
	
	void Draw( CGame* pGame );

	void SetPosition( Vector2f vPos );
	void SetPosition( Vector2i vPos );
	Vector2f GetPosition();
	Vector2i GetPositionI();
};

#endif