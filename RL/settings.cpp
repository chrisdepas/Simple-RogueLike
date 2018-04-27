#include "stdafx.h"
#include "settings.h"

CSettings::CSettings()
{
	m_vScreenSize.X		= DEFAULT_SCREEN_WIDTH;
	m_vScreenSize.Y		= DEFAULT_SCREEN_HEIGHT;
	m_bFullscreen		= DEFAULT_FULLSCREEN;
	m_bLimitFPS			= DEFAULT_LIMIT_FPS;
	m_iFPSCountLimit	= DEFAULT_FPS_COUNT_LIMIT;
	m_bVSync			= DEFAULT_VSYNC;

	/* Debug settings */
	m_bFullbright		= false
		;
}

CSettings::~CSettings()
{
}

void CSettings::LoadFromFile( char* szFileName, CIniReader* pIniReader )
{
	if ( !pIniReader )
		return;
	if ( !pIniReader->ParseFile( szFileName ) )
		return;

	if ( pIniReader->ItemExists( SCREEN_WIDTH_STRING ) )
	{
		pIniReader->GetIntValue( SCREEN_WIDTH_STRING, m_vScreenSize.X ); 
		if ( m_vScreenSize.X < SCREEN_WIDTH_MIN || m_vScreenSize.X > SCREEN_WIDTH_MAX )
			m_vScreenSize.X = DEFAULT_SCREEN_WIDTH;
	}

	if ( pIniReader->ItemExists( SCREEN_HEIGHT_STRING ) )
	{
		pIniReader->GetIntValue( SCREEN_HEIGHT_STRING, m_vScreenSize.Y ); 
		if ( m_vScreenSize.Y < SCREEN_HEIGHT_MIN || m_vScreenSize.Y > SCREEN_HEIGHT_MAX )
			m_vScreenSize.Y = DEFAULT_SCREEN_HEIGHT;
	}

	if ( pIniReader->ItemExists( FULLSCREEN_STRING ) )
		pIniReader->GetBoolValue( FULLSCREEN_STRING, m_bFullscreen ); 
	
	
	if ( pIniReader->ItemExists( FPS_COUNT_LIMIT_STRING ) )
	{
		pIniReader->GetIntValue( FPS_COUNT_LIMIT_STRING, m_iFPSCountLimit ); 
		if ( m_iFPSCountLimit < FPS_COUNT_LIMIT_MIN || m_iFPSCountLimit > FPS_COUNT_LIMIT_MAX )
			m_iFPSCountLimit = DEFAULT_FPS_COUNT_LIMIT;
	}

	if ( pIniReader->ItemExists( VSYNC_STRING ) )
		pIniReader->GetBoolValue( VSYNC_STRING, m_bVSync ); 

	if ( pIniReader->ItemExists( LIMIT_FPS_STRING ) )
		pIniReader->GetBoolValue( LIMIT_FPS_STRING, m_bLimitFPS ); 
}

void CSettings::SaveToFile( char* szFileName, CIniWriter* pIniWriter )
{ 
	pIniWriter->StoreComment( "RL Settings file" );

	pIniWriter->StoreSectionHeader( "VIDEO" );

	pIniWriter->StoreBoolItem( FULLSCREEN_STRING, m_bFullscreen );
	
	pIniWriter->StoreBoolItem( LIMIT_FPS_STRING, m_bLimitFPS );
	pIniWriter->StoreIntItem( FPS_COUNT_LIMIT_STRING, m_iFPSCountLimit );
	
	pIniWriter->StoreBoolItem( VSYNC_STRING, m_bVSync );
	
	pIniWriter->StoreIntItem( SCREEN_WIDTH_STRING, m_vScreenSize.X );
	pIniWriter->StoreIntItem( SCREEN_HEIGHT_STRING, m_vScreenSize.Y );

	pIniWriter->WriteToFile( szFileName );
}	