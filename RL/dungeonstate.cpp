#include "stdafx.h"
#include "Dungeonstate.h"
#include "game.h"
#include "menustate.h"
CDungeonState CDungeonState::m_DungeonState;

#define DEFAULT_DUNGEON_TILE_SIZE 16
#define MIN_DUNGEON_TILE_SIZE 2
#define MAX_DUNGEON_TILE_SIZE 20

void CDungeonState::Init( CGame* pGame )
{
	/* Initialise player */ 
	m_Player.Init( &pGame->m_TextureManager );
	
	/* Load items */
	if ( !LoadItemsFromItemList( m_LoadedDungeonItems, ".\\Items\\DungeonItems.idf", pGame ) )
	{
		MessageBoxA( 0, "Fatal: Unable to read items file at path .\\Items\\DungeonItems.idf", "Fatal Error", 0 );
		exit( -1 );
	}

	if ( !LoadMonstersFromMonsterList( m_LoadedDungeonMonsters, ".\\Monsters\\DungeonMonsters.idf", pGame ) )
	{
		MessageBoxA( 0, "Fatal: Unable to monsters file at path .\\Monsters\\DungeonMonsters.idf", "Fatal Error", 0 );
		exit( -1 );
	}


	/* Generate map and set player position */
	Vector2i Pos;
	m_Map.GenerateDungeon( pGame, Pos, m_LoadedDungeonItems, m_LoadedDungeonMonsters );
	m_Player.SetPosition( Pos );

	/* Update map visibility */
	m_Map.OnMove( Pos, &m_MessageList );

	m_iTileSize = DEFAULT_DUNGEON_TILE_SIZE;
}
void CDungeonState::Cleanup()
{
}

void CDungeonState::Draw( CGame* pGame )
{
	/* Centre camera */
	pGame->m_WindowManager.MoveCamera( m_Player.GetPosition(), m_iTileSize );

	/* Draw Map */
	m_Map.Draw( pGame, m_iTileSize );

	/* Draw Player */
	Vector2i PlayerPos = m_Player.GetPosition();
	pGame->m_Drawing.DrawSprite( &pGame->m_WindowManager, PlayerPos.X * m_iTileSize, PlayerPos.Y * m_iTileSize, 
		m_iTileSize, m_iTileSize, pGame->m_TextureManager.GetTexture( m_Player.GetTextureID() ) );

	/* Draw aiming line */
	m_Aimer.Draw( pGame, &m_Player, m_iTileSize );

	/* Restore camera */
	pGame->m_WindowManager.RestoreDefaultCamera();



	Vector2i Centre = pGame->m_WindowManager.GetScreenCentre();
	/* Draw list of game messages */
	m_MessageList.Draw( pGame );

	m_StatusBar.Draw( pGame, &m_Inventory, &m_Player );
}
void CDungeonState::Update( CGame* pGame )
{
	/* ToDo: fade to black/red then move to a new death screen state 
		also make it so player can die	*/
	if ( m_Player.IsDead() )
	{
		pGame->m_StateHandler.ChangeState( CMenuState::Instance(), pGame );
	}

	/* Allow monsters to think */
	m_Map.MonstersThink( &m_Player, pGame );

	/*  */
	m_Map.UpdateProjectiles( &m_Player, pGame, &m_MessageList );
}
void CDungeonState::HandleInput( CGame* pGame )
{
	/* Don't do anything while projectile is moving */
	if ( m_Map.BusyWithProjectiles() )
	{
		/* Ignore input while predicting projectiles */
		while ( pGame->m_Input.GetNextAction() != Action_None );
	}

	EAction ThisAction = Action_Unknown;

	/* If we are aiming, do aim state */
	if ( m_Aimer.IsAiming() )
	{
		while ( (ThisAction = pGame->m_Input.GetNextAction()) != Action_None )
		{
			switch ( ThisAction )
			{
				case Action_Move_Forward:
				{
					Vector2i Pos = m_Aimer.GetPosition();
					Pos.Y--;
					m_Aimer.SetPosition( Pos );
				}continue;
				case Action_Move_Back:
				{
					Vector2i Pos = m_Aimer.GetPosition();
					Pos.Y++;
					m_Aimer.SetPosition( Pos );
				}continue;
				case Action_Move_Left:
				{
					Vector2i Pos = m_Aimer.GetPosition();
					Pos.X--;
					m_Aimer.SetPosition( Pos );
				}continue;
				case Action_Move_Right:
				{
					Vector2i Pos = m_Aimer.GetPosition();
					Pos.X++;
					m_Aimer.SetPosition( Pos );
				}
				continue;
				case Action_Fire:
				{
					m_Aimer.StopAiming();
					if ( m_Player.GetPosition() != m_Aimer.GetPosition() )
					{
						m_Player.Fire( &m_Inventory, &m_MessageList, &m_Map, m_Aimer.GetPosition(), pGame );
					}
				}
				continue;
				case Action_Menu_Toggle:
					m_Aimer.StopAiming();
					continue;
			}
		}
		return;
	}

	/* We aren't aiming - parse all actions */
	while ( (ThisAction = pGame->m_Input.GetNextAction()) != Action_None )
	{
		switch ( ThisAction )
		{
		case Action_Menu_Toggle:
		case Action_Menu_Select:
		default:
			continue;

		case Action_Move_Forward:
			{
				Vector2i Pos = m_Player.GetPosition();
				Pos.Y--;
				if ( m_Map.CanMoveInto( Pos ) )
				{
					m_Player.SetPosition( Pos );
					m_Map.OnMove( Pos, &m_MessageList );
					m_Player.SubtractNutrition( m_Player.GetMovementCost() );
					m_Map.TurnsPassed( m_Player.GetMovementCost() );
				}
				else
				{
					CMonster* pMonster = m_Map.GetMonsterAtTile( Pos );
					m_Player.AttackMonster( pMonster, &m_Inventory, &m_MessageList, &m_Map, pGame );
				}
			}continue;
		case Action_Move_Back:
			{
				Vector2i Pos = m_Player.GetPosition();
				Pos.Y++;
				if ( m_Map.CanMoveInto( Pos ) )
				{
					m_Player.SetPosition( Pos );
					m_Map.OnMove( Pos, &m_MessageList );
					m_Player.SubtractNutrition( m_Player.GetMovementCost() );
					m_Map.TurnsPassed( m_Player.GetMovementCost() );
				}
				else
				{
					CMonster* pMonster = m_Map.GetMonsterAtTile( Pos );
					m_Player.AttackMonster( pMonster, &m_Inventory, &m_MessageList, &m_Map, pGame );
				}
			}continue;
		case Action_Move_Left:
			{
				Vector2i Pos = m_Player.GetPosition();
				Pos.X--;
				if ( m_Map.CanMoveInto( Pos ) )
				{
					m_Player.SetPosition( Pos );
					m_Map.OnMove( Pos, &m_MessageList );
					m_Player.SubtractNutrition( m_Player.GetMovementCost() );
					m_Map.TurnsPassed( m_Player.GetMovementCost() );
				}
				else
				{
					CMonster* pMonster = m_Map.GetMonsterAtTile( Pos );
					m_Player.AttackMonster( pMonster, &m_Inventory, &m_MessageList, &m_Map, pGame );
				}
			}continue;
		case Action_Move_Right:
			{
				Vector2i Pos = m_Player.GetPosition();
				Pos.X++;
				if ( m_Map.CanMoveInto( Pos ) )
				{
					m_Player.SetPosition( Pos );
					m_Map.OnMove( Pos, &m_MessageList );
					m_Player.SubtractNutrition( m_Player.GetMovementCost() );
					m_Map.TurnsPassed( m_Player.GetMovementCost() );
				}
				else
				{
					CMonster* pMonster = m_Map.GetMonsterAtTile( Pos );
					m_Player.AttackMonster( pMonster, &m_Inventory, &m_MessageList, &m_Map, pGame );
				}
			}continue;
		
		case Action_Move_Down:
			{
			m_iTileSize--;
			if ( m_iTileSize < MIN_DUNGEON_TILE_SIZE )
				m_iTileSize = MIN_DUNGEON_TILE_SIZE;
			continue;
			}
		case Action_Move_Up:
			{
				m_iTileSize++;
				if ( m_iTileSize > MAX_DUNGEON_TILE_SIZE )
					m_iTileSize = MAX_DUNGEON_TILE_SIZE;
				continue;
			}
		case Action_Grab:
			{
				m_Map.Grab( m_Player.GetPosition(), &m_Inventory, pGame );
			}
			continue;
		case Action_Drop:
			{
				if ( m_Inventory.Empty() )
					continue;
				CItem* pItem = m_Inventory.ChooseItem( pGame, "drop" );
				if ( !pItem )
					continue; 
				if ( m_Inventory.RemoveItem( pItem ) )
					m_Map.Drop( m_Player.GetPosition(), pItem );
			}
			continue;
		case Action_Eat:
			{
				if ( m_Inventory.Empty() )
					continue;
				CItem* pItem = m_Inventory.ChooseItem( pGame, IT_Food, "eat" );
				if ( !pItem )
					continue;
				if ( m_Inventory.RemoveItem( pItem ) )
				{
					/* Display eating */
					char* eatname = pItem->GetName();
					if ( !eatname )
						continue;
					int eatlen = strlen( eatname );
					char* msgstr = new char[eatlen + 18];
					sprintf_s( msgstr, eatlen + 18, "You consume the %s.", eatname );
					m_MessageList.AddMessage( msgstr );
					
					/* Give the player nutrition from the food */
					m_Player.AddNutrition( pItem->m_ExtraData.m_FoodData.m_iNutrition );
					
					/* Free memory */
					delete pItem; 
					delete msgstr;
				}
			}
			continue;
		case Action_Equip:
			{
				if ( m_Inventory.Empty() )
					continue;
				CItem* pItem = m_Inventory.ChooseItemEquip( pGame, "equip" );
				if ( !pItem )
					continue;
				if ( m_Inventory.EquipItem( pItem ) )
				{
					char* eqname = pItem->GetName();
					if ( !eqname )
						continue;
					int eqlen = strlen( eqname );
					char* msgstr = new char[eqlen + 16];
					sprintf_s( msgstr, eqlen + 16, "You equip the %s.", eqname );
					m_MessageList.AddMessage( msgstr );
					delete msgstr;
				}
				else
				{
					m_MessageList.AddMessage( "You are not able to equip this item." );
				}
			}
			continue;
		case Action_Wait:
			{
				m_Player.SubtractNutrition( m_Player.GetMovementCost() );
				m_Map.TurnsPassed( m_Player.GetMovementCost() );
			}
			continue;
		case Action_Fire:
			{
				if ( !m_Inventory.IsHoldingRangedWeapon() )
					continue;
				m_Aimer.StartAiming( m_Player.GetPosition() );
			}
			continue;
		}
	}
}
void CDungeonState::PauseState()
{
}
void CDungeonState::ResumeState()
{
}
