#include "stdafx.h"
#include "drawing.h"

CDrawingManager::CDrawingManager()
{
}
CDrawingManager::~CDrawingManager()
{
}
void CDrawingManager::Initialise()
{
	m_fFont.loadFromFile( GAME_FONT_FILE );
}
void CDrawingManager::DrawText( CWindowManager* pWindowManager, char* szText, int X, int Y, int iSize, unsigned __int8 R, unsigned __int8 G,
							   unsigned __int8 B, unsigned __int8 A )
{
	sf::Text text;
	text.setFont( m_fFont );
	text.setString( szText );
	text.setCharacterSize( iSize );
	text.setColor( sf::Color(R, G, B, A) );
	text.setPosition( (float)X, (float)Y );
	pWindowManager->Draw( &text );
}
void CDrawingManager::DrawTextCentred( CWindowManager* pWindowManager, char* szText, int X, int Y, int iSize, unsigned __int8 R, 
									  unsigned __int8 G, unsigned __int8 B, unsigned __int8 A )
{
	sf::Text text;
	text.setFont( m_fFont );
	text.setString( szText );
	text.setCharacterSize( iSize );

	/* Centre the text */
	sf::FloatRect bound = text.getGlobalBounds();
	text.setOrigin( bound.left + bound.width/2.0f, bound.top  + bound.height/2.0f );
	
	text.setColor( sf::Color(R, G, B, A) );
	text.setPosition( (float)X, (float)Y );
	pWindowManager->Draw( &text );
}
void CDrawingManager::DrawTextCentredX( CWindowManager* pWindowManager, char* szText, int X, int Y, int iSize, unsigned __int8 R, 
									  unsigned __int8 G, unsigned __int8 B, unsigned __int8 A )
{
	sf::Text text;
	text.setFont( m_fFont );
	text.setString( szText );
	text.setCharacterSize( iSize );

	/* Centre the text */
	sf::FloatRect bound = text.getGlobalBounds();
	text.setOrigin( bound.left + bound.width/2.0f, 0 );
	
	text.setColor( sf::Color(R, G, B, A) );
	text.setPosition( (float)X, (float)Y );
	pWindowManager->Draw( &text );
}
void CDrawingManager::DrawSprite( CWindowManager* pWindowManager, int X, int Y, int iWidth, int iHeight, float fAlpha, sf::Texture* pTexture )
{
	if ( !pTexture )
		return;
	
	/* Create a renderable sprite */
	sf::Sprite sp;
	sp.setTexture( *pTexture );
	sp.setPosition( sf::Vector2f((float)X,(float)Y) );
	
	/* Calculate integer alpha and set */
	int Alpha = (int)(255 * fAlpha);
	sp.setColor( sf::Color(255, 255, 255, Alpha) );
	
	/* Scale to required dimensions */
	sf::Vector2u f = pTexture->getSize();
	sf::Vector2f scale( (float)iWidth / f.x, (float)iHeight / f.y );
	sp.setScale( scale );
	
	/* Draw to window*/
	pWindowManager->Draw( &sp ); 
}
void CDrawingManager::DrawSprite( CWindowManager* pWindowManager, int X, int Y, int iWidth, int iHeight, sf::Texture* pTexture )
{
	if ( !pTexture )
		return;
	
	/* Create a renderable sprite */
	sf::Sprite sp;
	sp.setTexture( *pTexture );
	sp.setPosition( sf::Vector2f((float)X,(float)Y) );
	
	/* Scale to required dimensions */
	sf::Vector2u f = pTexture->getSize();
	sf::Vector2f scale( (float)iWidth / f.x, (float)iHeight / f.y );
	sp.setScale( scale );
	
	/* Draw to window*/
	pWindowManager->Draw( &sp ); 
}
void CDrawingManager::DrawSprite( CWindowManager* pWindowManager, Vector2i vPosition, int iWidth, int iHeight, sf::Texture* pTexture )
{
	if ( !pTexture )
		return;
	
	/* Create a renderable sprite */
	sf::Sprite sp;
	sp.setTexture( *pTexture );
	sp.setPosition( (float)vPosition.X, (float)vPosition.Y );
	
	/* Scale to required dimensions */
	sf::Vector2u f = pTexture->getSize();
	sf::Vector2f scale( (float)iWidth / f.x, (float)iHeight / f.y );
	sp.setScale( scale );
	
	/* Draw to window*/
	pWindowManager->Draw( &sp ); 
}

void CDrawingManager::DrawSquare( CWindowManager* pWindowManager, Vector2i vPosition, int iSize, unsigned __int8 R, unsigned __int8 G, 
		unsigned __int8 B, unsigned __int8 A )
{
	sf::RectangleShape sq( sf::Vector2f( (float)iSize, (float)iSize ) );
	
	sq.setPosition( (float)vPosition.X, (float)vPosition.Y );
	sq.setFillColor( sf::Color(R, G, B, A) );
	sq.setOutlineThickness( 0.0f );

	pWindowManager->Draw( &sq );
}