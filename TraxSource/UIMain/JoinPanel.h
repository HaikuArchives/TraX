#ifndef JOINPANEL_H
#define JOINPANEL_H

/*
 *       Class: JoinPanel
 *   Copyright: Tim Vernum 1999, All rights reserved
 *     Licence: GPL
 */

#ifndef   STRINGPOPUP_FWD
#include "StringPopUp.fwd"
#endif

#ifndef   PANEL_H
#include "Panel.h"
#endif

class JoinPanel : public Panel
{
	typedef Panel inherited ;
	public:

		JoinPanel( float top ) ;

		enum EPosition { ePosTop = 1, ePosMiddle , ePosBottom } ;
		void	SetPosition( EPosition ) ;

		void	MessageReceived( BMessage * ) ;
		void	AttachedToWindow( void ) ;
		void	Draw( BRect ) ;
		
	private:
		void	ProcessFindAction( void ) ;
		void	ImplementSettings( void ) ;

		void	ShowHidePopUps() ;
	
		StringPopUp	*	fpNot ;
		StringPopUp	*	fpCloseBracket ;
		StringPopUp	*	fpConjunction ;
		StringPopUp	*	fpOpenBracket ;
		EPosition  		fPosition ;
} ;

#endif
