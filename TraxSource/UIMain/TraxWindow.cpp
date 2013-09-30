#ifndef   TRAXWINDOW_H
#include "TraxWindow.h"
#endif

/*
 *       Class: TraxWindow
 *   Copyright: Tim Vernum 1999, All rights reserved
 *     Licence: GPL
 */


#ifndef   FOLDERPANEL_H
#include "FolderPanel.h"
#endif

#ifndef   JOINPANEL_H
#include "JoinPanel.h"
#endif

#ifndef   PREDICATEPANEL_H
#include "PredicatePanel.h"
#endif

#ifndef   OPTIONSPANEL_H
#include "OptionsPanel.h"
#endif

#ifndef   MESSAGES_H
#include "Messages.h"
#endif

#ifndef   FINDLIB_H
#include "findlib.h"
#endif

#ifndef   FINDLIBTHREAD_H
#include "FindLibThread.h"
#endif

#ifndef   SETTINGSWINDOW_H
#include "SettingsWindow.h"
#endif

#include <Application.h>
#include <Beep.h>

TraxSettings TraxWindow::sSettings ;
BWindow * TraxWindow::spSettingsWindow = NULL ;

const uint32 MSG_Reshuffle = 'RShf' ;

TraxWindow :: TraxWindow( entry_ref * ref , uint32 n )
:	BWindow(
		BRect( 50+(n*15) , 50+(n*25) , 550+(n*15) , 150+(n*25) ) ,
		"TraX",
		B_TITLED_WINDOW , B_NOT_V_RESIZABLE )
{
	// This loads the settings
	if( !spSettingsWindow )
	{
		spSettingsWindow = new SettingsWindow ;
		spSettingsWindow->Hide();
		spSettingsWindow->Show();
	}

	fpFolderPanel = new FolderPanel( ref ) ;
	AddChild( fpFolderPanel ) ;
	
	JoinPanel * topJoin = new JoinPanel( fpFolderPanel->Frame().bottom+1 ) ;
	topJoin->SetPosition( JoinPanel::ePosTop ) ;
	AddChild( topJoin ) ;
	fPanels.AddItem( topJoin ) ;

	PredicatePanel * pred = new PredicatePanel( topJoin->Frame().bottom+1 ) ;
	AddChild( pred ) ;
	fPanels.AddItem( pred ) ;
	
	JoinPanel * bottomJoin = new JoinPanel( pred->Frame().bottom+1 ) ;
	bottomJoin->SetPosition( JoinPanel::ePosBottom ) ;
	AddChild( bottomJoin ) ;
	fPanels.AddItem( bottomJoin ) ;
	
	fpOptionsPanel = new OptionsPanel( bottomJoin->Frame().bottom+1 ) ;
	AddChild( fpOptionsPanel, topJoin ) ;
	/* Add before the topJoin panel, so that when scripting, the middle
	 * panels (of which there may be many) are in sequence.
	 * Thus the order is: Folder, Options, join, pred, join, pred, join ...
	 */

	AddShortcut( 'N', 0, new BMessage( Messages::NewWindow ) ) ;
	ReshufflePanels() ;
}

void TraxWindow::ReshufflePanels( void )
{
	uint32 cnt = fPanels.CountItems() ;

	BView * v1 ;
	BView * v2 = reinterpret_cast<BView *>(fPanels.ItemAt(0)) ;
	for( uint32 i=1; i<cnt; i++)
	{
		v1 = v2 ;
		v2 = reinterpret_cast<BView *>( fPanels.ItemAt(i) ) ;
		if(	v1->IsHidden() )
		{
			v2->MoveTo( 0, v1->Frame().top ) ;
		}
		else
		{
			v2->MoveTo( 0, v1->Frame().bottom + 1 ) ;
		}
	}
	if(	v2->IsHidden() )
	{
		fpOptionsPanel->MoveTo( 0, v2->Frame().top ) ;
	}
	else
	{
		fpOptionsPanel->MoveTo( 0, v2->Frame().bottom + 1 ) ;
	}
	ResizeHeight() ;
}

void TraxWindow::ResizeHeight( void )
{
	float height = fpOptionsPanel->Frame().bottom ;
	
	float minH, minW, maxH, maxW ;
	GetSizeLimits( &minW, &maxW, &minH, &maxH ) ;
	SetSizeLimits( minW, maxW, height, height ) ;

 	ResizeTo( Frame().Width(), height ) ;
}

bool TraxWindow::QuitRequested( void )
{
	// Response to Close Message, not to Application Quiting
	if( CurrentMessage() )
	{		
		BMessenger msgr( be_app ) ;
		BMessage reply ;
		if( msgr.IsValid() )
		{
			BMessage msg( Messages::WindowClosed ) ;
			msg.AddPointer( "Window", this ) ;
			msgr.SendMessage( &msg , &reply ) ;
		}
	}
	return inherited :: QuitRequested( ) ;
}

void TraxWindow::MessageReceived( BMessage * msg )
{
	switch( msg->what )
	{
		case Messages::AddPanel:
		{
			uint32 i = fPanels.CountItems() ;
			BView * v = reinterpret_cast<BView *>( fPanels.ItemAt(i-1) ) ;

			BMessage send( Messages::SetPosition ) ;
			int32 pos = JoinPanel::ePosMiddle ;
			send.AddInt32( "position", pos ) ;
			
			PostMessage( &send, v ) ;

			float top = ( v->IsHidden() ?
			              v->Frame().top :
			              v->Frame().bottom +1 ) ;
			PredicatePanel * newPred = new PredicatePanel( top ) ;
			newPred->SetViewColor( v->ViewColor() ) ;
			AddChild( newPred ) ;
			fPanels.AddItem( newPred ) ;
						
			JoinPanel * newJoin = new JoinPanel( newPred->Frame().bottom+1 ) ;
			newJoin->SetViewColor( v->ViewColor() ) ;
			newJoin->SetPosition( JoinPanel::ePosBottom ) ;
			AddChild( newJoin ) ;
			fPanels.AddItem( newJoin ) ;

			top = ( newJoin->IsHidden() ?
			        newJoin->Frame().top :
			        newJoin->Frame().bottom +1 ) ;

			fpOptionsPanel->MoveTo( 0, top ) ;

			ReshufflePanels() ;			
			break ;
		}

		case Messages::RemovePanel:
		{
			uint32 i = fPanels.CountItems() ;
			if( i <= 3 )
				break ; 

			BView * v = reinterpret_cast<BView *>( fPanels.ItemAt(i-1) ) ;
			RemoveChild( v ) ;
			delete v ;

			v = reinterpret_cast<BView *>( fPanels.ItemAt(i-2) ) ;
			RemoveChild( v ) ;
			delete v ;

			fPanels.RemoveItems( i-2, 2 ) ;
			
			v = reinterpret_cast<BView *>( fPanels.ItemAt(i-3) ) ;

			BMessage send( Messages::SetPosition ) ;
			int32 pos = JoinPanel::ePosBottom ;
			send.AddInt32( "position", pos ) ;
			
			PostMessage( &send, v ) ;

			float top = ( v->IsHidden() ?
			              v->Frame().top :
			              v->Frame().bottom +1 ) ;
			fpOptionsPanel->MoveTo( 0, top ) ;

			ReshufflePanels() ;			
			break ;
		}

		case Messages::StartFind:
		{
			if( !FindLibThread::InitFind( ) )
			{
				beep() ;
				break ;
			}
			
			PostMessage( msg, fpOptionsPanel ) ;
			int32 i , c ; 
			for( i=0 , c = fPanels.CountItems() ; i<c ; i++ )
			{
				BView * v = reinterpret_cast<BView *>( fPanels.ItemAt(i) ) ;
				PostMessage( msg, v ) ;
			}
			PostMessage( msg, fpFolderPanel ) ;
			break ;
		}

		case Messages::LibInUse:
		case Messages::LibNotInUse:
		{
			Looper()->PostMessage( msg, fpOptionsPanel ) ;
			break ;
		}
		
		case Messages::Settings:
		{
			if( spSettingsWindow )
			{
				spSettingsWindow->Lock() ;
				if( spSettingsWindow->IsHidden() )
					spSettingsWindow->Show();
				spSettingsWindow->Unlock() ;
				spSettingsWindow->Activate( true ) ;
			}
			else
			{
				spSettingsWindow = new SettingsWindow ;
				spSettingsWindow->Show();
			}
			break ;
		}
		case Messages::SettingsChanged :
		{
			for( uint32 i=0 , c = fPanels.CountItems() ;
			     i<c ; i++ )
			{
				BView * v = reinterpret_cast<BView *>( fPanels.ItemAt(i) ) ;
				PostMessage( msg, v ) ;
			}
			PostMessage( MSG_Reshuffle ) ;
			break ;
		}
		case MSG_Reshuffle:
		{
			ReshufflePanels() ;
			break ;
		}
		case Messages::NewWindow:
		{
			be_app_messenger.SendMessage( msg ) ;
			break ;
		} 
		default:
		{
			inherited::MessageReceived( msg ) ;
			break ;
		}
	}
}

void TraxWindow:: Show( void )
{
	inherited::Show() ;
	PostMessage( Messages::SettingsChanged ) ;
}