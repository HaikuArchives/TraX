#include "Init.h"

/*
 *        File: Init.cpp
 *   Copyright: Tim Vernum 1999, All rights reserved
 *     Licence: GPL
 */


#ifndef   SCRIPTER_H
#include "Scripter.h"
#endif

#ifndef   FINDLIB_H
#include "findlib.h"
#endif

#ifndef   RESULTSWINDOW_H
#include "ResultsWindow.h"
#endif

#include <Application.h>
#include <Roster.h>

#ifndef   GLOBALS_H
#include "Globals.h"
#endif

void InitLibs( void )
{
	// Get AppInfo, for when we need to know our name etc.
	app_info ai ;
	be_app->GetAppInfo( &ai ) ;

	// Set default settings for the Scripter Class	
	Scripter::SetDefaultOptions(
		  Scripter::UseWildcard
		| Scripter::Capitalise
	) ;
	
	gResults.Init() ;
	
	// Set the findlib settings
	findlib_init( ai.ref.name ) ;
}
