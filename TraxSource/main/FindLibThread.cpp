#include "FindLibThread.h"

#ifndef   FINDLIB_H
#include "findlib.h"
#endif

#ifndef   MESSAGES_H
#include "Messages.h"
#endif

#ifndef   RESULTSWINDOW_H
#include "ResultsWindow.h"
#endif

#ifndef   GLOBALS_H
#include "Globals.h"
#endif

#ifndef   SETTINGS_H
#include "Settings.h"
#endif

#include <Application.h>

/*
 *       Class: FindLibThread
 *   Copyright: Tim Vernum 1999, All rights reserved
 *     Licence: GPL
 */

using namespace Threads;

Benaphore FindLibThread::sLock( "findlib" ) ;
bool FindLibThread::sUseTracker = true ;

bool FindLibThread::InitFind( )
{
	if( sLock.Grab() != B_OK )
		return false ;

	be_app->PostMessage( Messages::LibInUse, be_app ) ;

	sUseTracker = gSettings.fUseTracker ;
	
	if( sUseTracker )
		findlib_set_print( ResultsHandler::AddResult ) ;
	else
	{
		findlib_set_print( ResultsWindow::AddResult ) ;

		BWindow * win = new ResultsWindow ;
		BMessage msg( Messages::WindowOpened ) ;
		msg.AddPointer( "Window", win ) ;
		be_app->PostMessage( &msg, be_app ) ;
		win->Show() ;
	}
	
	return true ;
}

FindLibThread :: FindLibThread( )
:	ThreadObject( "find" )
{
}

int32 FindLibThread :: PerformThreadTask( void )
{
	if( sUseTracker )
		gResults.NewSearch() ;

	char * paths[1] = { const_cast<char*>(Path.Path()) } ;
	printf( "Find in: %s\n", paths[0] ) ;
	int32 val = findlib_find( 1, paths ) ;
	findlib_end() ;
	printf( "---------\n" ) ;

	if( sUseTracker )
		gResults.EndSearch() ;

	return val ;
}

void FindLibThread :: ThreadFinished( int32 )
{
	sLock.Unlock() ;
	be_app->PostMessage( Messages::LibNotInUse, be_app ) ;
	delete this ;
}

void FindLibThread :: ThreadKilled( void )
{
	ThreadFinished( 0 ) ;
}

