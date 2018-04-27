#include "stdafx.h"
#include "inventory.h"
#include "game.h"
#include "LetterList.h"

bool CDungeonInventory::CanEquip( CItem* pItem )
{
	EItemType t = pItem->GetType();
	return ( t == IT_MeleeWeapon || t == IT_RangedWeapon );
}
void CDungeonInventory::OnInventoryChanged()
{
	/* update weapon letter */
	int letterlist_offset = 0;
	if ( m_pWeapon )
		m_pWeapon->SetLetter( g_LetterList[letterlist_offset++] );
	
	/* Recalculate weight & update inventory letters */
	m_fWeight = 0;
	for ( unsigned int i = 0; i < m_vItems.size(); i++ )
	{
		if ( m_vItems[i] )
		{
			m_fWeight += m_vItems[i]->GetWeight();

			assert( i+letterlist_offset < LETTERLIST_SIZE && "Letter list size exceeded, too many items in inventory" );
			m_vItems[i]->SetLetter( g_LetterList[i+letterlist_offset] );
		}
	}
}
CDungeonInventory::CDungeonInventory()
{
	m_fMaxWeight = DEFAULT_DUNGEON_INVENTORY_WEIGHT;
	m_fWeight = 0;
	m_pWeapon = 0;
}
CDungeonInventory::~CDungeonInventory()
{
	// Todo: destructor
	// should free resources and crap
	// will do later tho
}
float CDungeonInventory::GetWeight()
{
	return m_fWeight;
}
float CDungeonInventory::GetMaxWeight()
{
	return m_fMaxWeight;
}
void CDungeonInventory::SetMaxWeight( float weight )
{
	m_fMaxWeight = weight;
}
int CDungeonInventory::GetItemCount()
{
	return m_vItems.size();
}
bool CDungeonInventory::AddItem( CItem* pItem )
{
	if ( !pItem )
		return false;
	
	if ( Full() )
		return false;

	/* Check if it is light enough that we can carry it */
	float WeightWithItem = pItem->GetWeight() + GetWeight();
	if ( WeightWithItem > GetMaxWeight() )
		return false;
	
	/* Add to inventory */
	m_vItems.push_back( pItem );
	OnInventoryChanged();

	return true;
}
bool CDungeonInventory::RemoveItem( CItem* pItem )
{
	/* Search for this item and remove it */
	for ( unsigned int i = 0; i < m_vItems.size(); i++ )
	{
		if ( m_vItems[i] == pItem )
		{
			m_vItems.erase( m_vItems.begin() + i );
			OnInventoryChanged();
			return true;
		}
	}

	/* Item not found */
	return false;
}
bool CDungeonInventory::Full()
{
	int eqsize = 0;
	if ( m_pWeapon )
		eqsize++;
	return m_vItems.size()+eqsize >= LETTERLIST_SIZE;
}
bool CDungeonInventory::Empty()
{
	return m_vItems.size() == 0;
}
bool CDungeonInventory::EquipItem( CItem* pItem )
{
	if ( !pItem )
		return false;
	if ( !CanEquip( pItem ) )
		return false;
	
	EItemType it = pItem->GetType();
	if ( it == IT_MeleeWeapon || it == IT_RangedWeapon )
	{
		for ( unsigned int i = 0; i < m_vItems.size(); i++ )
		{
			if ( m_vItems[i] != pItem )
				continue;

			/* Item found in inventory */
			if ( m_pWeapon )
				m_vItems.push_back( m_pWeapon ); /* Already carrying a weapon, put in inventory */
			/* Equip to weapon slot and remove from inventory */
			m_pWeapon = pItem;
			m_vItems.erase( m_vItems.begin() + i );
			OnInventoryChanged();
			return true;
		}
		return false;
	}
	else
		return false; //TODO: Add more item types
}
char* CDungeonInventory::GetWeaponName()
{
	if ( !m_pWeapon )
		return 0;
	return m_pWeapon->GetName();
}
CItem* CDungeonInventory::GetWeapon()
{
	return m_pWeapon;
}
bool CDungeonInventory::IsHoldingRangedWeapon()
{
	CItem* pWeapon = GetWeapon();
	if ( !pWeapon )
		return false;
	return pWeapon->GetType() == IT_RangedWeapon;
}
bool CDungeonInventory::IsEquipped( CItem* pItem )
{
	return pItem == m_pWeapon;
}
int CDungeonInventory::ChooseItems( CGame* pGame, std::vector< CItem* >& items, char* szTask )
{
	int itemcount = 0;

	/* Loop until ESC is pressed */
	while ( true )
	{
		/* Init drawing */
		pGame->m_WindowManager.Begin();

		int offset = 10;

		/* Draw instructions */
		if ( szTask )
		{
			/* Special task name given, create instruction string */
			
			int tasklen = strlen( szTask );
			
			/* Allocate room for task and 'Select items to or press ESC to cancel.' string */
			char* msgstr = new char[tasklen + 42];
			sprintf_s( msgstr, tasklen + 42, "Select items to %s and press ESC to finish.", szTask );
			
			/* Draw to screen */
			pGame->m_Drawing.DrawText( &pGame->m_WindowManager, msgstr, 
				5, offset, DUNGEON_INVENTORY_TEXT_SIZE, 255, 255, 255, 255 );

			offset += DUNGEON_INVENTORY_TEXT_SIZE;

			/* Free allocated space */
			delete msgstr;
		}
		else
		{
			/* No special task */
			pGame->m_Drawing.DrawText( &pGame->m_WindowManager, "Select items and press ESC to finish.", 
				5, offset, DUNGEON_INVENTORY_TEXT_SIZE, 255, 255, 255, 255 );
		}

		/* Draw items */
		for ( unsigned int i = 0; i < m_vItems.size(); i++ )
		{
			if ( !m_vItems[i])
				continue;

			/* Print the letter of the item */
			char* letterstr = new char[2];
			sprintf_s( letterstr, 2, "%c", m_vItems[i]->GetLetter() );
			pGame->m_Drawing.DrawText( &pGame->m_WindowManager, letterstr, 5, offset, 
				DUNGEON_INVENTORY_TEXT_SIZE, 200, 255, 200, 255 );
			delete letterstr;

			/* Check if item is in selected items list */
			bool bSelected = false;
			for ( unsigned int j = 0; j < items.size(); j++ )
			{
				if ( m_vItems[i] == items[j] )
				{
					bSelected = true;
					break;
				}
			}

			/* Print the name of the item */
			if ( bSelected )
			{
				if ( m_vItems[i]->GetType() == IT_RangedWeapon )
				{
					char szMessage[256];
					sprintf_s( szMessage, 256, "%s (%i/%i)", m_vItems[i]->GetName(), m_vItems[i]->m_ExtraData.m_RangedWeaponData.m_iCurrentAmmo, 
						m_vItems[i]->m_ExtraData.m_RangedWeaponData.m_iAmmoCapacity );
					pGame->m_Drawing.DrawText( &pGame->m_WindowManager, szMessage, 5+DUNGEON_INVENTORY_TEXT_SIZE, 
						offset, DUNGEON_INVENTORY_TEXT_SIZE, 0, 255, 255, 255 );
				}
				else
					pGame->m_Drawing.DrawText( &pGame->m_WindowManager, m_vItems[i]->GetName(), 5+DUNGEON_INVENTORY_TEXT_SIZE, 
					offset, DUNGEON_INVENTORY_TEXT_SIZE, 0, 255, 255, 255 );

			}
			else
			{
				if ( m_vItems[i]->GetType() == IT_RangedWeapon )
				{
					char szMessage[256];
					sprintf_s( szMessage, 256, "%s (%i/%i)", m_vItems[i]->GetName(), m_vItems[i]->m_ExtraData.m_RangedWeaponData.m_iCurrentAmmo, 
						m_vItems[i]->m_ExtraData.m_RangedWeaponData.m_iAmmoCapacity );
					pGame->m_Drawing.DrawText( &pGame->m_WindowManager, szMessage, 5+DUNGEON_INVENTORY_TEXT_SIZE, 
						offset, DUNGEON_INVENTORY_TEXT_SIZE, 255, 255, 255, 255 );
				}
				else
					pGame->m_Drawing.DrawText( &pGame->m_WindowManager, m_vItems[i]->GetName(), 5+DUNGEON_INVENTORY_TEXT_SIZE, 
					offset, DUNGEON_INVENTORY_TEXT_SIZE, 255, 255, 255, 255 );
			}
			
			offset += DUNGEON_INVENTORY_TEXT_SIZE;
		}

		/* Present to screen */
		pGame->m_WindowManager.Present();

		/* Wait for letter input or ESC */
		char cletter;
		if ( !pGame->m_Input.GetChar( cletter, pGame ) )
			return itemcount; //

		/* Letter given, get item for this letter */
		CItem* pThisItem = GetItem( cletter );
		if ( pThisItem )
		{
			/* Check if this has been added already. If it has already, remove it */
			bool uq = false;
			for ( unsigned int i = 0; i < items.size(); i++ )
			{
				if ( pThisItem == items[i] )
				{
					uq = true;
					items.erase( items.begin() + i );
					itemcount--;
					break;
				}
			}
			/* If item has not been added, add to list */
			if ( !uq )
			{
				items.push_back( pThisItem );
				itemcount++;
			}
		}
	}
}


CItem* CDungeonInventory::ChooseItemEquip( CGame* pGame, char* szTask /*= 0*/ )
{
	CItem* pChosenItem = 0;
	while ( !pChosenItem )
	{
		/* Init drawing */
		pGame->m_WindowManager.Begin();

		int offset = 10;

		/* Draw instructions */
		if ( szTask )
		{
			/* Special task name given, create instruction string */
			
			int tasklen = strlen( szTask );
			
			/* Allocate room for task and 'Select item to or press ESC to cancel.' string */
			char* msgstr = new char[tasklen + 40];
			sprintf_s( msgstr, tasklen + 40, "Select item to %s or press ESC to cancel.", szTask );
			
			/* Draw to screen */
			pGame->m_Drawing.DrawText( &pGame->m_WindowManager, msgstr, 
				5, offset, DUNGEON_INVENTORY_TEXT_SIZE, 255, 255, 255, 255 );

			offset += DUNGEON_INVENTORY_TEXT_SIZE;

			/* Free allocated space */
			delete msgstr;
		}
		else
		{
			/* No special task */
			pGame->m_Drawing.DrawText( &pGame->m_WindowManager, "Select item, or press ESC to cancel.", 
				5, offset, DUNGEON_INVENTORY_TEXT_SIZE, 255, 255, 255, 255 );
		}

		/* Draw items */
		for ( unsigned int i = 0; i < m_vItems.size(); i++ )
		{
			if ( !m_vItems[i])
				continue;

			if ( !CanEquip( m_vItems[i] ) )
				continue;

			/* Print the letter of the item */
			char* letterstr = new char[2];
			sprintf_s( letterstr, 2, "%c", m_vItems[i]->GetLetter() );
			pGame->m_Drawing.DrawText( &pGame->m_WindowManager, letterstr, 5, offset, 
				DUNGEON_INVENTORY_TEXT_SIZE, 200, 255, 200, 255 );
			delete letterstr;

			/* Print the name of the item */
			if ( m_vItems[i]->GetType() == IT_RangedWeapon )
			{
				char szMessage[256];
				int ammo = m_vItems[i]->m_ExtraData.m_RangedWeaponData.m_iCurrentAmmo;
				int ammomax = m_vItems[i]->m_ExtraData.m_RangedWeaponData.m_iAmmoCapacity;
				sprintf_s( szMessage, 256, "%s (%i/%i)", m_vItems[i]->GetName(), ammo, ammomax );
				pGame->m_Drawing.DrawText( &pGame->m_WindowManager, szMessage, 5+DUNGEON_INVENTORY_TEXT_SIZE, 
					offset, DUNGEON_INVENTORY_TEXT_SIZE, 255, 255, 255, 255 );
			}
			else
				pGame->m_Drawing.DrawText( &pGame->m_WindowManager, m_vItems[i]->GetName(), 5+DUNGEON_INVENTORY_TEXT_SIZE, 
					offset, DUNGEON_INVENTORY_TEXT_SIZE, 255, 255, 255, 255 );
			
			offset += DUNGEON_INVENTORY_TEXT_SIZE;
		}

		/* Present to screen */
		pGame->m_WindowManager.Present();

		/* Wait for letter input or ESC */
		char cletter;
		if ( !pGame->m_Input.GetChar( cletter, pGame ) )
			return 0;

		/* Letter given, get item for this letter */
		pChosenItem = GetItem( cletter );
		if ( !pChosenItem || pChosenItem == m_pWeapon )
			continue;
		if ( !CanEquip( pChosenItem ) )
			pChosenItem = 0;
	}
	/* Item is found, return it */
	return pChosenItem;
}
CItem* CDungeonInventory::ChooseItem( CGame* pGame, EItemType type, char* szTask /*= 0*/ )
{
	CItem* pChosenItem = 0;
	while ( !pChosenItem )
	{
		/* Init drawing */
		pGame->m_WindowManager.Begin();

		int offset = 10;

		/* Draw instructions */
		if ( szTask )
		{
			/* Special task name given, create instruction string */
			
			int tasklen = strlen( szTask );
			
			/* Allocate room for task and 'Select item to or press ESC to cancel.' string */
			char* msgstr = new char[tasklen + 40];
			sprintf_s( msgstr, tasklen + 40, "Select item to %s or press ESC to cancel.", szTask );
			
			/* Draw to screen */
			pGame->m_Drawing.DrawText( &pGame->m_WindowManager, msgstr, 
				5, offset, DUNGEON_INVENTORY_TEXT_SIZE, 255, 255, 255, 255 );

			offset += DUNGEON_INVENTORY_TEXT_SIZE;

			/* Free allocated space */
			delete msgstr;
		}
		else
		{
			/* No special task */
			pGame->m_Drawing.DrawText( &pGame->m_WindowManager, "Select item, or press ESC to cancel.", 
				5, offset, DUNGEON_INVENTORY_TEXT_SIZE, 255, 255, 255, 255 );
		}

		/* Draw items */
		for ( unsigned int i = 0; i < m_vItems.size(); i++ )
		{
			if ( !m_vItems[i])
				continue;

			if ( m_vItems[i]->GetType() != type )
				continue;

			/* Print the letter of the item */
			char* letterstr = new char[2];
			sprintf_s( letterstr, 2, "%c", m_vItems[i]->GetLetter() );
			pGame->m_Drawing.DrawText( &pGame->m_WindowManager, letterstr, 5, offset, 
				DUNGEON_INVENTORY_TEXT_SIZE, 200, 255, 200, 255 );
			delete letterstr;

			/* Print the name of the item */
			if ( m_vItems[i]->GetType() == IT_RangedWeapon )
			{
				char szMessage[256];
				int ammo = m_vItems[i]->m_ExtraData.m_RangedWeaponData.m_iCurrentAmmo;
				int ammomax = m_vItems[i]->m_ExtraData.m_RangedWeaponData.m_iAmmoCapacity;
				sprintf_s( szMessage, 256, "%s (%i/%i)", m_vItems[i]->GetName(), ammo, ammomax );
				pGame->m_Drawing.DrawText( &pGame->m_WindowManager, szMessage, 5+DUNGEON_INVENTORY_TEXT_SIZE, 
					offset, DUNGEON_INVENTORY_TEXT_SIZE, 255, 255, 255, 255 );
			}
			else
			pGame->m_Drawing.DrawText( &pGame->m_WindowManager, m_vItems[i]->GetName(), 5+DUNGEON_INVENTORY_TEXT_SIZE, 
				offset, DUNGEON_INVENTORY_TEXT_SIZE, 255, 255, 255, 255 );
			
			offset += DUNGEON_INVENTORY_TEXT_SIZE;
		}

		/* Present to screen */
		pGame->m_WindowManager.Present();

		/* Wait for letter input or ESC */
		char cletter;
		if ( !pGame->m_Input.GetChar( cletter, pGame ) )
			return 0;

		/* Letter given, get item for this letter */
		pChosenItem = GetItem( cletter );
		if ( !pChosenItem )
			continue;
		if ( pChosenItem->GetType() != type )
			pChosenItem = 0;
	}
	/* Item is found, return it */
	return pChosenItem;
}
CItem* CDungeonInventory::ChooseItem( CGame* pGame, char* szTask /*= 0*/ )
{
	CItem* pChosenItem = 0;
	while ( !pChosenItem )
	{
		/* Init drawing */
		pGame->m_WindowManager.Begin();

		int offset = 10;

		/* Draw instructions */
		if ( szTask )
		{
			/* Special task name given, create instruction string */
			
			int tasklen = strlen( szTask );
			
			/* Allocate room for task and 'Select item to or press ESC to cancel.' string */
			char* msgstr = new char[tasklen + 40];
			sprintf_s( msgstr, tasklen + 40, "Select item to %s or press ESC to cancel.", szTask );
			
			/* Draw to screen */
			pGame->m_Drawing.DrawText( &pGame->m_WindowManager, msgstr, 
				5, offset, DUNGEON_INVENTORY_TEXT_SIZE, 255, 255, 255, 255 );

			offset += DUNGEON_INVENTORY_TEXT_SIZE;

			/* Free allocated space */
			delete msgstr;
		}
		else
		{
			/* No special task */
			pGame->m_Drawing.DrawText( &pGame->m_WindowManager, "Select item, or press ESC to cancel.", 
				5, offset, DUNGEON_INVENTORY_TEXT_SIZE, 255, 255, 255, 255 );
		}

		/* Draw items */
		for ( unsigned int i = 0; i < m_vItems.size(); i++ )
		{
			if ( !m_vItems[i])
				continue;

			/* Print the letter of the item */
			char* letterstr = new char[2];
			sprintf_s( letterstr, 2, "%c", m_vItems[i]->GetLetter() );
			pGame->m_Drawing.DrawText( &pGame->m_WindowManager, letterstr, 5, offset, 
				DUNGEON_INVENTORY_TEXT_SIZE, 200, 255, 200, 255 );
			delete letterstr;

			/* Print the name of the item */
			if ( m_vItems[i]->GetType() == IT_RangedWeapon )
			{
				char szMessage[256];
				int ammo = m_vItems[i]->m_ExtraData.m_RangedWeaponData.m_iCurrentAmmo;
				int ammomax = m_vItems[i]->m_ExtraData.m_RangedWeaponData.m_iAmmoCapacity;
				sprintf_s( szMessage, 256, "%s (%i/%i)", m_vItems[i]->GetName(), ammo, ammomax );
				pGame->m_Drawing.DrawText( &pGame->m_WindowManager, szMessage, 5+DUNGEON_INVENTORY_TEXT_SIZE, 
					offset, DUNGEON_INVENTORY_TEXT_SIZE, 255, 255, 255, 255 );
			}
			else
			pGame->m_Drawing.DrawText( &pGame->m_WindowManager, m_vItems[i]->GetName(), 5+DUNGEON_INVENTORY_TEXT_SIZE, 
				offset, DUNGEON_INVENTORY_TEXT_SIZE, 255, 255, 255, 255 );
			
			offset += DUNGEON_INVENTORY_TEXT_SIZE;
		}

		/* Present to screen */
		pGame->m_WindowManager.Present();

		/* Wait for letter input or ESC */
		char cletter;
		if ( !pGame->m_Input.GetChar( cletter, pGame ) )
			return 0;

		/* Letter given, get item for this letter */
		pChosenItem = GetItem( cletter );
	}
	/* Item is found, return it */
	return pChosenItem;
}

CItem* CDungeonInventory::GetItem( char cLetter )
{
	/* Check if the equipped weapon matchs letter */
	if ( m_pWeapon )
	{
		if ( m_pWeapon->GetLetter() == cLetter )
			return m_pWeapon;
	}

	/* Search inventory for an item with letter */
	for ( unsigned int i = 0; i < m_vItems.size(); i++ )
	{
		if ( !m_vItems[i] )
			continue;
		if ( m_vItems[i]->GetLetter() == cLetter )
			return m_vItems[i];
	}

	/* No items found with cLetter, return NULL */
	return NULL;
}