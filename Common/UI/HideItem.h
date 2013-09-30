#ifndef HIDEITEM_H
#define HIDEITEM_H

/*******************************************************
 * (c) 1999, Tim Vernum <zodsoft@kagi.com>
 * All rights reserved
 *******************************************************
 * File   : HideItem.h
 * Author : Tim Vernum
 * Project: Trax Application
 * Purpose: Functions for hiding/showing views without nesting
 * Licence: AUL
 *******************************************************/

#include <View.h>
#include <Window.h>
#include <stdio.h>

inline void HideItem( BView * view )
{
	while( !view->IsHidden() )
	{
		view->Hide() ;
	}
}

inline void ShowItem( BView * view )
{
	if( view->Parent() && view->Parent()->IsHidden() )
		return ;
	if( view->Window() && view->Window()->IsHidden() )
		return ;

	while( view->IsHidden() )
	{
		view->Show() ;
	}
}

#endif

