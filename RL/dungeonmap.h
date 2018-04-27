#ifndef __DUNGEON_MAP_H__
#define __DUNGEON_MAP_H__

class CItem;
class CGame;
class CDungeonMap;
class CMessageList;
class CDungeonInventory;
class CProjectile;

#include <vector>
#include "vector.h"
#include "item.h"
#include "inventory.h"
#include "monster.h"

#define DEFAULT_DUNGEON_FOV 10.0f
enum EDungeonFlags
{
	FL_Discovered	= 1 << 0, /* Player has seen this tile */
	FL_Passable		= 1 << 1, /* Player can walk through this tile */
	FL_Transparent	= 1 << 2, /* Player can see through this tile */
	FL_Visible		= 1 << 3, /* Player can see this tile */
};
enum ESpecialFlags
{
	FL_DownStairs,
	FL_UpStairs,
};
class CDungeonTile
{
	friend class CDungeonMap;
protected:
	int m_iFlags; /* EDungeonFlags */
	int m_iTextureID; /* Texture to draw as */
	int m_iSpecialID; /* Function, e.g. stairs, fountain, sink etc */
public:
	CDungeonTile();
	CDungeonTile( int iTextureID, int iSpecialID, int iFlags );
	void SetFlag( EDungeonFlags Flag );
	void RemoveFlag( EDungeonFlags Flag );
	bool FlagIsSet( EDungeonFlags Flag );
};
#define DUNGEON_WIDTH 128
#define DUNGEON_HEIGHT 128
class CDungeonMap
{

	int m_iHeight;
	int m_iWidth;
	CDungeonTile m_Map[DUNGEON_WIDTH][DUNGEON_HEIGHT];
	std::vector< CItem* > m_vItems;
	std::vector< CMonster* > m_vMonsters;
	std::vector< CProjectile* > m_vProjectiles;
public:

	CDungeonMap();
	~CDungeonMap();

	/* Draw this level */
	void Draw( CGame* pGame, int TileSize );

	/* Procedurally generate this level */
	void GenerateDungeon( CGame* pGame, Vector2i& PlayerStartPos, std::vector< CItem* >& m_LoadedDungeonItems, std::vector< CMonster* >& m_LoadedDungeonMonsters ); /* Where stair x,y is the position of the downward stairs, entrance to this level */
	void GenerateDungeon( CGame* pGame );

	/*  */
	bool WallAtPosition( Vector2i Position );
	bool CanMoveInto( Vector2i Position, CDungeonPlayer* pPlayer );
	bool CanMoveInto( Vector2i Position );
	bool InBounds( Vector2i Position );
	
	/* Check visibility with calculation */
	bool CalcVisibleTile( Vector2i Origin, Vector2i Tile );
	/* Check visibility flag of tile */
	bool IsTileVisible( Vector2i Tile );
	/* Update visibility */
	void UpdateVisible( Vector2i Origin, Vector2i Tile, float FOV );

	void OnMove( Vector2i Position, CMessageList* pMessageList );

	/* Pick up item(s) */
	void Grab( Vector2i Position, CDungeonInventory* pInventory, CGame* pGame );
	/* Drop a single item */
	void Drop( Vector2i Position, CItem* pItem );
	
	bool GetNearestItem( Vector2i Position, CItem*& pRetItem, float& fRetDistance );

	/* Called when player has spent turns, all monsters are given these turns */
	void TurnsPassed( int iNumTurns );

	/* Monsters spend moves here */
	void MonstersThink( CDungeonPlayer* pPlayer, CGame* pGame );

	/* Get monster at a position */
	CMonster* GetMonsterAtTile( Vector2i vTilePosition );
	CMonster* GetFirstMonsterInRay( Vector2i Origin, Vector2i Final );

	bool RayHitsPlayer( Vector2i Origin, Vector2i Final, CDungeonPlayer* pPlayer );

	bool BusyWithProjectiles();
	void UpdateProjectiles( CDungeonPlayer* pPlayer, CGame* pGame, CMessageList* pMessageList );
	void CDungeonMap::AddProjectileToMap( CProjectile* pProjectile );
};

#endif