#include "stdafx.h"
#include "dungeonmap.h"
#include "game.h"
#include "messagelist.h"
#include "inventory.h"
#include "monster_rudy.h"
#include "monster_vermin.h"
#include "dungeonplayer.h"
#include "projectile.h"

CDungeonTile::CDungeonTile(){}

CDungeonTile::CDungeonTile( int iTextureID, int iSpecialID, int iFlags )
{
	m_iFlags = iFlags;
	m_iTextureID = iTextureID;
	m_iSpecialID  = iSpecialID;
}

void CDungeonTile::SetFlag( EDungeonFlags Flag )
{
	m_iFlags |= Flag;
}
void CDungeonTile::RemoveFlag( EDungeonFlags Flag )
{
	m_iFlags &= ~Flag;
}
bool CDungeonTile::FlagIsSet( EDungeonFlags Flag )
{
	return ( (m_iFlags & Flag) != 0 );
}
 
CDungeonMap::CDungeonMap()
{
	m_iHeight = DUNGEON_HEIGHT;
	m_iWidth = DUNGEON_WIDTH;
}
struct SDunegonRoom
{
	Vector2i TopLeft;
	Vector2i BottomRight;
};
CDungeonMap::~CDungeonMap(){}

#define DEFAULT_DUNGEON_MIN_ROOMS 30
#define DEFAULT_DUNGEON_MAX_ROOMS 60
#define DUNGEON_ITEM_SPAWN_CHANCE 0.02 /* 2% item spawn chance */
#define DUNGEON_MONSTER_SPAWN_CHANCE 0.02 /* 2% monster spawn chance */

void CDungeonMap::GenerateDungeon( CGame* pGame, Vector2i& PlayerStartPos, std::vector< CItem* >& m_LoadedDungeonItems, std::vector< CMonster* >& m_LoadedDungeonMonsters )
{
	/* Get texture IDs */
	int WallTextureID = pGame->m_TextureManager.GetID( ".\\Graphics\\Wall2.bmp" );
	int FloorTextureID = pGame->m_TextureManager.GetID( ".\\Graphics\\DungeonFloor.bmp" );

	/* Fill in solid */
	for ( int i = 0; i < m_iWidth; i++ )
	{
		for ( int j = 0; j < m_iHeight; j++ )
		{
				m_Map[i][j] = CDungeonTile( WallTextureID, 0, 0);	
		}
	}
	
	int minrooms = DEFAULT_DUNGEON_MIN_ROOMS;
	int maxrooms = DEFAULT_DUNGEON_MAX_ROOMS;
	int minroomsize = 5;
	int maxroomsize = 10;
	int minmapvolume = 500;

	/* Restrict room size to smallest dimension of map */
	if ( maxroomsize > m_iWidth-2 )
		maxroomsize = m_iWidth-1;
	if ( maxroomsize > m_iHeight-2 )
		maxroomsize = m_iHeight-1;

	/* Allocate a random number of rooms */
	int numrooms = pGame->m_Random.RandInt( minrooms, maxrooms );
	int roomarrsize = 2 * numrooms;
	Vector2i* Rooms = new Vector2i[roomarrsize];

	/**/
	int failcount = 0;

	/* Generate rooms */
	for ( int i = 0; i < numrooms*2; i+=2 )
	{
		/* Generate room size */
		int thisroomwidth  = pGame->m_Random.RandInt( minroomsize, maxroomsize );
		int thisroomheight = pGame->m_Random.RandInt( minroomsize, maxroomsize );

		/* Generate room top left position */
		Rooms[i  ].X = pGame->m_Random.RandInt( 1, m_iWidth-2-thisroomwidth );
		Rooms[i  ].Y = pGame->m_Random.RandInt( 1, m_iHeight-2-thisroomheight );

		/* Ensure room doesn't go out of bounds */
		int roomxdelta = (m_iWidth-2)  - (Rooms[i].X+thisroomwidth);
		if ( roomxdelta < 0 )
			Rooms[i  ].X -= roomxdelta;
		int roomydelta = (m_iHeight-2) - (Rooms[i].Y+thisroomheight);
		if ( roomydelta < 0 )
			Rooms[i  ].Y -= roomydelta;

		/* Get room bottom right position */
		Rooms[i+1].X = Rooms[i  ].X + thisroomwidth;
		Rooms[i+1].Y = Rooms[i  ].Y + thisroomheight;

		/* Set first room as spawn point */
		if ( i == 0 )
		{
			PlayerStartPos.X = Rooms[i].X;
			PlayerStartPos.Y = Rooms[i].Y;
		}
		else
		{
			bool bUnique = true;
			/* Check if room is unique */
			for ( int j = 0; j < i; j+=2 )
			{
				if ( !( (Rooms[i].X > Rooms[j+1].X) || (Rooms[i+1].X < Rooms[j].X ) && (Rooms[i].Y > Rooms[j+1].Y) || (Rooms[i+1].Y < Rooms[j].Y) ))
				{
					bUnique = false;
					break;
				}
			}
			if ( !bUnique )
			{
				failcount++;
				i-=2;
				
				if ( failcount > 20 )
				{
					numrooms--;
					failcount = 0;
					//GenerateDungeon( pGame, PlayerStartPos );
					//return;
				}
				continue;
			}
			else
			{
				failcount = 0;
			}
		}
	}

	/* 'Carve out' rooms */ 
	for (int i = 0; i < numrooms * 2; i+=2)
	{
		for (int j = Rooms[i].X; j < Rooms[i+1].X; j++)
		{
			for (int k = Rooms[i].Y; k < Rooms[i+1].Y; k++)
			{
				m_Map[j][k] = CDungeonTile( FloorTextureID, 0, FL_Passable | FL_Transparent );
			}
		}
	}

	/* Allocate sorted rooms */
	Vector2i* SortedRooms = new Vector2i[2 * numrooms];

	/* First room is 'root' room from which distance is calculated */
	SortedRooms[0] = Rooms[0];
	SortedRooms[1] = Rooms[1];

	/* Sort rooms based on distance */
	for ( int i = 2; i < 2*numrooms; i+=2 )
	{
		Vector2i* pCurRoom = SortedRooms + (i - 2);

		float LargestRoomDist = 999999.0f;
		int closestroom = -1;
		
		for ( int j = 0; j < 2*numrooms; j+=2 )
		{
			/* Check if this room is already in sorted room list */
			bool uniqueroom = true;
			for ( int k = 0; k < i; k+=2 )
			{
				if ( SortedRooms[k] == Rooms[j] && SortedRooms[k+1] == Rooms[j+1] )
				{
					uniqueroom = false;
					break;
				}
			}

			/* Room already sorted, find next closest room */
			if ( !uniqueroom )
				continue;

			/* Get distance to room */
			Vector2i delta = (Rooms[i] - pCurRoom[0]);
			float thisroomdist = (float)(delta.X * delta.X + delta.Y * delta.Y);

			if ( LargestRoomDist > thisroomdist )
			{
				LargestRoomDist = thisroomdist;
				closestroom = j;
			}
		}

		/* Copy the next close
		st room into sorted room array */
		if ( closestroom > 0 )
		{
			SortedRooms[i  ] = Rooms[closestroom  ];
			SortedRooms[i+1] = Rooms[closestroom+1];
		}
	}

	/* Don't need Room array because we have sorted room array */
	delete Rooms;

	/* 'carve' corridoors */

	for ( int i = 0; i < (numrooms-1)*2; i+=2 )
	{
		// Curroom  = SortedRoom[i  ] & SortedRoom[i+1];
		// NextRoom = SortedRoom[i+2] & SortedRoom[i+3];

		/* Get start and end positions of corridoor */
		float cswidth = (float)(SortedRooms[i+1].X - SortedRooms[i  ].X); 
		float csheight= (float)(SortedRooms[i+1].Y - SortedRooms[i  ].Y);
		int StartX	  = SortedRooms[i  ].X + (int)(cswidth/2);
		int StartY	  = SortedRooms[i  ].Y + (int)(csheight/2);
		
		float cewidth = (float)(SortedRooms[i+3].X - SortedRooms[i+2].X); 
		float ceheight= (float)(SortedRooms[i+3].Y - SortedRooms[i+2].Y);
		int EndX	  = SortedRooms[i+2].X + (int)(cewidth/2);
		int EndY	  = SortedRooms[i+2].Y + (int)(ceheight/2);

		/* Position of 'cursor' to carve */
		Vector2i ccursor( StartX, StartY );

		/* State based random corridoors, state is direction which has a chance to be changed 
		each step. state 1 = horizontal, state 2 = vertical */
		int state = pGame->m_Random.RandInt( 1, 2 );

		/* If we should change state, set to false when X or Y coord is reached */
		bool bchangestate = true;

		while ( !(ccursor.X == EndX && ccursor.Y == EndY) )
		{
			/* Don't move one direction if we have already reached the end */
			if ( ccursor.X == EndX )
			{
				state = 2;
				bchangestate = false;
			}
			else if ( ccursor.Y == EndY )
			{
				state = 1;
				bchangestate = false;
			}

			/* 30% chance to change state */
			if ( bchangestate && pGame->m_Random.RandFloat() > 0.7f )
			{
				if ( state == 1 )
					state = 2;
				else
					state = 1;
			}

			if ( state == 1 )
			{
				/* Horizontal */
				if  ( ccursor.X < EndX )
					ccursor.X++;
				else if ( ccursor.X > EndX )
					ccursor.X--;
			}
			else
			{
				/* Vertical */
				if  ( ccursor.Y < EndY )
					ccursor.Y++;
				else if ( ccursor.Y > EndY )
					ccursor.Y--;
			}

			/* Set this tile to floor tile */
			m_Map[ccursor.X][ccursor.Y] = CDungeonTile( FloorTextureID, 0, FL_Passable | FL_Transparent );
		}
	}
	
	/* Free allocated memory */
	delete SortedRooms;

	/* Check if level is within min volume */
	int mapvolume = 0;
	for ( int i = 0; i < m_iWidth; i++ )
	{
		for ( int j = 0; j < m_iHeight; j++ )
		{
				if ( m_Map[i][j].FlagIsSet( FL_Passable ) )
					mapvolume++;	
		}
	}

	/* Map has very little room, redo  */
	if ( mapvolume < minmapvolume ) 
		GenerateDungeon( pGame, PlayerStartPos, m_LoadedDungeonItems, m_LoadedDungeonMonsters );

	/* Now add items... */
	if ( m_LoadedDungeonItems.size() > 0 )
	{
		for ( int i = 0; i < m_iWidth; i++ )
		{
			for ( int j = 0; j < m_iHeight; j++ )
			{
				if ( m_Map[i][j].FlagIsSet( FL_Passable ) )
				{
					/* 2% chance item spawns on tile */
					if (pGame->m_Random.RandFloat() < DUNGEON_ITEM_SPAWN_CHANCE)
					{
						CItem* pItem = 0;
						/* Special case: only one item loaded */
						if ( m_LoadedDungeonItems.size() == 1 )
							pItem = m_LoadedDungeonItems[0]->Copy( pGame );
						else
						{
							/* Choose a random item */
							int numitems = m_LoadedDungeonItems.size()-1;
 							int itemtoadd = pGame->m_Random.RandInt( 0, numitems );
							pItem = m_LoadedDungeonItems[itemtoadd]->Copy( pGame );
						}
						if ( !pItem )
							continue;
						pItem->SetPosition( i, j );
						m_vItems.push_back( pItem );
					}	/* 2% chance monster spawns on tile */
					else if (pGame->m_Random.RandFloat() < DUNGEON_MONSTER_SPAWN_CHANCE)
					{
						CMonster* pMon = 0;
						/* Special case: only one monster loaded */
						if ( m_LoadedDungeonMonsters.size() == 1 )
							pMon = m_LoadedDungeonMonsters[0]->Copy();
						else
						{
							/* Choose a random item */
							int nummons   = m_LoadedDungeonMonsters.size()-1;
 							int itemtoadd = pGame->m_Random.RandInt( 0, nummons );
							pMon = m_LoadedDungeonMonsters[itemtoadd]->Copy();
						}
						if ( !pMon )
							continue;
						pMon->SetPosition( Vector2i(i, j) );
						m_vMonsters.push_back( pMon );
					}
				}
			}
		}
	}
}
void CDungeonMap::Draw( CGame* pGame, int TileSize )
{
	/* Draw tiles */
	for ( int i = 0; i < m_iWidth; i++ )
	{
		for ( int j = 0; j < m_iHeight; j++ )
		{
			if ( m_Map[i][j].FlagIsSet( FL_Visible ) || pGame->m_Settings.m_bFullbright )
			{
				pGame->m_Drawing.DrawSprite( &pGame->m_WindowManager, i*TileSize, j*TileSize, 
					TileSize, TileSize, pGame->m_TextureManager.GetTexture( m_Map[i][j].m_iTextureID ) );
			}
			else if ( m_Map[i][j].FlagIsSet( FL_Discovered ) )
			{
				pGame->m_Drawing.DrawSprite( &pGame->m_WindowManager, i*TileSize, j*TileSize, 
					TileSize, TileSize, 0.5f, pGame->m_TextureManager.GetTexture( m_Map[i][j].m_iTextureID ) );
			}
		}
	}

	/* Draw items */
	Vector2i itpos;
	for ( unsigned int i = 0; i < m_vItems.size(); i++ )
	{
		if ( !m_vItems[i] )
			continue;
		itpos = m_vItems[i]->m_vLocation;
		if ( IsTileVisible( itpos ) )
		{
			pGame->m_Drawing.DrawSprite( &pGame->m_WindowManager, itpos.X*TileSize, itpos.Y*TileSize, 
					TileSize, TileSize, pGame->m_TextureManager.GetTexture( m_vItems[i]->m_iTextureID ) );
		}
	}

	/* Draw monsters */
	for ( unsigned int i = 0; i < m_vMonsters.size(); i++ )
	{
		if ( !m_vMonsters[i] )
			continue;
		itpos = m_vMonsters[i]->m_vPosition;
		if ( IsTileVisible( itpos ) )
		{
			pGame->m_Drawing.DrawSprite( &pGame->m_WindowManager, itpos.X*TileSize, itpos.Y*TileSize, 
					TileSize, TileSize, pGame->m_TextureManager.GetTexture( m_vMonsters[i]->m_iTextureID ) );
		}
	}

	/* Draw projectiles */
	for ( unsigned int i = 0; i < m_vProjectiles.size(); i++ )
	{
		if ( !m_vProjectiles[i] )
			continue;
		itpos = m_vProjectiles[i]->GetPosition();
		if ( IsTileVisible( itpos ) )
		{
			pGame->m_Drawing.DrawSprite( &pGame->m_WindowManager, itpos.X*TileSize, itpos.Y*TileSize, 
					TileSize, TileSize, pGame->m_TextureManager.GetTexture( m_vProjectiles[i]->m_iTextureID ) );
		}
	}
}

bool CDungeonMap::WallAtPosition( Vector2i Position )
{
	return !m_Map[Position.X][Position.Y].FlagIsSet( FL_Passable );
}
bool CDungeonMap::CanMoveInto( Vector2i Position, CDungeonPlayer* pPlayer )
{
	if ( !InBounds( Position ) )
		return false;
	
	for ( unsigned int i = 0; i < m_vMonsters.size(); i++ )
	{
		if ( !m_vMonsters[i] )
			continue;
		if ( m_vMonsters[i]->GetPosition() == Position )
			return false;
	}
	
	if ( pPlayer->GetPosition() == Position )
		return false;

	return ( m_Map[Position.X][Position.Y].FlagIsSet(FL_Passable) );
}
bool CDungeonMap::CanMoveInto( Vector2i Position )
{
	if ( !InBounds( Position ) )
		return false;
	for ( unsigned int i = 0; i < m_vMonsters.size(); i++ )
	{
		if ( !m_vMonsters[i] )
			continue;
		if ( m_vMonsters[i]->GetPosition() == Position )
			return false;
	}
	return ( m_Map[Position.X][Position.Y].FlagIsSet(FL_Passable) );
}
bool CDungeonMap::InBounds( Vector2i Position )
{
	return ! ( Position.X < 0 || Position.X >= m_iWidth || Position.Y < 0 || Position.Y >= m_iHeight );
}

bool CDungeonMap::CalcVisibleTile( Vector2i Origin, Vector2i Tile )
{
	/* Square we are on is always visible */
	if ( Origin == Tile )
		return true;

	/* Stuff outside the map is never visible */
	if ( !InBounds(Tile) ) 
		return false;

	/* Get ray unit vector */
	Vector2f Ray( (float)(Tile.X - Origin.X), (float)(Tile.Y - Origin.Y) );
	float RayLength = Ray.Length();
	Vector2f RayUnit = Ray / RayLength;
	
	/* Initial ray position is origin */
	Ray.X = (float)Origin.X;
	Ray.Y = (float)Origin.Y;
	
	/* Ray position rounded to nearest index */
	Vector2i CurIndex( (int)Ray.X, (int)Ray.Y );

	while ( CurIndex != Tile )
	{
		/* Round ray vector to nearest index */
		CurIndex = Vector2i( (int)floor(Ray.X + 0.5f), (int)floor(Ray.Y + 0.5f) );
		
		/* Check if this position isn't visible */
		if ( !InBounds( CurIndex ) )
			return false;
		if ( !m_Map[CurIndex.X][CurIndex.Y].FlagIsSet(FL_Passable) )
			return false;

		/* Add a single step to the ray */
		Ray += RayUnit;
	}
	/* We made it to the tile without hitting a solid tile, so is visible */
	return true;
}
bool CDungeonMap::IsTileVisible( Vector2i Tile )
{
	if ( !InBounds( Tile ) )
		return false;
	return m_Map[Tile.X][Tile.Y].FlagIsSet( FL_Visible );
}
void CDungeonMap::UpdateVisible( Vector2i Origin, Vector2i Tile, float FOV )
{
	/* Check if it is far enough to see this tile */
	if ( (Origin - Tile).Length() > FOV )
		return;

	/* Check if this tile is in the map */
	if ( !InBounds( Tile ) )
		return;

	/* Don't bother checking visibility on already visible tiles */
	if ( IsTileVisible( Tile ) )
		return;

	/* Set visible flag */
	m_Map[Tile.X][Tile.Y].SetFlag( FL_Visible );
	m_Map[Tile.X][Tile.Y].SetFlag( FL_Discovered );

	/* we have hit a wall, make it visible but don't look further */
	if ( !CalcVisibleTile( Origin, Tile ) )
		return;

	/* Recursive flood fill */
	UpdateVisible( Origin, Vector2i(Tile.X+1, Tile.Y  ), FOV ); /* Right */
	UpdateVisible( Origin, Vector2i(Tile.X-1, Tile.Y  ), FOV ); /* Left */
	UpdateVisible( Origin, Vector2i(Tile.X  , Tile.Y+1), FOV ); /* Forward */
	UpdateVisible( Origin, Vector2i(Tile.X  , Tile.Y-1), FOV ); /* Backward */
	UpdateVisible( Origin, Vector2i(Tile.X+1, Tile.Y+1), FOV ); /* Top right */
	UpdateVisible( Origin, Vector2i(Tile.X+1, Tile.Y-1), FOV ); /* Bottom right */
	UpdateVisible( Origin, Vector2i(Tile.X-1, Tile.Y+1), FOV ); /* Top left */
	UpdateVisible( Origin, Vector2i(Tile.X-1, Tile.Y-1), FOV ); /* Bottom left */
}
void CDungeonMap::OnMove( Vector2i Position, CMessageList* pMessageList )
{
	if ( !InBounds(Position) )
		return;
	
	/* Reset visibility */
	for ( int i = 0; i < m_iWidth; i++ )
	{
		for ( int j = 0; j < m_iHeight; j++ )
		{
			m_Map[i][j].RemoveFlag( FL_Visible );
		}
	}

	/* Update visibility */
	UpdateVisible( Position, Position, DEFAULT_DUNGEON_FOV );

	/* Check for items on this square */
	int numsqitems = 0;
	CItem* psqitem = 0;
	for ( unsigned int i = 0; i < m_vItems.size(); i++ )
	{
		if ( !m_vItems[i] )
			continue;
		if ( m_vItems[i]->GetLocation() == Position )
		{
			numsqitems++;
			psqitem = m_vItems[i];
		}
	}

	/* Inform user that there are items on this square */
	if ( numsqitems == 1 )
	{
		char* name = psqitem->GetName();
		if ( name )
		{
			int namelen = strlen( name );
			/* Allocate room for name and 'There is a %s here.' string */
			char* msgstr = new char[namelen + 18];
			sprintf_s( msgstr, namelen + 18, "There is a %s here.", name );
			pMessageList->AddMessage( msgstr );
			/* Messagelist copies string, we can free space */
			delete msgstr;
		}
	}
	else if ( numsqitems > 1 )
		pMessageList->AddMessage( "There are many items here." );
}

void CDungeonMap::Grab( Vector2i Position, CDungeonInventory* pInventory, CGame* pGame )
{
	int iamt = 0;
	int lastindex = 0;
	std::vector< CItem* > vitems;
	
	for ( unsigned int i = 0; i < m_vItems.size(); i++ )
	{
		if ( !m_vItems[i] )
			continue;
		if ( m_vItems[i]->GetLocation() == Position )
		{
			iamt++;
			vitems.push_back( m_vItems[i] );
			lastindex = i;
		}
	}
	
	if ( iamt <= 0 )
		return;
	else if ( iamt == 1 )
	{
		/* Special case - 1 item */
		pInventory->AddItem( m_vItems[lastindex] );
		m_vItems.erase( m_vItems.begin() + lastindex );
	}
	else
	{
		/* Multiple items */
		CDungeonInventory tempinv;
		tempinv.SetMaxWeight( 20000000.0f );
		for ( unsigned int i = 0; i < vitems.size(); i++ )
			tempinv.AddItem( vitems[i] );
		
		/* Add items to inventory */
		std::vector< CItem* > chosenitems;
		tempinv.ChooseItems( pGame, chosenitems, "pick up" );
		for ( unsigned int i = 0; i < chosenitems.size(); i++ )
			pInventory->AddItem( chosenitems[i] );

		/* Remove from map */
		for ( unsigned int i = 0; i < chosenitems.size(); i++ ) 
			m_vItems.erase( std::remove( m_vItems.begin(), m_vItems.end(), chosenitems[i] ), m_vItems.end() ); //Erase-remove idiom
	}
}
void CDungeonMap::Drop( Vector2i Position, CItem* pItem )
{
	pItem->m_vLocation = Position;
	m_vItems.push_back( pItem );
}
bool CDungeonMap::GetNearestItem( Vector2i Position, CItem*& pRetItem, float& fRetDistance )
{
	float fClosestDist = 10000;
	CItem* pClosestItem = 0;
	for ( unsigned int i=0; i < m_vItems.size(); i++ )
	{
		if ( !m_vItems[i] )
			continue;
		if ( !m_vItems[i]->IsValid() )
			continue;
		Vector2i p = m_vItems[i]->GetLocation();
		p = p - Position;
		float d = p.Length();
		if ( d < fClosestDist )
		{
			fClosestDist = d;
			pClosestItem = m_vItems[i];
		}
	}
	pRetItem = pClosestItem;
	fRetDistance = fClosestDist;

	return pClosestItem != 0;
}
void CDungeonMap::TurnsPassed( int iNumTurns )
{
	/* Give all monsters turns to spend */
	for ( unsigned int i = 0; i < m_vMonsters.size(); i++ )
	{
		if ( m_vMonsters[i] )
			m_vMonsters[i]->GiveTurns( iNumTurns );
	}
}
void CDungeonMap::MonstersThink( CDungeonPlayer* pPlayer, CGame* pGame )
{
	/* Don't let monsters move around while a projectile is being predicted */
	if ( BusyWithProjectiles() )
		return;

	std::vector< CMonster* > vMonstersToRemove;
	/* Call Think() on all monsters */
	for ( unsigned int i = 0; i < m_vMonsters.size(); i++ )
	{
		if ( !m_vMonsters[i] )
			continue;

		/* Turn dead monsters into corpses */
		if ( m_vMonsters[i]->IsDead() )
		{
			CItem* pCorpse = m_vMonsters[i]->MakeCorpse( pGame );
			pCorpse->SetPosition( m_vMonsters[i]->GetPosition() );
			vMonstersToRemove.push_back( m_vMonsters[i] );
			m_vItems.push_back( pCorpse );
			continue;
		}

		/* Make alive monsters think */
		m_vMonsters[i]->Think( this, pPlayer, pGame );
	}

	/* Remove dead monsters */
	for ( unsigned int i = 0; i < vMonstersToRemove.size(); i++ )
		m_vMonsters.erase( std::remove( m_vMonsters.begin(), m_vMonsters.end(), vMonstersToRemove[i] ), m_vMonsters.end() );
}
CMonster* CDungeonMap::GetMonsterAtTile( Vector2i vTilePosition )
{
	for ( unsigned int i = 0; i < m_vMonsters.size(); i++ )
	{
		if ( !m_vMonsters[i] ) 
			continue;
		if ( m_vMonsters[i]->GetPosition() == vTilePosition )
			return m_vMonsters[i];
	}
	return 0;
}
CMonster* CDungeonMap::GetFirstMonsterInRay( Vector2i Origin, Vector2i Final )
{
	if ( Origin == Final )
		return ( GetMonsterAtTile( Origin ) );

	Vector2i delta = Final - Origin;
	Vector2f Ray( (float)delta.X, (float)delta.Y );
	Vector2f RayUnit = Ray / Ray.Length(); 
	Ray.X = (float)Origin.X;
	Ray.Y = (float)Origin.Y;

	Vector2i MonsterPos;
	CMonster* pMonster = 0;

	/* Ray position rounded to nearest index */
	Vector2i CurIndex( (int)Ray.X, (int)Ray.Y );

	while ( CurIndex != Final )
	{
		/* Round ray vector to nearest index */
		CurIndex = Vector2i( (int)floor(Ray.X + 0.5f), (int)floor(Ray.Y + 0.5f) );
		
		/* Check if this position isn't visible */
		if ( !InBounds( CurIndex ) )
			return 0;
		if ( !m_Map[CurIndex.X][CurIndex.Y].FlagIsSet(FL_Passable) )
			return 0;
		/* Check if ray hits player */
		pMonster = GetMonsterAtTile( CurIndex );
		if ( pMonster )
			return pMonster;

		/* Add a single step to the ray */
		Ray += RayUnit;
	}
	return 0;
}
bool CDungeonMap::RayHitsPlayer( Vector2i Origin, Vector2i Final, CDungeonPlayer* pPlayer )
{
	if ( Origin == Final )
		return ( pPlayer->GetPosition() == Origin );

	Vector2i PlayerPos = pPlayer->GetPosition();
	Vector2i delta = Final - Origin;
	Vector2f Ray( (float)delta.X, (float)delta.Y );
	Vector2f RayUnit = Ray / Ray.Length(); 
	Ray.X = (float)Origin.X;
	Ray.Y = (float)Origin.Y;

	/* Ray position rounded to nearest index */
	Vector2i CurIndex( (int)Ray.X, (int)Ray.Y );

	while ( CurIndex != Final )
	{
		/* Round ray vector to nearest index */
		CurIndex = Vector2i( (int)floor(Ray.X + 0.5f), (int)floor(Ray.Y + 0.5f) );
		
		/* Check if this position isn't visible */
		if ( !InBounds( CurIndex ) )
			return false;
		if ( !m_Map[CurIndex.X][CurIndex.Y].FlagIsSet(FL_Passable) )
			return false;
		/* Check if ray hits player */
		if ( CurIndex == PlayerPos )
			return true;

		/* Add a single step to the ray */
		Ray += RayUnit;
	}
	return false;
}

bool CDungeonMap::BusyWithProjectiles()
{
	return ( m_vProjectiles.size() != 0 );
}
void CDungeonMap::UpdateProjectiles( CDungeonPlayer* pPlayer, CGame* pGame, CMessageList* pMessageList )
{
	if ( !BusyWithProjectiles() )
		return;

	std::vector< CProjectile* > vDeadProjectiles;

	/* Update projectiles and find dead ones */
	for ( unsigned int i = 0; i < m_vProjectiles.size(); i++ ) 
	{
		if ( !m_vProjectiles[i] )
			continue;
		
		m_vProjectiles[i]->Update( pGame, this, pPlayer, pMessageList );
	
		if ( m_vProjectiles[i]->IsDead() )
			vDeadProjectiles.push_back( m_vProjectiles[i] );
	}

	/* Remove all dead projectiles */
	for ( unsigned int i = 0; i < vDeadProjectiles.size(); i++ )
		m_vProjectiles.erase( std::remove( m_vProjectiles.begin(), m_vProjectiles.end(), vDeadProjectiles[i] ), m_vProjectiles.end() ); 
}
void CDungeonMap::AddProjectileToMap( CProjectile* pProjectile )
{
	if ( pProjectile )
		m_vProjectiles.push_back( pProjectile );
}