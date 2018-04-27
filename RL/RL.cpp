#include "stdafx.h"

#include <SFML/Graphics.hpp>
#include "game.h"
int _tmain(int argc, _TCHAR* argv[])
{
	bool bRunning = true;
	printf("Welcome to RL\n");
		
	sf::Font f;
	f.loadFromFile( ".\\DejaVuSans.ttf" );
	sf::Text text;
	text.setFont( f );
	text.setString( "goon" );
	text.setCharacterSize( 24 );
	text.setColor( sf::Color::Cyan );

	printf("Initialising game\n");
	CGame Game;
	Game.Initialise();
	printf("Done.\nRunning!\n");

	while ( !Game.ShouldQuit() )
	{
		Game.Step();
		Game.HandleInput();
		Game.Draw();
	}

	return 0;
}

