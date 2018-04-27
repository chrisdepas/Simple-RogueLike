#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <SFML/Graphics.hpp>

#define INVALID_TEXTURE_ID -1

class CTextureItem
{
private:
	sf::Texture* m_pTexture;
	char* m_szTextureName;
	bool m_bFailedToLoad;

public:
	CTextureItem();
	CTextureItem( char* m_szTexture );
	~CTextureItem();
	bool MatchesName( char* szTextureName );
	sf::Texture* GetTexture();
	char* GetName();
};

class CTextureManager
{
private:
	std::vector < CTextureItem* > m_vTextures;
public:
	CTextureManager();
	~CTextureManager();
	int GetID( char* szTextureName ); /* ID is the index in the texture array */
	sf::Texture* GetTexture( int ID ); /* Get a texture from an ID */
};

#endif