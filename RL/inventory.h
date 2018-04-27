#ifndef __INVENTORY_H__
#define __INVENTORY_H__

#include "item.h"
#include <vector>

#define DEFAULT_DUNGEON_INVENTORY_WEIGHT 40.0f
#define DUNGEON_INVENTORY_TEXT_SIZE 16
class CDungeonInventory
{
	float m_fWeight;	/* Kilograms */
	float m_fMaxWeight;

	CItem* m_pWeapon;	/* Held weapon, does not count for weight */

	std::vector < CItem* > m_vItems;

	/* Update weight and item letters */
	void OnInventoryChanged();
	bool CanEquip( CItem* pItem );
public:
	
	CDungeonInventory();
	~CDungeonInventory();

	float GetWeight();
	float GetMaxWeight();
	void SetMaxWeight( float weight );

	int	  GetItemCount();
	bool AddItem( CItem* pItem );
	bool RemoveItem( CItem* pItem );
	
	/* Returns true if inventory has no more 'slots' (free letters) */
	bool Full();
	/* Returns true if inventory has no items*/
	bool Empty();

	/* Equip item */
	bool EquipItem( CItem* );
	/* Equip an item letter */
	bool EquipItem( char cLetter );
	
	/**/
	char* GetWeaponName();
	CItem* GetWeapon();
	
	/* Is holding a ranged weapon */
	bool IsHoldingRangedWeapon();
	/* Check if an item is equipped */
	bool IsEquipped( CItem* pItem );

	/* Open inventory screen and choose items */
	int CDungeonInventory::ChooseItems( CGame* pGame, std::vector< CItem* >& items, char* szTask = 0 );
	/* Open inventory screen and choose a single item */
	CItem* ChooseItem( CGame* pGame, char* szTask = 0 );
	/* Choose single item by type */
	CItem* ChooseItem( CGame* pGame, EItemType type, char* szTask = 0 );
	/* choose item you can equip */
	CItem* ChooseItemEquip( CGame* pGame, char* szTask = 0 );

	/* Get an inventory item from a letter */
	CItem* GetItem( char cLetter );
};

#endif