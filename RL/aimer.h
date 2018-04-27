#ifndef __AIMER_H__
#define __AIMER_H__

#include "vector.h"
class CDungeonPlayer;
class CGame;

class CAimer
{
protected:
	bool	 m_bIsAiming;
	Vector2i m_vCurrentPosition;
public:
	
	CAimer();
	void Draw( CGame* pGame, CDungeonPlayer* pPlayer, int iTileSize );
	void SetPosition ( Vector2i vPosition );
	Vector2i GetPosition();
	bool IsAiming();

	void StartAiming( Vector2i Pos );
	void StopAiming(); 
};
#endif