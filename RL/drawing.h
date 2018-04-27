#ifndef __DRAWING_H__
#define __DRAWING_H__

#define GAME_FONT_FILE "DejaVuSans.ttf"
#include "SFML/Graphics.hpp"
#include "window.h"

class CDrawingManager
{
	sf::Font m_fFont;
	
public:
	CDrawingManager();
	~CDrawingManager();
	void Initialise();
	void DrawText(  CWindowManager* pWindowManager, char* szText, int X, int Y, int iSize, unsigned __int8 R, unsigned __int8 G, 
		unsigned __int8 B, unsigned __int8 A );
	void DrawTextCentred( CWindowManager* pWindowManager, char* szText, int X, int Y, int iSize, unsigned __int8 R, unsigned __int8 G, 
		unsigned __int8 B, unsigned __int8 A );
	void DrawTextCentredX( CWindowManager* pWindowManager, char* szText, int X, int Y, int iSize, unsigned __int8 R, 
									  unsigned __int8 G, unsigned __int8 B, unsigned __int8 A );
	void DrawSprite( CWindowManager* pWindowManager, int X, int Y, int iWidth, int iHeight, sf::Texture* pTexture );
	void DrawSprite( CWindowManager* pWindowManager, Vector2i vPosition, int iWidth, int iHeight, sf::Texture* pTexture );
	void DrawSprite( CWindowManager* pWindowManager, int X, int Y, int iWidth, int iHeight, float fAlpha, sf::Texture* pTexture );
	
	/* Draw a simple square shape */
	void DrawSquare( CWindowManager* pWindowManager, Vector2i vPosition, int iSize, unsigned __int8 R, unsigned __int8 G, 
		unsigned __int8 B, unsigned __int8 A );
};
#endif