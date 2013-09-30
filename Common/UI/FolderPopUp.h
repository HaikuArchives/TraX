#ifndef FOLDERPOPUP_H
#define FOLDERPOPUP_H

/*******************************************************
 * (c) 1999, Tim Vernum <zodsoft@kagi.com>
 * All rights reserved
 *******************************************************
 * File   : FolderPopUp.h
 * Author : Tim Vernum
 * Project: Trax Application
 * Purpose: Class to provide a popup menu for a user to
 *           select a folder from, either a predefined
 *           folder, or another of the user's choice.
 * Licence: LGPL
 *******************************************************/

// Define the foward decl, since it is no longer needed
#ifndef FOLDERPOPUP_FWD
#define FOLDERPOPUP_FWD
#endif

#include <MenuField.h>
#include <Messenger.h>

// Include scripting
#ifndef   SCRIPTER_H
#include "Scripter.h"
#endif

class BFilePanel ;
class BPath ;
struct entry_ref ;

class FolderPopUp : public BMenuField
{
	typedef BMenuField inherited ;

	public:
		FolderPopUp( BRect, const char * name ) ;
		FolderPopUp( BRect, const char * name , const char * label ) ;
		
		void AttachedToWindow( void ) ;
		void ResizeToPreferred( void ) ;
		void GetPreferredSize( float * , float * ) ;

		bool GetRef( entry_ref * ) ;
		
		void AddFolder( const char * ) ;
		void AddFolder( const char *, uint32 ) ;
		void AddFolder( BPath * ) ;
		void AddFolder( BPath *, uint32 ) ;

		void AddSeparator() ;
		void AddChoice( const char * ) ;

		void SetChoice( const entry_ref * ) ;
		void SetChoice( const char * ) ;

		void RemoveFolder( uint32 ) ;
		void RemoveFolder( entry_ref * ) ;

		void MessageReceived( BMessage * ) ;

		void SetViewColor( rgb_color ) ;
		
		status_t	GetSupportedSuites( BMessage * ) ;
		BHandler *	ResolveSpecifier( BMessage * , int32 , BMessage *,
		          	                  int32 , const char * ) ;
	protected:
		bool RefAt( uint32, entry_ref * ) ;


	private:
		void		Create( void ) ; 
		bool		HandleProperty( const char *, BMessage * ) ;

		BMenuItem  	*	fOtherItem ;
		BFilePanel	*	fFilePanel ;
		BMessenger   	fMessenger ;
		BMenuItem 	*	fpCurrentSelection ;

		static	property_info	sPropList[] ;
				Scripter		fScripter ;
} ;

#endif
