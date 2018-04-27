#ifndef __INIWRITER_H__
#define __INIWRITER_H__

#include <vector>
#define MAX_ITEM_SIZE 256

class CIniWriter
{
	std::vector< char* > m_Items;
public:
	~CIniWriter();
	void ClearItems();
	void StoreBoolItem( char* szKey, bool bValue );
	void StoreIntItem( char* szKey, int iValue );
	void StoreStringItem( char* szKey, char* szValue );
	void StoreFloatItem( char* szKey, float fValue );
	void StoreComment( char* szComment );
	void StoreSectionHeader( char* szSectionName );
	void WriteToFile( char* szFileName );
};
#endif