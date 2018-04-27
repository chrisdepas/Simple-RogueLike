#include "stdafx.h"
#include "iniwriter.h"
#include <fstream>


CIniWriter::~CIniWriter()
{
	ClearItems();
}
void CIniWriter::ClearItems()
{
	for ( unsigned int i = 0; i < m_Items.size(); i++ )
	{
		if ( m_Items[i] )
			delete m_Items[i];
	}
	m_Items.clear();
}
void CIniWriter::StoreBoolItem( char* szKey, bool bValue ) /* TODO: ERROR HANDLING */
{
	char* thisitem = new char[MAX_ITEM_SIZE];

	if ( bValue )
		sprintf_s( thisitem, MAX_ITEM_SIZE, "%s=1\n", szKey );
	else
		sprintf_s( thisitem, MAX_ITEM_SIZE, "%s=0\n", szKey );
	m_Items.push_back( thisitem );
}
void CIniWriter::StoreIntItem( char* szKey, int iValue )
{
	char* thisitem = new char[MAX_ITEM_SIZE];
	sprintf_s( thisitem, MAX_ITEM_SIZE, "%s=%i\n", szKey, iValue );
	m_Items.push_back( thisitem );
}
void CIniWriter::StoreStringItem( char* szKey, char* szValue )
{
	char* thisitem = new char[MAX_ITEM_SIZE];
	sprintf_s( thisitem, MAX_ITEM_SIZE, "%s=%s\n", szKey, szValue );
	m_Items.push_back( thisitem );
}
void CIniWriter::StoreFloatItem( char* szKey, float fValue )
{
	char* thisitem = new char[MAX_ITEM_SIZE];
	sprintf_s( thisitem, MAX_ITEM_SIZE, "%s=%f\n", szKey, fValue );
	m_Items.push_back( thisitem );
}
void CIniWriter::StoreComment( char* szComment )
{
	char* thisitem = new char[MAX_ITEM_SIZE];
	sprintf_s( thisitem, MAX_ITEM_SIZE, ";%s\n", szComment );
	m_Items.push_back( thisitem );
}
void CIniWriter::StoreSectionHeader( char* szSectionName )
{
	char* thisitem = new char[MAX_ITEM_SIZE];
	sprintf_s( thisitem, MAX_ITEM_SIZE, "[%s]\n", szSectionName );
	m_Items.push_back( thisitem );
}
void CIniWriter::WriteToFile( char* szFileName )
{
	std::ofstream ini;
	ini.open( szFileName );
	if ( !ini.is_open() || ini.bad() )
		return;
	
	for ( unsigned int i = 0; i < m_Items.size(); i++ )
	{
		if ( m_Items[i] )
			ini.write( m_Items[i], strlen(m_Items[i]) );
	}

	ini.close();
}