#include "stdafx.h"
#include "window.h"

CWindowManager::CWindowManager()
{
	m_pWindow = NULL;
	m_bWindowCreated = false;
	m_bWindowClosed = false;
	m_bFocus = true;
}
CWindowManager::~CWindowManager()
{
	if ( m_bWindowCreated && m_pWindow )
		delete m_pWindow;
}

/*	Create a window with supplied settings.
	WARNING: Assumes settings are sanitary!
*/
void CWindowManager::Create( CSettings* pSettings )
{
	if ( m_bWindowCreated )
		return;

	m_bWindowCreated = true;

	sf::VideoMode videomode( pSettings->m_vScreenSize.X, pSettings->m_vScreenSize.Y );
	
	m_vSize = pSettings->m_vScreenSize;

	if ( pSettings->m_bFullscreen )
		m_pWindow = new sf::RenderWindow( videomode, WINDOW_TITLE, sf::Style::Fullscreen );
	else
		m_pWindow = new sf::RenderWindow( videomode, WINDOW_TITLE, sf::Style::Titlebar | sf::Style::Close  );

	if ( pSettings->m_bVSync )
	{
		m_pWindow->setVerticalSyncEnabled( true );
	}
	else if ( pSettings->m_bLimitFPS )
	{
		m_pWindow->setFramerateLimit( pSettings->m_iFPSCountLimit );
	}

	//m_pWindow->setKeyRepeatEnabled( false );
}

void CWindowManager::HandleEvents( CInput* pInput )
{
	sf::Event e;
	while ( m_pWindow->pollEvent( e ) )
	{
		switch ( e.type ) 
		{
		case sf::Event::Closed:
			m_bWindowClosed = true;
		case sf::Event::GainedFocus:
			m_bFocus = true;
		case sf::Event::LostFocus:
			m_bFocus = false;
	
			/* Input handling */
		case sf::Event::KeyPressed:
	//		if ( HasFocus() )
				pInput->InjectKeyPress( e.key.code, e.key.shift, e.key.control, e.key.alt );
		case sf::Event::MouseMoved:
				pInput->InjectMouseMove( e.mouseMove.x, e.mouseMove.y ); 
		}
	}
}

void CWindowManager::Begin()
{
	if ( m_bWindowCreated && m_pWindow )
		m_pWindow->clear();
}

void CWindowManager::Present()
{
	if ( m_bWindowCreated && m_pWindow )
		m_pWindow->display();
}

bool CWindowManager::WindowClosed()
{
	return m_bWindowClosed;
}

bool CWindowManager::HasFocus()
{
	return m_bFocus;
}
Vector2i CWindowManager::GetScreenCentre()
{
	return m_vSize / 2;
}
Vector2i CWindowManager::GetScreenDimensions()
{
	return m_vSize;
}
void CWindowManager::Draw( sf::Drawable* Item )
{
	m_pWindow->draw( *Item );
}
void CWindowManager::RestoreDefaultCamera()
{
	m_pWindow->setView( m_pWindow->getDefaultView() );
}
void CWindowManager::MoveCamera( Vector2i CentrePos, int TileSize)
{
	Vector2i ScreenCentre = this->GetScreenCentre();
	CentrePos.X *= TileSize;
	CentrePos.Y *= TileSize;
	sf::View view( sf::Vector2f( (float)CentrePos.X + (float)TileSize / 2, (float)CentrePos.Y + (float)TileSize / 2 ), sf::Vector2f((float)m_vSize.X, (float)m_vSize.Y) );
	m_pWindow->setView( view );
}
sf::RenderWindow* CWindowManager::GetWindow()
{
	return m_pWindow;
}