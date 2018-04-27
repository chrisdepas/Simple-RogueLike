#ifndef __MESSAGELIST_H__
#define __MESSAGELIST_H__

class CGame;
#define MESSAGELIST_BUFFER_SIZE 5
#define MESSAGELIST_TEXT_SIZE 12
class CMessageList
{
	int m_iNumMessages;
	char* m_pszMessageBuffer[MESSAGELIST_BUFFER_SIZE];

public:

	void Draw( CGame* pGame );
	CMessageList();
	void AddMessage( char* szMessage );
};

#endif