#include "stdafx.h"
#include "item.h"
#include "game.h"
#include <fstream>

//Chance that a random weapon spawns with ammo 
#define RANGED_WEAPON_AMMO_CHANCE 1

bool LoadItemsFromItemList( std::vector< CItem* >& vItems, char* szItemListPath, CGame* pGame )
{
	std::ifstream file;
	file.open( szItemListPath );

	if ( !file.is_open() || file.bad() )
		return false;
	
	char thisline[1024];

	while ( file.getline( thisline, 1024 ) )
	{
		/* Skip section headings or comment lines*/
		if ( thisline[0] == '[' || thisline[0] == ';' || thisline[0] =='#' )
			continue;

		/* Load item from file */
		CItem* NewItem = new CItem( pGame, thisline );

		/* Don't add invalid items */
		if ( !NewItem->IsValid() )
		{
			delete NewItem;
			continue;
		}

		/* Item is valid, load */
		vItems.push_back( NewItem );
	}
	return vItems.size() > 0;
}

EItemType GetItemTypeFromString( char* szItemType )
{
	if ( !_stricmp( szItemType, "Food" ) )
		return IT_Food;
	if ( !_stricmp( szItemType, "Junk" ) )
		return IT_Junk;
	if ( !_stricmp( szItemType, "Ammo" ) )
		return IT_Ammo;
	if ( !_stricmp( szItemType, "MeleeWeapon" ) )
		return IT_MeleeWeapon;
	if ( !_stricmp( szItemType, "RangedWeapon" ) )
		return IT_RangedWeapon;
	return IT_None;
}

EItemType CItem::GetType()
{
	return m_eType;
}
void CItem::SetLetter( char cLetter )
{
	m_Letter = cLetter;
}
char CItem::GetLetter()
{
	return m_Letter;
}
bool CItem::IsValid()
{
	return m_bValid;
}
float CItem::GetWeight()
{
	return m_fWeight;
}
char* CItem::GetName()
{
	return m_szName;
}

CItem* CItem::Copy( CGame* pGame )
{
	/* Can't copy invalid items */
	if ( !m_bValid )
		return 0;

	/* Copy over */
	CItem* ret = new CItem;
	memcpy( ret, this, sizeof(CItem) );
	
	/* Copy strings, so freeing one item doesnt free name of all */
	int namelen = strlen( ret->m_szName );
	ret->m_szName = (char*)malloc( namelen+1 );
	memcpy( ret->m_szName, m_szName, namelen+1 );

	int desclen = strlen( ret->m_szDescription );
	ret->m_szDescription = (char*)malloc( desclen+1 );
	memcpy( ret->m_szDescription, m_szDescription, desclen+1 );

	/* Special case : weapon spawns with random ammo */
	if ( GetType() == IT_RangedWeapon )
	{
 		if ( pGame->m_Random.RandFloat() < RANGED_WEAPON_AMMO_CHANCE )
			m_ExtraData.m_RangedWeaponData.m_iCurrentAmmo = pGame->m_Random.RandInt( 1, m_ExtraData.m_RangedWeaponData.m_iAmmoCapacity );
		else
			m_ExtraData.m_RangedWeaponData.m_iCurrentAmmo = 0;
	}

	return ret;
}
void CItem::SetPosition( int X, int Y )
{
	m_vLocation.X = X;
	m_vLocation.Y = Y;
}
void CItem::SetPosition( Vector2i vPos )
{
	m_vLocation = vPos;
}
Vector2i CItem::GetLocation()
{
	return m_vLocation;
}
CItem::CItem( CGame* pGame, char* szItemFile )
{
	m_bValid = false;
	m_szName = 0;
	m_szDescription = 0;

	pGame->m_IniReader.ParseFile( szItemFile );
	
	/* Load texture */
	char* texture = 0;
	if ( !pGame->m_IniReader.GetStringValue( "Texture", texture) || !texture )
		return;
	m_iTextureID = pGame->m_TextureManager.GetID( texture );

	/* Load type */
	char* type = 0;
	if ( !pGame->m_IniReader.GetStringValue( "Type", type) || !type )
		return;
	m_eType = GetItemTypeFromString( type );

	/* Load name */
	char* name = 0;
	if ( !pGame->m_IniReader.GetStringValue( "Name", name) || !name )
		return;
	/* Name valid - copy to class (as inireader name will be free'd) */
	int namelen = strlen( name );
	m_szName = new char[namelen + 1];
	memcpy( m_szName, name, namelen );
	m_szName[namelen] = 0;

	/* Load description */
	char* desc = 0;
	if ( !pGame->m_IniReader.GetStringValue( "Description", desc) || !desc )
		return;
	int desclen = strlen( desc );
	m_szDescription = new char[desclen + 1];
	memcpy( m_szDescription, desc, desclen);
	m_szDescription[desclen] = 0;

	/* Load weight */
	float weight;
	if ( !pGame->m_IniReader.GetFloatValue( "Weight", weight ) )
		return;
	m_fWeight = weight;

	/* Type specific stuff */
	switch ( m_eType )
	{
	case IT_Food:
		{
			int nutrition = 0;
			if ( !pGame->m_IniReader.GetIntValue( "Nutrition", nutrition ) )
				return;
			m_ExtraData.m_FoodData.m_iNutrition = nutrition;
		}
		break;
	case IT_Ammo:
		{
			int ammocount = 0;
			if ( !pGame->m_IniReader.GetIntValue( "AmmoCount", ammocount ) )
				return;
			m_ExtraData.m_AmmoData.m_iAmmoCount = ammocount;
		}
		break;
	case IT_MeleeWeapon:
		{
			int dam = 0;
			if ( !pGame->m_IniReader.GetIntValue( "Damage", dam ) )
				return;
			m_ExtraData.m_MeleeWeaponData.m_iDamage = dam;
			int ac = 0;
			if ( !pGame->m_IniReader.GetIntValue( "AttackCost", ac ) )
				return;
			m_ExtraData.m_MeleeWeaponData.m_iAttackCost = ac;
			
			char* szSoundName;
			if ( !pGame->m_IniReader.GetStringValue( "AttackSound", szSoundName ) || !szSoundName )
				return;
			m_ExtraData.m_MeleeWeaponData.m_iAttackSoundID = pGame->m_SoundManager.GetID( szSoundName );
		}
		break;
	case IT_RangedWeapon:
		{
			int ammoc = 0;
			if ( !pGame->m_IniReader.GetIntValue( "AmmoCapacity", ammoc ) )
				return;
			m_ExtraData.m_RangedWeaponData.m_iAmmoCapacity = ammoc;

			int rc = 0;
			if ( !pGame->m_IniReader.GetIntValue( "ReloadCost", rc ) )
				return;
			m_ExtraData.m_RangedWeaponData.m_iReloadCost = rc;

			/*int range = 0;
			if ( !pGame->m_IniReader.GetIntValue( "Range", range ) )
				return;
			m_ExtraData.m_RangedWeaponData.m_iRange = range;*/

			int ac = 0;
			if ( !pGame->m_IniReader.GetIntValue( "AttackCost", ac ) )
				return;
			m_ExtraData.m_RangedWeaponData.m_iAttackCost = ac;

			char* szSoundName;
			if ( !pGame->m_IniReader.GetStringValue( "AttackSound", szSoundName ) || !szSoundName )
				return;
			m_ExtraData.m_RangedWeaponData.m_iAttackSoundID = pGame->m_SoundManager.GetID( szSoundName );


			m_ExtraData.m_RangedWeaponData.m_iCurrentAmmo = 0;
		}
		break;
	};

	/* All needed data found, item is valid */
	m_bValid = true;
}
CItem::CItem()
{
	m_bValid = false;
	m_szName = 0;
	m_szDescription = 0;
}
CItem::~CItem()
{ 
	if ( m_szName )
		delete m_szName;
	if ( m_szDescription )
		delete m_szDescription;
}