#include "stdafx.h"
#include "inireader.h"
#include <fstream>

bool	CIniItem::MatchesKey(char* Key)
{
	return !_stricmp( Key, m_szKey );
}
bool	CIniItem::GetBool()
{
	if ( !m_szValue )
		return false;
	if ( !_stricmp( "false", m_szValue ) || !_stricmp( "f", m_szValue ) || !strcmp( "0", m_szValue ) )
		return false;
	if ( !_stricmp( "true", m_szValue ) || !_stricmp( "t", m_szValue ) || !strcmp( "1", m_szValue ) )
		return true;
	return false; 
	?
}
int		CIniItem::GetInt() 
{
	if ( m_szValue )
		return strtol( m_szValue, NULL, 0 );
	return 0;
}
float	CIniItem::GetFloat()
{
	if ( m_szValue )
		return (float)strtod( m_szValue, NULL );
	return 0.0f;
}
char*	CIniItem::GetString()
{
	return m_szValue;
}

CIniReader::CIniReader(){}
CIniReader::~CIniReader(){}
CIniItem* CIniReader::GetItem( char* Key )
{
	for (unsigned int i = 0; i < m_Items.size(); i++)
	{
		if ( !m_Items[i] )
			continue;
		if ( m_Items[i]->MatchesKey( Key ) )
			return m_Items[i];
	}
	return NULL;
}
bool	CIniReader::ItemExists( char* Key )
{
	for (unsigned int i = 0; i < m_Items.size(); i++)
	{
		if ( !m_Items[i] )
			continue;
		if ( m_Items[i]->MatchesKey( Key ) )
			return true;
	}
	return false;
}
void	CIniReader::ClearItems()
{
	if (m_Items.empty())
		return;

	for (unsigned int i = 0; i < m_Items.size(); i++)
	{
		if (m_Items[i])
			delete m_Items[i];
	}
	m_Items.clear();
}
bool	CIniReader::ParseFile( char* FileName )
{
	ClearItems();

	std::ifstream ini;
	ini.open( FileName );

	char thisline[1024];
	if ( !ini.is_open() || ini.bad() )
		return false;

	char* context = NULL;

	while ( ini.getline( thisline, 1024 ) )
	{
		/* Skip section headings or comment lines*/
		if ( thisline[0] == '[' || thisline[0] == ';' || thisline[0]=='#' )
		{
			continue;
		}
		/* Get key string */
		char* key = strtok_s( thisline, "=", &context );
		if ( !key || ItemExists(key) )
			continue;
		int keylen = strlen( key );
		
		/* Save copy of key to item */
		CIniItem* newitem = new CIniItem;
		newitem->m_szKey = new char[keylen + 1];
		memcpy( newitem->m_szKey, key, keylen );
		newitem->m_szKey[keylen] = 0;

		/* value of key */
		char* value = strtok_s( NULL, "=", &context );
		if ( !value ) 
		{
			delete newitem->m_szKey;
			delete newitem;
			continue;
		}
		int valuelen = strlen ( value );

		newitem->m_szValue = new char[valuelen + 1]; 
		memcpy( newitem->m_szValue, value, valuelen );
		newitem->m_szValue[valuelen] = 0;

		m_Items.push_back( newitem );
	}
	
	ini.close();

	return true;
}
bool	CIniReader::GetBoolValue( char* Key, bool& ret )
{
	CIniItem* item = GetItem( Key );
	if (!item)
		return false;
	ret = item->GetBool();
	return true;
}
bool	CIniReader::GetIntValue( char* Key, int& ret)
{
	CIniItem* item = GetItem( Key );
	if (!item)
		return false;
	ret = item->GetInt();
	return true;
}
bool	CIniReader::GetFloatValue( char* Key, float& ret )
{
	CIniItem* item = GetItem( Key );
	if (!item)
		return false;
	ret = item->GetFloat();
	return true;
}
bool	CIniReader::GetStringValue( char* Key, char*& ret )
{
	CIniItem* item = GetItem( Key );
	if (!item)
		return false;
	ret = item->GetString();
	return true;
}