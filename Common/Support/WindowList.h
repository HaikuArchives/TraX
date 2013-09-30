#ifndef WINDOWLIST_H
#define WINDOWLIST_H

#ifndef   BENAPHORE_H
#include "Benaphore.h"
#endif

/*******************************************************
 * (c) 1999, Tim Vernum <zodsoft@kagi.com>
 * All rights reserved
 *******************************************************
 * File   : WindowList.h
 * Author : Tim Vernum
 * Project: Trax Application
 * Purpose: Class to manage a list of windows open in an application
 * Licence: LGPL
 *******************************************************/

#include <List.h>

class BWindow ;

class WindowList
{
	public:
		WindowList( void ) ;
		
		void		AddWindow( BWindow * ) ;

		uint32		GetIndex( BWindow * ) ;
		BWindow *	GetWindow( uint32 ) ;

		BWindow *	RemoveWindow( BWindow * ) ;
		BWindow *	RemoveWindow( uint32 ) ;

		void		CloseWindow( BWindow * ) ;
		void		CloseWindow( uint32 ) ;
		void		CloseAll( void ) ;

		void		HideAllVisible( bool force ) ;
		void		ShowAllHidden( bool force ) ;

		uint32		CountWindows( void ) const ;
	
		void		Broadcast( BMessage * ) const ;
		void		Broadcast( uint32 ) const ;
	
	private:
		void		Lock() const ;
		void		Unlock() const ;

		mutable	Benaphore	fLock ;
		BList 				fList ;
} ;	

#endif