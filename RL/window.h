#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <SFML/Graphics.hpp>
#include "settings.h"
#include "input.h"
#define WINDOW_TITLE "Window"

class CWindowManager
{
	Vector2i m_vSize;
	bool m_bWindowCreated;
	bool m_bFocus;
	sf::RenderWindow* m_pWindow;
	bool m_bWindowClosed;

public:
	CWindowManager();
	~CWindowManager();
	void Create( CSettings* Settings );
	void HandleEvents( CInput* pInput );
	sf::RenderWindow* GetWindow();

	/* Begin drawing */
	void Begin();
	/* Present scene */
	void Present();

	bool WindowClosed();
	bool HasFocus();
	Vector2i GetScreenCentre();
	Vector2i GetScreenDimensions();

	void Draw( sf::Drawable* Item );
	void RestoreDefaultCamera();
	void MoveCamera( Vector2i CentrePos, int TileSize);
};

#endif