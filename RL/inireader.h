#ifndef __INIREADER_H__
#define __INIREADER_H__

#define MAX_INI_ITEMS 64
#include <vector>
struct CIniItem
{
	char*	m_szKey;
	char*	m_szValue;

	bool	MatchesKey(char* Key); /* Check if this this is the keyname supplied */
	bool	GetBool(); /* Get boolean representation of value */
	int		GetInt(); /* Get integer representation of value */
	float	GetFloat(); /* Get float representation of value */
	char*	GetString(); /* Gets string representation of value. !Don't save! may be free'd */
};
class CIniReader
{
	std::vector< CIniItem* > m_Items;
	void	ClearItems();
public:	
	CIniReader();
	~CIniReader();
	CIniItem* GetItem( char* Key );
	bool	ItemExists( char* Key );
	bool	ParseFile( char* FileName );
	bool	GetBoolValue( char* Key, bool& ret );
	bool	GetIntValue( char* Key, int& ret );
	bool	GetFloatValue( char* Key, float& ret );
	/* Get string representation of value. Dont save! may be freed*/
	bool    GetStringValue( char* Key, char*& ret );
};
#endif