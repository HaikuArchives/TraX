#ifndef EDITBOX_H
#define EDITBOX_H

/*
 *       Class: EditBox
 * Description: A simple TextControl view
 *   Copyright: Tim Vernum 1999, All rights reserved
 *     Licence: LGPL
 */

#include <TextView.h>

#ifndef   SCRIPTER_H
#include "Scripter.h"
#endif

class EditBox : public BView
{
	typedef BView inherited ;
	public:
		EditBox(	BRect frame , const char * name ,
					uint32 resizeMask = B_FOLLOW_LEFT | B_FOLLOW_TOP ,
					uint32 flags = B_WILL_DRAW ) ;

		EditBox(	BRect frame , const char * name , BTextView * ,
					uint32 resizeMask = B_FOLLOW_LEFT | B_FOLLOW_TOP ,
					uint32 flags = B_WILL_DRAW ) ;
		
		void	SetString( const char * ) ;
		void	SetFloat( float f ) ;
		void	SetFloat( float f , const char * fmt ) ;
		void	SetInt( int32 ) ;
		void	SetIntOct( int32 ) ;
		void	SetIntHex( int32 ) ;
		void	SetIntHEX( int32 ) ;
		void	SetDate( time_t ) ;

		const char *	GetString( void ) const ;
		float			GetFloat( void ) const ;
		int32			GetInt( void ) const ;
		int32			GetIntC( void ) const ;
		int32			GetIntInBase( int radix ) const ;
		time_t			GetDate( void ) const ;
		
		enum EditFilter
		{
			eString ,
			eFloat ,
			eInteger ,
			eIntOct ,
			eIntHex ,
			eIntC ,
		} ;
		void	SetFilter( EditFilter ) ;

		void 	Draw( BRect r ) ;
		void 	MakeFocus( bool ) ;
		void 	WindowActivated( bool ) ;
		void 	FrameResized( float , float ) ;
		void 	MessageReceived( BMessage * ) ;
		void	AttachedToWindow( void ) ;


		status_t	GetSupportedSuites( BMessage * ) ;
		BHandler *	ResolveSpecifier( BMessage * , int32 , BMessage *,
		          	                  int32 , const char * ) ;

	protected:
		void 	ResetTextViewRect( float w, float h ) ;
		void	DrawFrame( bool highlight ) ;
		void	Create( BTextView * ) ;
		
		BTextView	* fpTextView ;

	private:
		bool		HandleProperty( const char *, BMessage * ) ;

		static	property_info	sPropList[] ;
				Scripter		fScripter ;
} ;

#endif
