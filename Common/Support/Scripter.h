#ifndef SCRIPTER_H
#define SCRIPTER_H

/*
 *       Class: Scripter
 * Description: Class to managing BeOS application scripting.
 *   Copyright: Tim Vernum 1999, All rights reserved
 *     Licence: LGPL
 */

#include <Message.h>
#include <Handler.h>
#include <PropertyInfo.h>

class Scripter
{
	public:
	
		enum {
			PrintDebug  = 0x1 ,
			UseWildcard = 0x2 ,
			LowerCase   = 0x4 ,
			Capitalise  = 0x8 ,
		} ;
	
				 Scripter( const char * , property_info * ) ;
				 Scripter( const char * , property_info *, uint32 options ) ;
		virtual	~Scripter( void ) ;

		static	uint32 DefaultOptions( void ) ;
		static	uint32 SetDefaultOptions( uint32 ) ;
		
		void		SetOwner( BHandler * ) ;
		void		SetAlias( const char *, const char * ) ;

		typedef BHandler * (BHandler::*SpecifierResolver_f)(
		                                BMessage * , int32 ,
										const char *, int32 ) ;
		void		SetResolver( const char *, SpecifierResolver_f ) ;

		typedef bool (BHandler::*PropertyHandler_f)( const char *, BMessage * ) ;
		void		SetHandler( const char *, PropertyHandler_f ) ;

		virtual	status_t	GetSupportedSuites( BMessage * ) ;
		virtual BHandler *	ResolveSpecifier( BMessage * , int32 ,
				                              BMessage * , int32 , const char * ) ;
		virtual	bool		HandleMessage( BMessage * ) ;
		
	
		virtual	int32		FindMatch( BMessage *, int32, BMessage *, int32, const char * ) ;
		virtual bool		FindResolver( const char *, SpecifierResolver_f * ) ;
		virtual bool		FindHandler( const char *, PropertyHandler_f * ) ;

	private:
		BHandler *				fpTarget ;
		const char *			fSuite ;
		property_info *			fPropInfoArray ;
		BPropertyInfo	 		fPropInfo ;

		BMessage				fAlias ;
		BMessage				fHandlers ;
		BMessage				fResolvers ;
		
		uint32					fOptions ;
		static uint32			sDefaultOptions ;
} ;

#endif
