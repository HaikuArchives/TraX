#ifndef OPTIONSPANEL_H
#define OPTIONSPANEL_H

/*
 *       Class: OptionsPanel
 *   Copyright: Tim Vernum 1999, All rights reserved
 *     Licence: GPL
 */

#ifndef   EDITBOX_FWD
#include "EditBox.fwd"
#endif

#ifndef   PANEL_H
#include "Panel.h"
#endif

class BCheckBox ;
class BarberPole ;
class ColouredView ;

class OptionsPanel : public Panel
{
	typedef Panel inherited ;
	public:
		 OptionsPanel( float top ) ;
		~OptionsPanel( void ) ;

		void	AttachedToWindow( void ) ;
		void	Draw( BRect ) ;
		void	MessageReceived( BMessage * ) ;
		
	private:
		void	StartFind( void ) ;
		
		BButton     	*	fpFindButton ;
		EditBox	    	*	fpMaxDepthEdit ;
		BCheckBox   	*	fpMaxDepthCheck ;
		BarberPole  	* 	fpBarberPole ;
		ColouredView	*	fpPoleFrame ;
} ;

#endif
