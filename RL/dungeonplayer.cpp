#include "stdafx.h"
#include "dungeonplayer.h"
#include "monster.h"
#include "inventory.h"
#include "messagelist.h"
#include "dungeonmap.h"
#include "projectile.h"

#define GENERIC_HIT_SOUND ".\\Sounds\\GenericHit.wav"

void CDungeonPlayer::Init( CTextureManager* pTextureMgr )
{
	m_iTextureID = pTextureMgr->GetID( ".\\Graphics\\DefaultPlayer.tga" );
	m_iHealth = m_iMaxHealth = DEFAULT_PLAYER_MAX_HEALTH;
	
	m_iNutrition = DEFAULT_PLAYER_NUTRITION;

	m_szName = "Player";

	m_iMovementCost = DEFAULT_PLAYER_MOVEMENT_COST;
	m_iUnarmedDamage = 3;
}
void CDungeonPlayer::AttackMonster( CMonster* pMonster, CDungeonInventory* pPlayerInventory, CMessageList* pMessageList, CDungeonMap* pMap, CGame* pGame )
{
	if ( !pMonster || !pPlayerInventory || !pMessageList )
		return;
	
	char* szMonsterName = pMonster->GetName();
	CItem* pWeapon = pPlayerInventory->GetWeapon();
	if ( !szMonsterName )
		return;
	
	bool bBashWithWeapon = false;
	if ( pWeapon )
		bBashWithWeapon = pWeapon->GetType() == IT_RangedWeapon;

	if ( !pWeapon || bBashWithWeapon )
	{
		/* punch */
		pMonster->Damage( m_iUnarmedDamage );

		/* Time has passed */
		pMap->TurnsPassed( m_iMovementCost );
		
		/* output hit and damage */
		char msgstr[256];
		if ( !bBashWithWeapon )
			sprintf_s( msgstr, 256, "You hit the %s", szMonsterName );
		else
			sprintf_s( msgstr, 256, "You bash the %s with your %s", szMonsterName, pWeapon->GetName() );

		/* Play a hit sound */
		pGame->m_SoundManager.PlaySound( pGame->m_SoundManager.GetID( GENERIC_HIT_SOUND ) );

		pMessageList->AddMessage( msgstr );
		if ( pMonster->IsDead() )
		{
			sprintf_s( msgstr, 256, "The %s dies", szMonsterName );
			pMessageList->AddMessage( msgstr );
		}
		return;
	}
	
	char* szWeaponName = pWeapon->GetName();
	EItemType eWeaponType = pWeapon->GetType();

	switch ( eWeaponType )
	{
	case IT_MeleeWeapon:
		{
			int dam = pWeapon->m_ExtraData.m_MeleeWeaponData.m_iDamage;
			int ac  = pWeapon->m_ExtraData.m_MeleeWeaponData.m_iAttackCost;
			assert ( dam > 0 && ac > 0 );

			/* Damage monster */
			pMonster->Damage( dam );
			
			/* Play weapon sound */
			pGame->m_SoundManager.PlaySound( pWeapon->m_ExtraData.m_MeleeWeaponData.m_iAttackSoundID );

			/* Time has passed */
			pMap->TurnsPassed( ac );
			
			/* output hit and damage */
			char msgstr[256];
			sprintf_s( msgstr, 256, "You hit the %s with the %s", szMonsterName, szWeaponName );
			pMessageList->AddMessage( msgstr );
			if ( pMonster->IsDead() )
			{
				sprintf_s( msgstr, 256, "The %s dies", szMonsterName );
				pMessageList->AddMessage( msgstr );
			}
		}
		break;
	}

}

bool CDungeonPlayer::Fire( CDungeonInventory* pPlayerInventory, CMessageList* pMessageList, CDungeonMap* pMap, Vector2i vTarget, CGame* pGame )
{
	/* Check if we have a weapon */
	CItem* pWeapon = pPlayerInventory->GetWeapon();
	if ( !pWeapon )
		return false;
	
	/* Check if the weapon is ranged */
	if ( pWeapon->GetType() != IT_RangedWeapon )
		return false;

	/* Check if it has enough ammo to fire */
	if ( pWeapon->m_ExtraData.m_RangedWeaponData.m_iCurrentAmmo <= 0 )
	{
		pMessageList->AddMessage( "The gun is empty. " );
		// Todo: Make a click noise here
		return false;
	}

	/* Reduce ammo */
	pWeapon->m_ExtraData.m_RangedWeaponData.m_iCurrentAmmo--;
	char szMessage[256];
	sprintf_s( szMessage, "You fire the %s.", pWeapon->GetName() );
	pMessageList->AddMessage( szMessage );

	/* Play weapon sound */
	pGame->m_SoundManager.PlaySound( pWeapon->m_ExtraData.m_RangedWeaponData.m_iAttackSoundID );

	/* Spawn projectile */
	Vector2i Position = GetPosition();
	Vector2i Delta = vTarget - Position;
	float DeltaLen = Delta.Length();
	Vector2f Direction = Vector2f( (float)Delta.X/DeltaLen, (float)Delta.Y/DeltaLen );

	/* Spend time */
	pMap->TurnsPassed( pWeapon->m_ExtraData.m_RangedWeaponData.m_iAttackCost );

	CProjectile* pProj = new CProjectile( GetPosition(), Direction, 10, 10, pGame, false );
	pMap->AddProjectileToMap( pProj );


	return true;
}

void CDungeonPlayer::SetPosition( int X, int Y )
{
	m_vPosition.X = X;
	m_vPosition.Y = Y;
}
void CDungeonPlayer::SetPosition( Vector2i Position )
{
	m_vPosition = Position;
}
int CDungeonPlayer::GetTextureID()
{
	return m_iTextureID;
}
Vector2i CDungeonPlayer::GetPosition()
{
	return m_vPosition;
}
int CDungeonPlayer::GetMovementCost()
{
	return m_iMovementCost;
}
char* CDungeonPlayer::GetName()
{
	return m_szName;
}
void CDungeonPlayer::Damage( int Damage )
{
	m_iHealth -= Damage;
}
void CDungeonPlayer::Heal( int HealPoints )
{
	m_iHealth += HealPoints;
	if ( m_iHealth > m_iMaxHealth )
		m_iHealth = m_iMaxHealth;
}
float CDungeonPlayer::GetHealthPercentage()
{
	return (float)m_iHealth / m_iMaxHealth;
}
void CDungeonPlayer::AddNutrition( int Amt )
{
	m_iNutrition += Amt;
}
void CDungeonPlayer::SubtractNutrition( int Amt )
{
	m_iNutrition -= Amt;
}
char* CDungeonPlayer::GetNutritionStateName()
{
	if ( m_iNutrition > 8000 )
		return "Satiated";
	else if ( m_iNutrition > 6000 )
		return "Full";
	else if ( m_iNutrition > 4000 )
		return 0; // Dont display this one :)
	else if ( m_iNutrition > 2000 )
		return "Hungry";
	else if ( m_iNutrition > 0 )
		return "Famished";
	else if ( m_iNutrition > -2000 )
		return "Weak";
	return "Starving";
}
bool CDungeonPlayer::IsDead()
{
	return ( m_iHealth <= 0 );
}