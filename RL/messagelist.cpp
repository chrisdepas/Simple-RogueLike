#include "stdafx.h"
#include "messagelist.h"
#include "game.h"

void CMessageList::Draw( CGame* pGame )
{
	int yoffset = 0;
	for ( int i = 0; i < m_iNumMessages; i++ )
	{
		pGame->m_Drawing.DrawText( &pGame->m_WindowManager, m_pszMessageBuffer[i], 0, yoffset, MESSAGELIST_TEXT_SIZE, 255, 255, 255, 255 );
		yoffset += MESSAGELIST_TEXT_SIZE;
	}
}
CMessageList::CMessageList()
{
	m_iNumMessages = 0;
}
void CMessageList::AddMessage( char* szMessage )
{
	if ( !szMessage )
		return;

	/* We have room, add message to queue */
 	if ( m_iNumMessages < MESSAGELIST_BUFFER_SIZE )
	{
		int slen = strlen( szMessage );
		m_pszMessageBuffer[m_iNumMessages] = (char*)malloc( slen+1 );
		memcpy( m_pszMessageBuffer[m_iNumMessages], szMessage, slen );
		m_pszMessageBuffer[m_iNumMessages][slen] = 0;
		m_iNumMessages++;
		return;
	}
	
	/* Already have max number of messages, delete oldest one */
	free( m_pszMessageBuffer[0] );
	
	/* Shift down */
	for ( int i = 0; i < m_iNumMessages-1; i++ )
	{
		m_pszMessageBuffer[i] = m_pszMessageBuffer[i+1];
	}

	/* Decrement amount of messages - one has been removed */
	m_iNumMessages--;

	/* Add message to queue */
	AddMessage( szMessage );
}
