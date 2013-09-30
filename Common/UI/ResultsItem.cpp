
/*******************************************************
 * (c) 1999, Tim Vernum <zodsoft@kagi.com>
 * All rights reserved
 *******************************************************
 * File   : ResultsItem.cpp
 * Author : Tim Vernum
 * Project: Trax Application
 * Purpose: Item that displays results of a Trax search
 * Licence: GPL
 *******************************************************/

#include "ResultsItem.h"

#ifndef   ICONS_H
#include "Icons.h"
#endif

#ifndef   COLOURS_H
#include "Colours.h"
#endif

#include <Font.h>
#include <NodeInfo.h>
#include <View.h>

#include <string.h>

ResultsItem:: ResultsItem( const char * path )
:	fIcon( BRect(0,0,15,15), B_CMAP8 )
,	fPath( strdup( path ) ) 
{
	get_ref_for_path( path, &fRef ) ;

	if( BNodeInfo::GetTrackerIcon( &fRef, &fIcon, B_MINI_ICON ) != B_OK )
		fIcon.SetBits( fRedCrossBits, 16,16, B_CMAP8 ) ;
}

void ResultsItem:: Update( BView * owner, const BFont *font )
{
	BListItem::Update( owner, font ) ;

	if( Height() < 20 )
		SetHeight( 20 ) ;
	
	float w = 25 + owner->StringWidth( fPath ) ;
	if( Width() > w )
		SetWidth( w ) ;
}

void ResultsItem:: DrawItem( BView * v, BRect r, bool b )
{
	r.InsetBy( 1,1 ) ;
	DrawIcon( v, r ) ;
	r.left += 20 ;
	DrawPath( v, r, b ) ;
}

void ResultsItem:: DrawIcon( BView * v, BRect r )
{
	drawing_mode dm = v->DrawingMode() ;
	v->SetDrawingMode( B_OP_OVER ) ;
	BPoint pt( r.left + 1 , r.bottom - 17 ) ;
	v->DrawBitmap( &fIcon, pt ) ;
	v->SetDrawingMode( dm ) ;
}

void ResultsItem:: DrawPath( BView * v, BRect r, bool b )
{
	r.right = r.left + v->StringWidth( fPath ) + 6 ;

	if ( IsSelected() )
	{ 
		v->SetLowColor( Colours::Lavender ); 
		v->FillRect( r, B_SOLID_LOW ); 
	} 
	else
	{
		v->SetLowColor( v->ViewColor() ); 
		if ( b )
		{ 
			v->FillRect( r, B_SOLID_LOW ) ; 
		} 
	}
	
	font_height fh ;
	v->GetFontHeight( &fh ) ;
	float h = fh.ascent + fh.descent + fh.leading * 2 ;
	BPoint pt( r.left + 3, r.bottom - r.Height()/2 + h/2 ) ;
	
	v->DrawString( fPath, pt ) ;
}
