#include "stdafx.h"
#include "worldmap.h"
#include "game.h"

#define ICE_TEXTURE			".\\Graphics\\Terrain\\Ice.bmp"
#define WATER_TEXTURE		".\\Graphics\\Terrain\\Water.bmp"
#define DEEPWATER_TEXTURE	".\\Graphics\\Terrain\\DeepWater.bmp"
#define SAND_TEXTURE		".\\Graphics\\Terrain\\Sand.bmp"
#define WETSAND_TEXTURE		".\\Graphics\\Terrain\\WetSand.bmp"
#define GRASS_TEXTURE		".\\Graphics\\Terrain\\Grass.bmp"
#define SANDSTONE_TEXTURE	".\\Graphics\\Terrain\\SandStone.bmp"
#define SNOWDIRT_TEXTURE	".\\Graphics\\Terrain\\SnowDirt.bmp"
#define SNOWGRASS_TEXTURE	".\\Graphics\\Terrain\\SnowGrass.bmp"
#define SNOWSAND_TEXTURE	".\\Graphics\\Terrain\\SnowSand.bmp"
#define DIRT_TEXTURE		".\\Graphics\\Terrain\\Dirt.bmp"
#define LAVA_TEXTURE		".\\Graphics\\Terrain\\Lava.bmp"
#define GRAVEL_TEXTURE		".\\Graphics\\Terrain\\Gravel.bmp"
#define CLAY_TEXTURE		".\\Graphics\\Terrain\\Clay.bmp"

#define MAP_PERLIN_SPREAD 0.015f // This value is 1/UnitsPerBiome
#define MAP_PERLIN_OFFSET_RAINFALL 2003.0f
#define MAP_PERLIN_OFFSET_TEMP 185243.0f

#define FREEZING_TEMP 0.4f
#define SAND_HEIGHT 0.2f
#define WETSAND_HEIGHT 0.18f
#define WATER_HEIGHT 0.6f

ETileType STileInfo::CalcType()
{
	//if ( m_fTemperature > FREEZING_TEMP )
	{
		if ( m_fElevation > WATER_HEIGHT )
		{
			if ( m_fElevation > 0.99f )
				return TT_Lava;
			if ( m_fElevation > 0.95f )
				return TT_Gravel;
			else if ( m_fElevation > 0.91f )
				return TT_Clay;
			else if ( m_fElevation > 0.80f )
				return TT_Dirt;
			else if ( m_fElevation > 0.64f )
				return TT_Grass;
			else
				return TT_Sand;
		}
		else
		{
			if ( m_fElevation > 0.55f )
				return TT_Water;
			else
				return TT_DeepWater;
		}
	}
	//else
	{
		if ( m_fElevation > 0.98f )
			return TT_Clay;
		else if ( m_fElevation > 0.87f )
			return TT_Gravel;
		else if ( m_fElevation > 0.6f )
			return TT_SnowDirt;
		else if ( m_fElevation > 0.4f )
			return TT_SnowGrass;
		else if ( m_fElevation > 0.3f )
			return TT_SnowSand;
		else 
			return TT_Ice;
	}
}

void CWorldMap::Generate( CGame* pGame )
{
	int icetex		= pGame->m_TextureManager.GetID( ICE_TEXTURE );
	int watertex	= pGame->m_TextureManager.GetID( WATER_TEXTURE );
	int sandtex		= pGame->m_TextureManager.GetID( SAND_TEXTURE );
	int wetsandtex	= pGame->m_TextureManager.GetID( WETSAND_TEXTURE );
	int grasstex	= pGame->m_TextureManager.GetID( GRASS_TEXTURE );
	int sandstonetex= pGame->m_TextureManager.GetID( SANDSTONE_TEXTURE );
	int snowdirttex = pGame->m_TextureManager.GetID( SNOWDIRT_TEXTURE );
	int snowgrasstex= pGame->m_TextureManager.GetID( SNOWGRASS_TEXTURE );
	int snowsandtex = pGame->m_TextureManager.GetID( SNOWSAND_TEXTURE );
	int dirttex		= pGame->m_TextureManager.GetID( DIRT_TEXTURE );
	int lavatex		= pGame->m_TextureManager.GetID( LAVA_TEXTURE );
	int graveltex	= pGame->m_TextureManager.GetID( GRAVEL_TEXTURE );
	int claytex		= pGame->m_TextureManager.GetID( CLAY_TEXTURE );
	int deepwater	= pGame->m_TextureManager.GetID( DEEPWATER_TEXTURE );

	if ( deepwater > MAXBYTE )
	{
		printf( "[FATAL] Too many textures loaded before terrain texture." );
		exit ( -1 );
	}
	STileInfo curinfo;
	ETileType thistiletype;

	for ( int i = 0; i < m_iWidth; i++ )
	{
		for ( int j = 0; j < m_iHeight; j++ )
		{
			/* Reset flags */
			m_Tiles[i][j].m_Flags = 0;

			/* Calculate randomized elevation of tile */
			curinfo.m_fElevation = pGame->m_Random.InterpolatedNoise( i * MAP_PERLIN_SPREAD, j * MAP_PERLIN_SPREAD );
			
			/* Calculate randomised rainfall on tile */
			curinfo.m_fRainfall  = pGame->m_Random.InterpolatedNoise( (i+MAP_PERLIN_OFFSET_RAINFALL) * MAP_PERLIN_SPREAD,
				(j+MAP_PERLIN_OFFSET_RAINFALL) * MAP_PERLIN_SPREAD );

			/* Calculate randomized temperature of tile */
			curinfo.m_fTemperature = pGame->m_Random.InterpolatedNoise( (i+MAP_PERLIN_OFFSET_TEMP) * MAP_PERLIN_SPREAD,
				(j+MAP_PERLIN_OFFSET_TEMP) * MAP_PERLIN_SPREAD );

			/* Based on these values, return a tile type */
			thistiletype = curinfo.CalcType();

			switch( thistiletype )
			{
			case TT_Water:
				m_Tiles[i][j].m_cTextureID = watertex;
				break;
			case TT_DeepWater:
				m_Tiles[i][j].m_cTextureID = deepwater;
				m_Tiles[i][j].m_Flags &= F_DeepWater;
				break;
			case TT_Sand:
				m_Tiles[i][j].m_cTextureID = sandtex;
				break;
			case TT_WetSand:
				m_Tiles[i][j].m_cTextureID = wetsandtex;
				break;
			case TT_SandStone:
				m_Tiles[i][j].m_cTextureID = sandstonetex;
				break;
			case TT_Ice:
				m_Tiles[i][j].m_cTextureID = icetex;
				break;
			case TT_SnowDirt:
				m_Tiles[i][j].m_cTextureID = snowdirttex;
				break;
			case TT_SnowGrass:
				m_Tiles[i][j].m_cTextureID = snowgrasstex;
				break;
			case TT_SnowSand:
				m_Tiles[i][j].m_cTextureID = snowsandtex;
				break;
			case TT_Grass:	
				m_Tiles[i][j].m_cTextureID = grasstex;
				break;
			case TT_Dirt:
				m_Tiles[i][j].m_cTextureID = dirttex;
				break;
			case TT_Lava:
				m_Tiles[i][j].m_cTextureID = lavatex;
				break;
			case TT_Gravel:
				m_Tiles[i][j].m_cTextureID = graveltex;
				break;
			case TT_Clay:
				m_Tiles[i][j].m_cTextureID = claytex;
				break;
			default:
				printf( "[FATAL] Unknown tile created" );
				exit( -1 );
			}
		}
	}
}
void CWorldMap::Draw( CGame* pGame, int iTileSize )
{
	/* Draw tiles */
	for ( int i = 0; i < m_iWidth; i++ )
	{
		for ( int j = 0; j < m_iHeight; j++ )
		{
			pGame->m_Drawing.DrawSprite( &pGame->m_WindowManager, i*iTileSize, j*iTileSize, 
					iTileSize, iTileSize, pGame->m_TextureManager.GetTexture( m_Tiles[i][j].m_cTextureID ) );
		}
	}
}
Vector2i CWorldMap::CalcSpawn()
{
	for ( int i = 0; i < m_iWidth; i++ )
	{
		for ( int j = 0; j < m_iHeight; j++ )
		{
			if ( m_Tiles[i][j].m_Flags & F_DeepWater || m_Tiles[i][j].m_Flags & F_NotPassable )
				continue;
			return Vector2i( i, j );
		}
	}
	return Vector2i( 1, 1 );
}
CWorldMap::CWorldMap()
{
	m_iHeight = WORLDMAP_HEIGHT;
	m_iWidth = WORLDMAP_WIDTH;
}
CWorldMap::~CWorldMap()
{
}