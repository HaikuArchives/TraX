#include "WindowList.h"

#include <Window.h>
#include <Message.h>

/*******************************************************
 * (c) 1999, Tim Vernum <zodsoft@kagi.com>
 * All rights reserved
 *******************************************************
 * File   : WindowList.cpp
 * Author : Tim Vernum
 * Project: Trax Application
 * Purpose: Class to manage a list of windows open in an application
 * Licence: LGPL
 *******************************************************/

inline void WindowList :: Lock( void ) const
{
	fLock.Lock() ;
}

void WindowList :: Unlock( void ) const 
{
	fLock.Unlock() ;
}

WindowList :: WindowList( void )
:	fLock( "WindowList" )
,	fList( 5 )
{
}

void WindowList :: AddWindow( BWindow * win )
{
	Lock() ;
	fList.AddItem( win ) ;
	Unlock() ;
}

uint32 WindowList :: GetIndex( BWindow * win )
{
	Lock() ;
	uint32 i = fList.IndexOf( win ) ;
	Unlock() ;
	return i ;
}

BWindow * WindowList :: GetWindow( uint32 i )
{
	Lock() ;
	void * v = fList.ItemAt( i ) ;
	Unlock() ;
	return reinterpret_cast<BWindow *>( v ) ;
}
	
BWindow * WindowList :: RemoveWindow( BWindow * win )
{
	Lock() ;
	if( !fList.RemoveItem( win ) )
		win = NULL ;
	Unlock() ;
	return win ;
}

BWindow * WindowList :: RemoveWindow( uint32 i )
{
	Lock() ;
	void * v = fList.RemoveItem( i ) ;
	Unlock() ;
	return reinterpret_cast<BWindow *>( v ) ;
}

void WindowList :: CloseWindow( BWindow * win )
{
	Lock() ;
	if( !fList.RemoveItem( win ) )
		win = NULL ;
	Unlock() ;
	if( win )
		win->Quit() ;
}

void WindowList :: CloseWindow( uint32 i )
{
	Lock() ;
	void * v = fList.RemoveItem( i ) ;
	Unlock() ;
	if( v )
		reinterpret_cast<BWindow *>( v )->Quit() ;
}

void WindowList :: CloseAll( void )
{
	Lock() ;
	void * v ;
	while( (v = fList.RemoveItem( 0L )) != NULL )
	{
		reinterpret_cast<BWindow *>( v )->Quit() ;
	}
	Unlock() ;
}


/*
		void		HideAllVisible( bool force ) ;
		void		ShowAllHidden( bool force ) ;
*/

uint32 WindowList :: CountWindows( void ) const
{
	Lock() ;
	uint32 c = fList.CountItems() ;
	Unlock() ;
	return c ;
}

void WindowList :: Broadcast( BMessage * msg ) const
{
	Lock() ;

	int32 i = 0 ;
	void * v ;
	while( (v = fList.ItemAt( i )) != NULL )
	{
		BWindow * win = reinterpret_cast<BWindow *>( v );
		win->PostMessage( msg, win ) ;
		i++ ;
	}

	Unlock() ;
}

void WindowList :: Broadcast( uint32 what ) const
{
	BMessage msg( what ) ;
	return Broadcast( &msg ) ;
}

