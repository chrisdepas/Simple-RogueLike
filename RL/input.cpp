#include "stdafx.h"
#include "input.h"
#include "game.h"

void CInput::LoadDefaultKeys()
{
	/* Menu buttons */
	m_DefaultMap.insert( std::make_pair( sf::Keyboard::Escape, Action_Menu_Toggle )	 ); // Escape = Action_Menu_Toggle 
	m_DefaultMap.insert( std::make_pair( sf::Keyboard::Return, Action_Menu_Select )  ); // Return = Action_Menu_Select

	/* Movement buttons */
	m_DefaultMap.insert( std::make_pair( sf::Keyboard::Up, Action_Move_Forward ) );		// UpArrow = forward
	m_DefaultMap.insert( std::make_pair( sf::Keyboard::Down, Action_Move_Back ) );		// DownkArrow = back
	m_DefaultMap.insert( std::make_pair( sf::Keyboard::Left, Action_Move_Left ) );		// LeftArrow = move left 
	m_DefaultMap.insert( std::make_pair( sf::Keyboard::Right, Action_Move_Right ) );	// RightArrow = move right
	m_DefaultMap.insert( std::make_pair( sf::Keyboard::Add, Action_Move_Up ) );			// Plus = move Up 
	m_DefaultMap.insert( std::make_pair( sf::Keyboard::Subtract, Action_Move_Down ) );	// Minus = move down

	/* Action buttons */
	m_DefaultMap.insert( std::make_pair( sf::Keyboard::G, Action_Grab ) );				// G = Grab
	m_DefaultMap.insert( std::make_pair( sf::Keyboard::D, Action_Drop ) );				// D = drop
	m_DefaultMap.insert( std::make_pair( sf::Keyboard::E, Action_Eat ) );				// E = eat
	m_DefaultMap.insert( std::make_pair( sf::Keyboard::Q, Action_Equip ) );				// Q = eQuip
	m_DefaultMap.insert( std::make_pair( sf::Keyboard::Period, Action_Wait ) );			// . = wait

	m_DefaultMap.insert( std::make_pair( sf::Keyboard::F, Action_Fire ) );			// F = Fire
}
CInput::CInput()
{
	
}
CInput::~CInput()
{
}
void CInput::Initialise()
{
	LoadDefaultKeys();
}
void CInput::LoadBindings( char* szFileName )
{
 
}
void CInput::SaveBindings( char* szFileName )
{
}

void CInput::InjectKeyPress( sf::Keyboard::Key Key, bool bShift, bool bCtrl, bool bAlt )
{
	std::map< sf::Keyboard::Key, EAction >::iterator it;
	
	/* Find key in user-bound keymap */
	it = m_KeyMap.find( Key );
	
	/* Check if it has been bound*/
	if ( it != m_KeyMap.end() )
	{
		/* Key has been bound, store action */
		m_vAction.push_back( it->second );
		return;
	}

	/* Find key in default keymap */
	it = m_DefaultMap.find( Key );


	/* Check if key has been bound */
	if ( it != m_DefaultMap.end() )
		/* Key bound, store action */
		m_vAction.push_back( it->second );
}
void CInput::InjectKeyRelease( sf::Keyboard::Key Key, bool bShift, bool bCtrl, bool bAlt )
{

}
void CInput::InjectMouseMove( int x, int y )
{
	m_vMousePosition.X = x;
	m_vMousePosition.Y = y;
}
void CInput::LostFocus()
{

}
EAction CInput::GetNextAction()
{
	if ( m_vAction.empty() )
		return Action_None;
	EAction retaction = m_vAction.back();
	m_vAction.pop_back();
	return retaction;
}
Vector2i CInput::GetMousePosition()
{
	return m_vMousePosition;
}
bool CInput::GetChar( char& c, CGame* pGame )
{
	sf::Event e;
	sf::Window* pWindow = pGame->m_WindowManager.GetWindow();
	if ( !pWindow )
		return false;

	/* loop until key is found or ESC is pressed */
	while ( true )
	{
		if ( !pWindow->pollEvent( e ) )
			continue;

		if ( e.type == sf::Event::KeyPressed )
		{
			/* check if user pressed ESC to cancel */
			if ( e.key.code == sf::Keyboard::Escape )
				return false;
		}
		else if ( e.type == sf::Event::TextEntered )
		{
			if ( e.text.unicode < 128 )
			{
				/* Ascii character was entered, return it */
				c = e.text.unicode;
				return true;
			}
		}
	}
}