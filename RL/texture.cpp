#include "stdafx.h"
#include "texture.h"

CTextureItem::CTextureItem()
{
	m_pTexture = 0;
	m_szTextureName = 0;
	m_bFailedToLoad = false;
}
CTextureItem::CTextureItem( char* szTextureName )
{
	m_bFailedToLoad = false;

	/* Save a copy of texture name */ 
	int slen = strlen( szTextureName );
	m_szTextureName = new char[ slen+1 ];
	memcpy( m_szTextureName, szTextureName, slen );
	m_szTextureName[ slen ] = 0;

	/* Init texture to NULL */
	m_pTexture = 0;
}
CTextureItem::~CTextureItem()
{
	if ( m_szTextureName )
		delete m_szTextureName;
	m_szTextureName = 0;
}
bool CTextureItem::MatchesName( char* szTextureName )
{
	if ( !m_szTextureName || !szTextureName )
		return false;
	return !_stricmp( szTextureName, m_szTextureName );
}
sf::Texture* CTextureItem::GetTexture()
{
	/* This image cannot be loaded, return NULL */
	if ( m_bFailedToLoad )
		return NULL;

	/* If texture is loaded already, return it*/
	if ( m_pTexture )
		return m_pTexture;

	/* check if texture has a name */
	if ( !m_szTextureName )
		return NULL;

	/* Load texture */
	m_pTexture = new sf::Texture;
	if ( !m_pTexture->loadFromFile( m_szTextureName ) )
	{
		/* Couldn't load, return NULL */
		m_bFailedToLoad = true;
		delete m_pTexture;
		return NULL;
	}
	return m_pTexture;
}
char* CTextureItem::GetName()
{
	return m_szTextureName;
}

CTextureManager::CTextureManager()
{
}
CTextureManager::~CTextureManager()
{
	for (unsigned int i = 0; i < m_vTextures.size(); i++)
	{
		if ( m_vTextures[i] )
			delete m_vTextures[i];
	}
	m_vTextures.clear();
}
int CTextureManager::GetID( char* szTextureName )
{
	if ( !szTextureName )
		return INVALID_TEXTURE_ID;

	/* If texture has already been saved, return the index */
	for (unsigned int i = 0; i < m_vTextures.size(); i++)
	{
		if ( m_vTextures[i]->MatchesName( szTextureName ) )
			return i;
	}

	/* Texture hasn't been saved, add and return index */
	m_vTextures.push_back( new CTextureItem( szTextureName ) );
	return ( m_vTextures.size() - 1 );
} 
sf::Texture* CTextureManager::GetTexture( int ID )
{
	if ( ID == INVALID_TEXTURE_ID || ID < 0 || ID > (int)m_vTextures.size()-1 )
		return 0;
	return m_vTextures[ID]->GetTexture();
} 
