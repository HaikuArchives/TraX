#ifndef PREDICATEPANEL_H
#define PREDICATEPANEL_H

/*
 *       Class: PredicatePanel
 *   Copyright: Tim Vernum 1999, All rights reserved
 *     Licence: GPL
 */

#ifndef   STRINGPOPUP_FWD
#include "StringPopUp.fwd"
#endif

#ifndef   EDITBOX_FWD
#include "EditBox.fwd"
#endif

#ifndef   PANEL_H
#include "Panel.h"
#endif

class PredicatePanel : public Panel
{
	typedef Panel inherited ;
	public:

		 PredicatePanel( float top ) ;
		~PredicatePanel( void ) ;

		void	AttachedToWindow( void ) ;
		void	Draw( BRect ) ;
		
		void	MessageReceived( BMessage * ) ;
		
	private:
		void	ProcessFindAction( void ) ;
		void	SetupItems() ;
	
		StringPopUp	*	fpAction ;
		StringPopUp	*	fpTest ;
		EditBox		*	fpEdit ;
		StringPopUp	*	fpOptions ;
		char *			fStr ;
} ;

#endif
