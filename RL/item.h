#ifndef __ITEM_H__
#define __ITEM_H__

#include <vector>
#include "vector.h"

class CDungeonMap;
class CGame;
class CItem;
class CMonster;

enum EItemType
{
	IT_None, 
	IT_Junk, /* No specific function */
	IT_Food, /* Edible */
	IT_Ammo, /* Ammunition for a weapon */
	IT_MeleeWeapon, /* Melee weapon e.g. pipe, sword */
	IT_RangedWeapon, /* Ranged weapon e.g. crossbow, pistol */
};
enum EAmmoType
{
	AT_BB,   //  BBs for BB guns
	AT_PT22, // .22
	AT_PT45, // .45
	AT_PT308,// .308
	AT_PT357,// .357
	AT_PT44, // .44 magnum
	AT_PT50, // .50 mg
	AT_5MM,  //  5mm
	AT_9MM,  //  9mm 
	AT_10MM, //  10mm
	AT_5PT56,//  5.56mm
	AT_12GA, //  12 gauge shotgun shell
	AT_20GA, //  20 gauge shotgun shell
};

EItemType GetItemTypeFromString( char* szItemType );
bool LoadItemsFromItemList( std::vector< CItem* >& vItems, char* szItemListPath, CGame* pGame );

class CItem
{
	friend class CMonster;
	friend class CDungeonMap;

protected:
	bool m_bValid;
	int m_iTextureID;
	Vector2i m_vLocation;
	EItemType m_eType;
	char* m_szName;
	char* m_szDescription;
	float m_fWeight;
	char m_Letter;

public:
	union
	{
		uint64_t __edata; /* DWORD*4, short*8 */
		struct 
		{
			short m_iNutrition;
			short m_iServings;
		} m_FoodData;
		struct 
		{
			short m_iAmmoCount; /* Amount of ammo, should stack */
			EAmmoType m_eAmmoType; 
		} m_AmmoData;
		struct 
		{
			short m_iDamage;
			short m_iAttackCost; /* 'speed' */
			short m_iAttackSoundID; 
		} m_MeleeWeaponData;
		struct
		{
			short m_eAmmoType;		/* Type of ammo which can be loaded into this weapon */
			short m_iAmmoCapacity;  /* Maximum amount of ammo which can be loaded at one time */
			short m_iCurrentAmmo;   /* Current amount of ammo in the weapon */

			short m_eAttackType;	/* Type of attack (TODO: ADD TYPES SUCH AS LOADANDFIRE, LAUNCHER, SINGLESHOT_PROJECTILE) */
			short m_iAttackCost;	/* time to attack in turns */
			//short m_iRange;			/* range in squares */
			short m_iAttackSoundID;
			short m_iReloadCost;	/* Time to reload in turns */			
			
			short m_iExtraDamage;	/* Bullet damage = AmmoDamage+ExtraDamage */
		} m_RangedWeaponData;
	} m_ExtraData;

	EItemType GetType();
	void SetLetter( char cLetter );
	char GetLetter();
	bool IsEdible();
	bool IsValid();
	float GetWeight();
	char* GetName();
	CItem* Copy( CGame* pGame );
	void SetPosition( int X, int Y );
	void SetPosition( Vector2i Pos );
	Vector2i GetLocation();

	/* Parse an item file from ini */
	CItem( CGame* pGame , char* szItemFile );

	CItem();
	~CItem();
	
};
#endif