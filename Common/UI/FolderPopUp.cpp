/*******************************************************
 * (c) 1999, Tim Vernum <zodsoft@kagi.com>
 * All rights reserved
 *******************************************************
 * File   : FolderPopUp.cpp
 * Author : Tim Vernum
 * Project: Trax Application
 * Purpose: Class to provide a popup menu for a user to
 *           select a folder from, either a predefined
 *           folder, or another of the user's choice.
 * Licence: LGPL
 *******************************************************/

#ifndef   FOLDERPOPUP_H
#include "FolderPopUp.h"
#endif

#ifndef   SUPPORT_H
#include "Support.h"
#endif

#include <MenuItem.h>
#include <PopUpMenu.h>
#include <Path.h>
#include <FilePanel.h>
#include <Window.h>
 
#include <stdio.h>

const uint32 kMSG_FolderSelected = 'FSel' ;
const uint32 kMSG_OtherSelected = 'OSel' ;

const uint32 kMSG_SelectFolder = 'SelF' ;
const uint32 kMSG_FolderChosen = 'FChn' ;
	
class FPUFolderItem : public BMenuItem
{
	public:
		FPUFolderItem( const entry_ref * , const char *, const uint32 =kMSG_FolderSelected ) ;

	const entry_ref&  GetRef( entry_ref * = NULL ) ;

	private:
		entry_ref fRef;
} ;

FPUFolderItem :: FPUFolderItem( const entry_ref * ref , const char * label, const uint32 what )
: 	BMenuItem( label, new BMessage( what ) ) ,
	fRef( *ref )
{
}

inline const entry_ref& FPUFolderItem :: GetRef( entry_ref * pRef )
{
	if( pRef )
		*pRef = fRef ;
	return fRef ;
}

property_info FolderPopUp :: sPropList[] =
{ 
 	{
        "Folder",
		{ B_GET_PROPERTY, B_SET_PROPERTY, 0 } ,
		{ B_DIRECT_SPECIFIER , 0 },
		"Set/Get the selected folder by entry_ref"
	} ,
	NULL
} ;

FolderPopUp :: FolderPopUp( BRect r , const char * name )
:	BMenuField( r, name, static_cast<const char *>(NULL),
		static_cast<BMenu *>( new BPopUpMenu( "folder_popup" ) ) )
,	fOtherItem( NULL ) , fFilePanel( NULL )
,	fMessenger( this )
,	fScripter( "suite/vnd.Zod-FolderPopUp", sPropList )
{
	SetDivider( 0.0 ) ;
	Create() ;
}		

FolderPopUp :: FolderPopUp( BRect r , const char * name, const char * label  )
:	BMenuField( r, name, label,
		static_cast<BMenu *>( new BPopUpMenu( "folder_popup" ) ) )
,	fOtherItem( NULL ) , fFilePanel( NULL )
,	fMessenger( this )
,	fScripter( "suite/vnd.Zod-FolderPopUp", sPropList )
{
	SetDivider( StringWidth( label ) + 10 ) ;
	Create() ;
}

void FolderPopUp :: Create( void )
{
	SetEnabled( true ) ;

	fScripter.SetOwner( this ) ;

	Scripter::PropertyHandler_f f = Scripter::PropertyHandler_f(&FolderPopUp::HandleProperty) ;
	fScripter.SetHandler( "*", f ) ;
}

void FolderPopUp :: AttachedToWindow( void )
{
	int32 count = Menu()->CountItems();
	for( int32 i = 0 ; i < count ; i++ )
		Menu()->ItemAt(i)->SetTarget( this ) ;

	if( Menu()->FindMarked() == NULL )
	{
		BMenuItem * mi = Menu()->FindItem( kMSG_FolderSelected ) ;
		if( mi )
			mi->SetMarked( true ) ;
	}
	fpCurrentSelection = Menu()->FindMarked() ;
	
	if( Label() )
	{
		float div = StringWidth( Label() ) + 10 ;
		SetDivider( div ) ;
	}
}

void FolderPopUp :: GetPreferredSize( float * w, float * h )
{
	inherited::GetPreferredSize( w, h ) ;

	float width = 0 ;
	int32 count = Menu()->CountItems();
	for( int32 i = 0 ; i < count ; i++ )
	{
		float sw = StringWidth( Menu()->ItemAt(i)->Label() ) ;
		width = MathUtils::max(	sw , width ) ;
	}
	width += StringWidth( Label() ) + 25 ;

	*w = MathUtils::max( *w, width ) ;
}

void FolderPopUp :: ResizeToPreferred( void )
{
	inherited:: ResizeToPreferred( ) ;

	if( Label() )
	{
		float div = StringWidth( Label() ) + 10 ;
		SetDivider( div ) ;
	}
}

void FolderPopUp :: AddFolder( const char * path )
{
	entry_ref ref ;
	get_ref_for_path( path, &ref  ) ;

	FPUFolderItem * fi = new FPUFolderItem( &ref, path ) ;
	Menu()->AddItem( fi ) ;
	fi->SetTarget( this ) ;
}
	
void FolderPopUp :: AddFolder( const char * path, uint32 at )
{
	entry_ref ref ;
	get_ref_for_path( path, &ref  ) ;

	FPUFolderItem * fi = new FPUFolderItem( &ref, path ) ;
	Menu()->AddItem( fi, at ) ;
	fi->SetTarget( this ) ;
}

void FolderPopUp :: AddFolder( BPath * path )
{
	AddFolder( path->Path() ) ;
}

void FolderPopUp :: AddFolder( BPath * path, uint32 at )
{
	AddFolder( path->Path(), at ) ;
}

void FolderPopUp :: AddSeparator()
{
	Menu()->AddSeparatorItem() ;
}

void FolderPopUp :: AddChoice( const char * text )
{
	Menu()->AddItem( new BMenuItem( text, new BMessage( kMSG_SelectFolder ) ) ) ;
}

void FolderPopUp :: MessageReceived( BMessage * msg )
{
	if( fScripter.HandleMessage( msg ) )
		return ;

	switch( msg->what )
	{
		// Add new folder via 'choice'
		case kMSG_SelectFolder:
		{
			if( !fpCurrentSelection )
				fpCurrentSelection = Menu()->FindItem( kMSG_FolderSelected ) ;
			if( fpCurrentSelection )
				fpCurrentSelection->SetMarked( true ) ;

			if( !fFilePanel )
			{			
				entry_ref ref ;
				if( fpCurrentSelection )
				{
					reinterpret_cast<FPUFolderItem*>(fpCurrentSelection)->GetRef( &ref ) ;
				}
				else
					get_ref_for_path( "/", &ref ) ;
				BMessage openMsg( kMSG_FolderChosen ) ;
				
				fFilePanel = new BFilePanel ( B_OPEN_PANEL,
								&fMessenger,  &ref ,
								B_DIRECTORY_NODE, false,
								&openMsg, NULL , true , true ) ;
				fFilePanel->SetTarget( this ) ;
			}
			fFilePanel->Show() ;
		}
		break ;
			
		case B_CANCEL:
		{
			delete fFilePanel ;
			fFilePanel = NULL ;
			break ;
		}

		// Folder Panel has chosen new folder
		case kMSG_FolderChosen:
		{
			delete fFilePanel ;
			fFilePanel = NULL ;
			
			entry_ref ref ;
			if( msg->FindRef( "refs", &ref ) != B_OK )
			{
				SetChoice( static_cast<entry_ref*>(NULL) ) ;
				return ;
			}

			SetChoice( &ref ) ;
		}
		break ;
		
		case kMSG_FolderSelected:
		case kMSG_OtherSelected:
		{
			msg->FindPointer( "source", reinterpret_cast<void**>(&fpCurrentSelection) ) ;
			BMenuItem * mi ;
			do	{
				mi = Menu()->FindMarked() ;
				if( mi )
					mi->SetMarked( false ) ;				
			} while( mi ) ;

			fpCurrentSelection->SetMarked( true ) ;
		}
		break ;

		default:
			inherited :: MessageReceived( msg ) ;
			break ;
	}	
}		

bool FolderPopUp :: GetRef( entry_ref * pRef )
{
	FPUFolderItem *fi = dynamic_cast<FPUFolderItem*>( Menu()->FindMarked() );
	if( fi == NULL )
		return false ;

	fi->GetRef( pRef ) ;
	return true ;
}

bool FolderPopUp :: RefAt( uint32 i, entry_ref * pRef )
{
	FPUFolderItem *fi = dynamic_cast<FPUFolderItem*>( Menu()->ItemAt(i) ) ;
	if( fi == NULL || pRef == NULL )
		return false ;
	
	fi->GetRef( pRef ) ;
	return true ;
}

void FolderPopUp :: RemoveFolder( uint32 i )
{
	Menu()->RemoveItem( i ) ;
}

void FolderPopUp :: RemoveFolder( entry_ref * ref )
{
	int32 count = Menu()->CountItems();
	for( int32 i = 0 ; i < count ; i++ )
	{
		FPUFolderItem *fi = dynamic_cast<FPUFolderItem*>( Menu()->ItemAt(i) ) ;
		entry_ref item_ref ;
		fi->GetRef( &item_ref ) ;
		if( item_ref == (*ref) )
		{
			RemoveFolder(i);
			count = Menu()->CountItems();
			i-- ;
		}
	}
}	
	
void FolderPopUp :: SetChoice( const entry_ref * ref )
{
	if( fOtherItem )
	{
		Menu()->RemoveItem( fOtherItem ) ;
		delete fOtherItem ;
		if( fpCurrentSelection == fOtherItem )
			fpCurrentSelection = NULL ;
		fOtherItem = NULL ;
	}
	if( ref == NULL )
	{
		fpCurrentSelection = Menu()->FindItem( kMSG_FolderSelected ) ;
		if( fpCurrentSelection )
			fpCurrentSelection->SetMarked( true ) ;
		return ;
	}
	BMenuItem * mi = Menu()->FindItem( kMSG_SelectFolder ) ;
	if( mi )
		mi->SetMarked( false ) ;

	BEntry ent( ref ) ;
	BPath path ;
	ent.GetPath( &path ) ;

	fOtherItem = new FPUFolderItem( ref, path.Path(), kMSG_OtherSelected ) ;
	Menu()->AddItem( fOtherItem ) ;
	fOtherItem->SetTarget( this ) ;
	
	do	{
		mi = Menu()->FindMarked() ;
		if( mi )
			mi->SetMarked( false ) ;
	} while( mi ) ;

	fOtherItem->SetMarked( true ) ;
	fpCurrentSelection = fOtherItem ;
}

void FolderPopUp :: SetChoice( const char * path )
{
	entry_ref ref ;
	if( get_ref_for_path( path, &ref ) == B_OK )
		SetChoice( &ref ) ;
}

void FolderPopUp :: SetViewColor( rgb_color col )
{
	inherited::SetViewColor( col ) ;
	SetLowColor( col ) ;
	Invalidate() ;
}

status_t FolderPopUp :: GetSupportedSuites( BMessage * msg )
{
	status_t s = fScripter.GetSupportedSuites( msg ) ;

	if( s == B_OK )
		return inherited ::GetSupportedSuites( msg );
	else
		return s ;
}

BHandler * FolderPopUp :: ResolveSpecifier(
	BMessage * msg , int32 idx, BMessage * spec, int32 what, const char * prop )
{
	BHandler * h = fScripter.ResolveSpecifier( msg, idx, spec, what, prop ) ;

	if( h )
		return h ;
	else
		return inherited::ResolveSpecifier( msg, idx, spec, what, prop ) ;
}
	
bool FolderPopUp :: HandleProperty( const char * prop, BMessage * msg )
{
	if( strcmp( prop, "Folder" ) == 0 )
	{
		switch( msg->what )
		{
			case B_SET_PROPERTY:
			{
				entry_ref ref ;
				if( msg->FindRef( "data", &ref ) != B_OK )
				{
					msg->SendReply( B_MESSAGE_NOT_UNDERSTOOD ) ;
				}
				else
				{
					BMessage reply( B_REPLY ) ;
					SetChoice( &ref ) ;
					reply.AddInt32( "error", 0 ) ;
					msg->SendReply( &reply ) ;
				}
				break ;
			}

			case B_GET_PROPERTY:
			{
				entry_ref ref ;
				GetRef( &ref ) ;
				BMessage reply( B_REPLY ) ;
				reply.AddRef( "result", &ref ) ;
				msg->SendReply( &reply ) ;
				break ;
			}
			
			default:
			{
				msg->SendReply( B_MESSAGE_NOT_UNDERSTOOD ) ;
				break ;
			}
		}
		return true ;
	}

	return false ;
}

