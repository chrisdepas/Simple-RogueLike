#ifndef __INPUT_H__
#define __INPUT_H__

#include <map>
#include "action.h"
#include "vector.h"
#include <vector>
#include <SFML/Window.hpp>

class CGame;

class CInput
{
	std::map< sf::Keyboard::Key, EAction > m_KeyMap;
	std::map< sf::Keyboard::Key, EAction > m_DefaultMap;
	Vector2i m_vMousePosition;
	std::vector< EAction > m_vAction;

	/* Set up the default key bindings (m_DefaultMap)*/
	void LoadDefaultKeys();

public:
	CInput();
	~CInput();
	
	void Initialise();

	/* Read in / Save key bindings from file */
	void LoadBindings( char* szFileName );
	void SaveBindings( char* szFileName );
	
	/* Should be called by window loop */
	void InjectKeyPress( sf::Keyboard::Key Key, bool bShift, bool bCtrl, bool bAlt );
	void InjectKeyRelease( sf::Keyboard::Key Key, bool bShift, bool bCtrl, bool bAlt );
	void InjectMousePress( int x, int y );
	void InjectMouseRelease( int x, int y );
	void InjectMouseMove( int x, int y );
	void InjectMouseWheel( int delta ); 
	void LostFocus();

	/* Get next action. repeat until ACTION_NONE is returned */
	EAction GetNextAction(); 

	Vector2i GetMousePosition();

	/* Get a character, !blocking!, returns false on ESC press */
	bool GetChar( char& c, CGame* pGame );
};
#endif