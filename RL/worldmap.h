#ifndef __WORLDMAP_H__
#define __WORLDMAP_H__

class CGame;
#include "vector.h"

#define WORLDMAP_WIDTH 512
#define WORLDMAP_HEIGHT WORLDMAP_WIDTH

enum EWorldMapFlags
{
	F_Visible		= 1 << 0,	/**/
	F_DeepWater		= 1 << 2,	/**/
	F_NotPassable	= 1 << 1,	/**/
	F_Transparent	= 1 << 3,	/**/ 
};

enum ETileType
{
	/* Beach biome */
	TT_Water,
	TT_DeepWater,
	TT_Sand,
	TT_WetSand,
	TT_SandStone,

	/* Cold biome */
	TT_Ice,
	TT_SnowDirt,
	TT_SnowGrass,
	TT_SnowSand,

	/* Land biome */
	TT_Grass,	
	TT_Dirt,
	
	/* Hot biome */
	TT_Lava,
	TT_Gravel,

	TT_Clay,

};
struct STileInfo
{
	float m_fElevation;
	float m_fRainfall;
	float m_fTemperature;

	ETileType CalcType();
};

struct SMapTile
{
	unsigned char m_cTextureID;
	unsigned char m_Flags;
};

class CWorldMap
{
	int m_iWidth;
	int m_iHeight;

	SMapTile m_Tiles[WORLDMAP_WIDTH][WORLDMAP_HEIGHT];

public:
	CWorldMap();
	~CWorldMap();

	void Generate( CGame* pGame );
	void Draw( CGame* pGame, int iTileSize );
	Vector2i CalcSpawn();
};

#endif