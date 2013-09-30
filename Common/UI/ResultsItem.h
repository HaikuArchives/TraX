#ifndef RESULTSITEM_H
#define RESULTSITEM_H

#include <Bitmap.h>
#include <Entry.h>
#include <ListItem.h>

/*******************************************************
 * (c) 1999, Tim Vernum <zodsoft@kagi.com>
 * All rights reserved
 *******************************************************
 * File   : ResultsItem.h
 * Author : Tim Vernum
 * Project: Trax Application
 * Purpose: Item that displays results of a Trax search
 * Licence: GPL
 *******************************************************/

class ResultsItem : public BListItem
{
	public:
		 ResultsItem( const char * ) ;
		~ResultsItem( void ) ;

		void DrawItem( BView * , BRect , bool = false ) ;
		void Update( BView *, const BFont * ) ;
		const entry_ref * Ref() ; 
	
	private:
		void DrawIcon( BView * , BRect ) ;
		void DrawPath( BView * , BRect, bool ) ;

		BBitmap  	fIcon ;
		entry_ref 	fRef ;
		char *		fPath ;
} ;

inline const entry_ref * ResultsItem:: Ref()
{
	return &fRef ;
}

inline ResultsItem:: ~ResultsItem( )
{
	delete[] fPath ;
}

#endif
