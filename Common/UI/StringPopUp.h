#ifndef STRINGPOPUP_H
#define STRINGPOPUP_H

/*******************************************************
 * (c) 1999, Tim Vernum <zodsoft@kagi.com>
 * All rights reserved
 *******************************************************
 * File   : StringPopUp.h
 * Author : Tim Vernum
 * Project: Trax Application
 * Purpose: Class to provide a popup menu of simple strings
 * Licence: LGPL
 *******************************************************/

#ifndef STRINGPOPUP_FWD
#define STRINGPOPUP_FWD
#endif

#include <MenuField.h>

#ifndef   SCRIPTER_H
#include "Scripter.h"
#endif 

class StringPopUp : public BMenuField
{
	typedef BMenuField inherited ;

	public:
			 StringPopUp(	BRect, const char * name ,
							const char * label = NULL ,
							uint32 = B_FOLLOW_LEFT | B_FOLLOW_TOP ,
							uint32 = B_WILL_DRAW | B_NAVIGABLE ) ;
			~StringPopUp( void ) ;
		
		void	AttachedToWindow( void ) ;

		const char *	GetSelection( void ) const ;
		int32			GetIndex( void ) const ;

		bool			SetSelection( const char * ) ;
		bool			SetIndex( int32 ) ;

		const char * GetString( int32 ) const ;
		
		void	AddStrings( int32, const char * const * ) ;
		void	AddString( const char * ) ;
		void	AddSeparator( void ) ;

		void	Empty( void ) ;

		void	SetTarget( BHandler * ) ;
		void	SetMessage( BMessage * ) ;

		void	SetPopUpWidth( float ) ;
		void	GetPreferredSize(float * w, float * h ) ;

		void 	MessageReceived( BMessage * ) ;

		void 	SetViewColor( rgb_color ) ;

		status_t	GetSupportedSuites( BMessage * ) ;
		BHandler *	ResolveSpecifier( BMessage * , int32 , BMessage *,
		          	                  int32 , const char * ) ;

	private:

		void		InvokeItem( BMenuItem * ) ;
		bool		HandleProperty( const char *, BMessage * ) ;

		BHandler *	fpTarget ;
		BMessage *	fpMessage ;
		float		fPopUpWidth ;

		static	property_info	sPropList[] ;
				Scripter		fScripter ;
} ;

#endif
