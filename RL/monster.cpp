#include "stdafx.h"
#include "monster.h"
#include "item.h"
#include "game.h"
#include "monster_vermin.h"

EPlayerRelationship CMonster::GetRelationshipToPlayer()
{
	return Relationship_Enemy;
}
bool CMonster::IsPlayerPet()
{
	return GetRelationshipToPlayer() == Relationship_Pet;
}
int CMonster::GetMovementCost()
{
	return m_iMovementCost;
}
char* CMonster::GetName()
{
	return m_szName;
}
Vector2i CMonster::GetPosition()
{
	return m_vPosition;
}
void CMonster::SetPosition( Vector2i Pos)
{
	m_vPosition = Pos;
}
void CMonster::Damage( int iAmt )
{
	m_iHealth -= iAmt;
}
void CMonster::Heal( int iAmt )
{
	m_iHealth += iAmt;
}
bool CMonster::IsDead()
{
	return m_iHealth <= 0;
}
void CMonster::GiveTurns( int iAmt )
{
	m_iTurns += iAmt;
}
void CMonster::Think( CDungeonMap* pMap, CDungeonPlayer* pPlayer, CGame* pGame )
{
	MessageBoxA( 0, "Fatal Error", "Fatal: Think() called on base monster. ", 0 );
	exit( -1 );
}
CItem* CMonster::MakeCorpse( CGame* pGame )
{
	CItem* pCorpse = new CItem();
	pCorpse->m_eType = IT_Junk;
	pCorpse->m_fWeight = 10.0f; ////// HACKHACK
	pCorpse->m_iTextureID = m_iCorpseTextureID;

	/* create name of corpse */
	int namlen = strlen( m_szName ); 
	pCorpse->m_szName = new char[namlen+8];
	sprintf_s( pCorpse->m_szName, namlen+8, "%s corpse", m_szName );

	/* create description of corpse */
	pCorpse->m_szDescription = new char[namlen+18];
	sprintf_s( pCorpse->m_szDescription, namlen+18, "The remains of a %s", m_szName );

	pCorpse->m_bValid = true;

	return pCorpse;
}

#include <fstream>
/* UTIL: Make a monster from a monster ini file */
CMonster* MakeMonsterFromFile( char* szMonsterFile, CGame* pGame )
{
	pGame->m_IniReader.ParseFile( szMonsterFile );
	
	/* Load type name */
	char* szType = 0;
	if ( !pGame->m_IniReader.GetStringValue( "Type", szType) )
		return 0;
	
	/* Load texture name */
	char* szTexture = 0;
	if ( !pGame->m_IniReader.GetStringValue( "Texture", szTexture) )
		return 0;

	/* Load corpse texture name */
	char* szCorpseTexture = 0;
	if ( !pGame->m_IniReader.GetStringValue( "CorpseTexture", szCorpseTexture) )
		return 0;

	/* Load name */
	char* szName = 0;
	if ( !pGame->m_IniReader.GetStringValue( "Name", szName ) )
		return 0;

	/* Load health */
	int iHealth = 0;
	if ( !pGame->m_IniReader.GetIntValue( "Health", iHealth ) )
		return 0;

	/* Load movement cost */
	int iMovementCost = 0;
	if ( !pGame->m_IniReader.GetIntValue( "MovementCost", iMovementCost ) )
		return 0;

	/* Load attack cost */
	int iAttackCost = 0;
	if ( !pGame->m_IniReader.GetIntValue( "AttackCost", iAttackCost ) )
		return 0;

	/* Load damage */
	int iDamage = 0;
	if ( !pGame->m_IniReader.GetIntValue( "Damage", iAttackCost ) )
		return 0;

	/* Create unique type of monster */
	if ( !_strcmpi( szType, "Vermin" ) )
	{
		/* Create a copy of the name */
		int namelen = strlen( szName );
		char* szNameCpy = new char[namelen+1];
		memcpy( szNameCpy, szName, namelen );
		szNameCpy[namelen] = 0;

		CMonster* pMonster = new CMonsterVermin( pGame, szTexture, szCorpseTexture, iHealth, iMovementCost, szNameCpy );
		return pMonster;
	}
//	else if ( !_strcmpi ( szType, "" ) );
	
	/* Unknown type */
	return 0;
}

/* UTIL: Populates a list of items from a file listing monster files */
bool LoadMonstersFromMonsterList( std::vector< CMonster* >& vMonsters, char* szMonsterListPath, CGame* pGame )
{
	std::ifstream file;
	file.open( szMonsterListPath );

	if ( !file.is_open() || file.bad() )
		return false;
	
	char thisline[1024];

	while ( file.getline( thisline, 1024 ) )
	{
		/* Skip section headings or comment lines*/
		if ( thisline[0] == '[' || thisline[0] == ';' || thisline[0] =='#' )
			continue;

		/* Load monster from file */
		CMonster* pNewMonster = MakeMonsterFromFile( thisline, pGame );
		if ( !pNewMonster )
			continue;

		vMonsters.push_back( pNewMonster );
	}
	return vMonsters.size() > 0;
}
CMonster* CMonster::Copy()
{
	MessageBoxA( 0, "Tried to call Copy() on non-specific monster", "Fatal", 0 );
	exit( -1 );
}