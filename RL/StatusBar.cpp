#include "stdafx.h"

#include "StatusBar.h"
#include "game.h"
#include "inventory.h"
#include "dungeonplayer.h"


void CDungeonStatusBar::Draw( CGame* pGame, CDungeonInventory* pInventory, CDungeonPlayer* pPlayer )
{
	float MaxWeight   = pInventory->GetMaxWeight();
	float CurWeight   = pInventory->GetWeight();
	char* szNutrition = pPlayer->GetNutritionStateName();
	float HPPercent   = pPlayer->GetHealthPercentage();
	char* szName	  = pPlayer->GetName();
	CItem* pWeapon	  = pInventory->GetWeapon();

	Vector2i ScrDimensions = pGame->m_WindowManager.GetScreenDimensions();
	int y = ScrDimensions.Y - DUNGEON_STATUS_BAR_TEXT_SIZE * 2 - 4;

	/* draw top layer of status bar */
	pGame->m_Drawing.DrawText( &pGame->m_WindowManager, szName, 5, y, DUNGEON_STATUS_BAR_TEXT_SIZE, 255, 255, 255, 255 );  
	
	if ( pWeapon )
	{
		if ( pWeapon->GetType() == IT_RangedWeapon )
		{
			char szMessage[256];
			int ammo = pWeapon->m_ExtraData.m_RangedWeaponData.m_iCurrentAmmo;
			int ammomax = pWeapon->m_ExtraData.m_RangedWeaponData.m_iAmmoCapacity;
			sprintf_s( szMessage, 256, "%s (%i/%i)", pWeapon->GetName(), ammo, ammomax );
			pGame->m_Drawing.DrawTextCentredX( &pGame->m_WindowManager, szMessage, ScrDimensions.X/2+1, 
				y, DUNGEON_STATUS_BAR_TEXT_SIZE, 255, 255, 255, 255 );
		}
		else
			pGame->m_Drawing.DrawTextCentredX( &pGame->m_WindowManager, pWeapon->GetName(), ScrDimensions.X/2+1, 
			y, DUNGEON_STATUS_BAR_TEXT_SIZE, 255, 255, 255, 255 );
	}
	/*	Draw bottom layer of status bar */
	y += DUNGEON_STATUS_BAR_TEXT_SIZE;

	/* Health */
	char hpstr[250];
	sprintf_s( hpstr, 250, "Health: %3.2f", HPPercent*100 );
	pGame->m_Drawing.DrawText( &pGame->m_WindowManager, hpstr, 5, y, DUNGEON_STATUS_BAR_TEXT_SIZE, 255, 255, 255, 255 );  

	/* Nutrition */
	if ( szNutrition )
		pGame->m_Drawing.DrawTextCentredX( &pGame->m_WindowManager, szNutrition, ScrDimensions.X/2, y, DUNGEON_STATUS_BAR_TEXT_SIZE, 255, 255, 255, 255 );

}