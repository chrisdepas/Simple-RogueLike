#ifndef __GAME_H__
#define __GAME_H__

#include "window.h"
#include "drawing.h"
#include "gamestate.h"
#include "input.h"
#include "texture.h"
#include "random.h"
#include "time.h"
#include "sound.h"

class CGame
{
	bool				m_bInitialised;
public:
	CSettings			m_Settings;
	CWindowManager		m_WindowManager;
	CGameStateHandler	m_StateHandler;
	CDrawingManager		m_Drawing;
	CInput				m_Input;
	CIniReader			m_IniReader;
	CIniWriter			m_IniWriter;
	CRandom				m_Random;
	CTextureManager		m_TextureManager; 
	CTime				m_Time;
	CSoundManager		m_SoundManager;

	CGame();
	~CGame();
	void Initialise();
	bool ShouldQuit();
	void Draw();
	void HandleInput();
	void Step();
};
#endif