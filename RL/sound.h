#ifndef __SOUND_H__
#define __SOUND_H__

#include <vector>
#include <SFML/Audio.hpp>

class CSoundItem
{
private:
	sf::Sound m_Sound;
	sf::SoundBuffer m_SoundBuffer;
	char* m_szSoundName;
	bool m_bCanLoad;
	bool m_bLoaded;

public:
	CSoundItem( char* m_szTexture );
	~CSoundItem();
	bool MatchesName( char* szTextureName );
	bool PlaySound();
	char* GetName();
};

class CSoundManager
{
	std::vector< CSoundItem* > m_vSounds;
public:
	bool PlaySound( int ID );
	int GetID( char* szSoundName );
};

#endif