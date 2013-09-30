/*******************************************************
 * (c) 1999, Tim Vernum <zodsoft@kagi.com>
 * All rights reserved
 *******************************************************
 * File   : Support.cpp
 * Author : Tim Vernum
 * Project: Trax Application
 * Purpose: General Support functions
 * Licence: AUL
 *******************************************************/

//=================
// Local Headers
#include "Support.h"

//=================
// BeOS Headers
#include <Errors.h>

//=================
// ANSI/POSIX Headers
#include <string.h>
#include <ctype.h>
#include <stdio.h>


//=================
//String Utils
Benaphore StringUtils:: fStrErrorLock( "strerror" ) ;

char * StringUtils:: ErrorString( status_t e , char * str )
{
	fStrErrorLock.Lock() ;
	{
		strcpy( str, strerror( e ) ) ;
	}
	fStrErrorLock.Unlock() ;

	return str ;
}

char * StringUtils:: ErrorString( status_t e , char * str , size_t sz )
{
	fStrErrorLock.Lock() ;
	{
		strncpy( str, strerror( e ), sz ) ;
	}
	fStrErrorLock.Unlock() ;

	str[sz-1] = 0 ;
	return str ;
}

char * StringUtils:: VerboseErrorString( status_t e , char * str )
{
	switch( e )
	{
		case B_ERROR:
			strcpy( str, "B_ERROR " ) ;
			break ;

		case B_NO_ERROR:
			strcpy( str, "B_NO_ERROR " ) ;
			break ;
			
//----------------------------------

		case B_NO_MEMORY:
			strcpy( str, "B_NO_MEMORY " ) ;
			break ;

		case B_IO_ERROR:
			strcpy( str, "B_IO_ERROR " ) ;
			break ;
			
		case B_PERMISSION_DENIED:
			strcpy( str, "B_PERMISSION_DENIED " ) ;
			break ;

		case B_BAD_INDEX:
			strcpy( str, "B_BAD_INDEX " ) ;
			break ;

		case B_BAD_TYPE:
			strcpy( str, "B_BAD_TYPE " ) ;
			break ;

		case B_BAD_VALUE:
			strcpy( str, "B_BAD_VALUE " ) ;
			break ;

		case B_MISMATCHED_VALUES:
			strcpy( str, "B_MISMATCHED_VALUES " ) ;
			break ;

		case B_NAME_NOT_FOUND:
			strcpy( str, "B_NAME_NOT_FOUND " ) ;
			break ;

		case B_NAME_IN_USE:
			strcpy( str, "B_NAME_IN_USE " ) ;
			break ;

		case B_TIMED_OUT:
			strcpy( str, "B_TIMED_OUT " ) ;
			break ;

		case B_INTERRUPTED:
			strcpy( str, "B_INTERRUPTED " ) ;
			break ;

		case B_WOULD_BLOCK:
			strcpy( str, "B_WOULD_BLOCK " ) ;
			break ;

		case B_CANCELED:
			strcpy( str, "B_CANCELED " ) ;
			break ;

		case B_NO_INIT:
			strcpy( str, "B_NO_INIT " ) ;
			break ;

		case B_BUSY:
			strcpy( str, "B_BUSY " ) ;
			break ;

		case B_NOT_ALLOWED:
			strcpy( str, "B_NOT_ALLOWED " ) ;
			break ;

//----------------------------------

		case B_BAD_SEM_ID:
			strcpy( str, "B_BAD_SEM_ID " ) ;
			break ;

		case B_NO_MORE_SEMS:
			strcpy( str, "B_NO_MORE_SEMS " ) ;
			break ;

		case B_BAD_THREAD_ID:
			strcpy( str, "B_BAD_THREAD_ID " ) ;
			break ;

		case B_NO_MORE_THREADS:
			strcpy( str, "B_NO_MORE_THREADS " ) ;
			break ;

		case B_BAD_THREAD_STATE:
			strcpy( str, "B_BAD_THREAD_STATE " ) ;
			break ;

		case B_BAD_TEAM_ID:
			strcpy( str, "B_BAD_TEAM_ID " ) ;
			break ;

		case B_NO_MORE_TEAMS:
			strcpy( str, "B_NO_MORE_TEAMS " ) ;
			break ;

		case B_BAD_PORT_ID:
			strcpy( str, "B_BAD_PORT_ID " ) ;
			break ;

		case B_NO_MORE_PORTS:
			strcpy( str, "B_NO_MORE_PORTS " ) ;
			break ;

		case B_BAD_IMAGE_ID:
			strcpy( str, "B_BAD_IMAGE_ID " ) ;
			break ;

		case B_BAD_ADDRESS:
			strcpy( str, "B_BAD_ADDRESS " ) ;
			break ;

		case B_NOT_AN_EXECUTABLE:
			strcpy( str, "B_NOT_AN_EXECUTABLE " ) ;
			break ;

		case B_DEBUGGER_ALREADY_INSTALLED:
			strcpy( str, "B_DEBUGGER_ALREADY_INSTALLED " ) ;
			break ;

//----------------------------------

		case B_BAD_REPLY:
			strcpy( str, "B_BAD_REPLY " ) ;
			break ;

		case B_DUPLICATE_REPLY:
			strcpy( str, "B_DUPLICATE_REPLY " ) ;
			break ;

		case B_MESSAGE_TO_SELF:
			strcpy( str, "B_MESSAGE_TO_SELF " ) ;
			break ;

		case B_BAD_HANDLER:
			strcpy( str, "B_BAD_HANDLER " ) ;
			break ;

		case B_ALREADY_RUNNING:
			strcpy( str, "B_ALREADY_RUNNING " ) ;
			break ;

		case B_LAUNCH_FAILED:
			strcpy( str, "B_LAUNCH_FAILED " ) ;
			break ;

		case B_AMBIGUOUS_APP_LAUNCH:
			strcpy( str, "B_AMBIGUOUS_APP_LAUNCH " ) ;
			break ;

		case B_UNKNOWN_MIME_TYPE:
			strcpy( str, "B_UNKNOWN_MIME_TYPE " ) ;
			break ;

		case B_BAD_SCRIPT_SYNTAX:
			strcpy( str, "B_BAD_SCRIPT_SYNTAX " ) ;
			break ;

		case B_LAUNCH_FAILED_NO_RESOLVE_LINK:
			strcpy( str, "B_LAUNCH_FAILED_NO_RESOLVE_LINK " ) ;
			break ;

		case B_LAUNCH_FAILED_EXECUTABLE:
			strcpy( str, "B_LAUNCH_FAILED_EXECUTABLE " ) ;
			break ;

		case B_LAUNCH_FAILED_APP_NOT_FOUND:
			strcpy( str, "B_LAUNCH_FAILED_APP_NOT_FOUND " ) ;
			break ;

		case B_LAUNCH_FAILED_APP_IN_TRASH:
			strcpy( str, "B_LAUNCH_FAILED_APP_IN_TRASH " ) ;
			break ;

		case B_LAUNCH_FAILED_NO_PREFERRED_APP:
			strcpy( str, "B_LAUNCH_FAILED_NO_PREFERRED_APP " ) ;
			break ;

		case B_LAUNCH_FAILED_FILES_APP_NOT_FOUND:
			strcpy( str, "B_LAUNCH_FAILED_FILES_APP_NOT_FOUND " ) ;
			break ;

//------------------------------------------------------

		case B_FILE_ERROR:
			strcpy( str, "B_FILE_ERROR " ) ;
			break ;

		case B_FILE_NOT_FOUND:
			strcpy( str, "B_FILE_NOT_FOUND " ) ;
			break ;

		case B_FILE_EXISTS:
			strcpy( str, "B_FILE_EXISTS " ) ;
			break ;

		case B_ENTRY_NOT_FOUND:
			strcpy( str, "B_ENTRY_NOT_FOUND " ) ;
			break ;

		case B_NAME_TOO_LONG:
			strcpy( str, "B_NAME_TOO_LONG " ) ;
			break ;

		case B_NOT_A_DIRECTORY:
			strcpy( str, "B_NOT_A_DIRECTORY " ) ;
			break ;

		case B_DIRECTORY_NOT_EMPTY:
			strcpy( str, "B_DIRECTORY_NOT_EMPTY " ) ;
			break ;

		case B_DEVICE_FULL:
			strcpy( str, "B_DEVICE_FULL " ) ;
			break ;

		case B_READ_ONLY_DEVICE:
			strcpy( str, "B_READ_ONLY_DEVICE " ) ;
			break ;

		case B_IS_A_DIRECTORY:
			strcpy( str, "B_IS_A_DIRECTORY " ) ;
			break ;

		case B_NO_MORE_FDS:
			strcpy( str, "B_NO_MORE_FDS " ) ;
			break ;

		case B_CROSS_DEVICE_LINK:
			strcpy( str, "B_CROSS_DEVICE_LINK " ) ;
			break ;

		case B_LINK_LIMIT:
			strcpy( str, "B_LINK_LIMIT " ) ;
			break ;

		case B_BUSTED_PIPE:
			strcpy( str, "B_BUSTED_PIPE " ) ;
			break ;

		case B_UNSUPPORTED:
			strcpy( str, "B_UNSUPPORTED " ) ;
			break ;

//------------------------------------------------

		default:
			strcpy( str, "Unknown Error Code " ) ;
			break ;
	}
	char * s = StrEnd(str) ;

	fStrErrorLock.Lock() ;
	{
		sprintf( s, "(%ld) : %s", long(e), strerror( e ) ) ;
	}
	fStrErrorLock.Unlock() ;

	return str ;
}
 

char * StringUtils:: LowerCase( char * s )
{
	char * r = s ;
	while( *s )
	{
		*s = tolower(*s) ;
		s++ ;
	}
	return r ;
}
