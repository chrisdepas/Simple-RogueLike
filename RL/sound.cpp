#include "stdafx.h"
#include "sound.h"

CSoundItem::CSoundItem( char* szName )
{
	m_szSoundName = szName;
	m_bLoaded = false;
	m_bCanLoad = true;
}
bool CSoundItem::MatchesName( char* szName )
{
	if ( !szName || !m_szSoundName )
		return false;
	return !_stricmp( szName, m_szSoundName );
}
bool CSoundItem::PlaySound()
{
	/* This sound cannot be loaded, don't play it */
	if ( !m_bCanLoad )
		return false;

	/* If sound is loaded already, play it */
	if ( m_bLoaded )
	{
		m_Sound.setBuffer( m_SoundBuffer );
		m_Sound.setVolume( 100 );
		m_Sound.play();

		return true;
	}

	/* check if sound has a name */
	if ( !m_szSoundName )
		return false;

	/* Load sound */
	m_bCanLoad = m_bLoaded = m_SoundBuffer.loadFromFile( m_szSoundName );
	if ( !m_bLoaded )
		return false;

	/* Play sound */
	m_Sound.setBuffer( m_SoundBuffer );
	m_Sound.setVolume( 100 );
	m_Sound.play();

	return true;
}
int CSoundManager::GetID( char* szSoundName )
{
	/* If sound has already been saved, return the index */
	for (unsigned int i = 0; i < m_vSounds.size(); i++)
	{
		if ( m_vSounds[i]->MatchesName( szSoundName ) )
			return i;
	}

	/* sound hasn't been saved, add and return index */
	m_vSounds.push_back( new CSoundItem( szSoundName ) );
	return ( m_vSounds.size() - 1 );
}
bool CSoundManager::PlaySound( int id )
{
	return m_vSounds[id]->PlaySound();
}