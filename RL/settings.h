#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "vector.h"
#include "inireader.h"
#include "iniwriter.h"

#define SCREEN_WIDTH_STRING "ScreenWidth"
#define DEFAULT_SCREEN_WIDTH 800
#define SCREEN_WIDTH_MIN 100
#define SCREEN_WIDTH_MAX 100000

#define SCREEN_HEIGHT_STRING "ScreenHeight"
#define DEFAULT_SCREEN_HEIGHT 600
#define SCREEN_HEIGHT_MIN 100
#define SCREEN_HEIGHT_MAX 100000

#define FULLSCREEN_STRING "Fullscreen"
#define DEFAULT_FULLSCREEN false

#define LIMIT_FPS_STRING "LimitFPS"
#define DEFAULT_LIMIT_FPS false

#define FPS_COUNT_LIMIT_STRING "MaxFPS"
#define DEFAULT_FPS_COUNT_LIMIT 60
#define FPS_COUNT_LIMIT_MIN 30
#define FPS_COUNT_LIMIT_MAX 100000

#define VSYNC_STRING "VSyncEnabled"
#define DEFAULT_VSYNC false

class CSettings
{
public:
	Vector2i m_vScreenSize;
	bool m_bFullscreen;
	bool m_bLimitFPS;
	int m_iFPSCountLimit;
	bool m_bVSync;
	bool m_bFullbright;
	CSettings();
	~CSettings();
	void LoadFromFile( char* szFileName, CIniReader* pIniReader );
	void SaveToFile( char* szFileName, CIniWriter* pIniWriter );
};
#endif