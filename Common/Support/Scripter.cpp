#ifndef   SCRIPTER_H
#include "Scripter.h"
#endif

/*
 *       Class: Scripter
 * Description: Class to managing BeOS application scripting.
 *   Copyright: Tim Vernum 1999, All rights reserved
 *     Licence: LGPL
 */

#ifndef   SUPPORT_H
#include "Support.h"
#endif

#include <stdio.h>
#include <string.h>

uint32 Scripter::sDefaultOptions ;
uint32 Scripter::DefaultOptions( void )
{
	return sDefaultOptions ;
}

uint32 Scripter::SetDefaultOptions( uint32 opt )
{
	uint32 old = sDefaultOptions ;
	sDefaultOptions = opt ;
	return old ;
}

Scripter :: Scripter( const char * s, property_info* pi )
:	fSuite( s )
,	fPropInfoArray( pi )
,	fPropInfo( pi )
,	fOptions( sDefaultOptions )
{
	if( fOptions & PrintDebug )
	{
		fprintf( stderr, "Created Scripter (%p) as\n\
Suite: %s\n\
property_info *: %p\n\
Default Options: %lu\n", this, fSuite, pi, fOptions ) ;
	}
}

Scripter :: Scripter( const char * s, property_info* pi, uint32 opt )
:	fSuite( s )
,	fPropInfoArray( pi ) 
,	fPropInfo( pi )
,	fOptions( opt )
{
	if( fOptions & PrintDebug )
	{
		fprintf( stderr, "Created Scripter (%p) as\n\
Suite: %s\n\
property_info *: %p\n\
Set Options: %lu\n", this, fSuite, pi, fOptions ) ;
	}
}

Scripter :: ~Scripter( void )
{
	if( fOptions & PrintDebug )
		fprintf( stderr, "Delete Scripter (%p)\n", this ) ;
}

void Scripter :: SetOwner( BHandler * h )
{
	fpTarget = h ;
}

void Scripter :: SetAlias( const char * p, const char * a)
{
	if( fOptions & PrintDebug )
		fprintf( stderr, "Add Alias %s for %s in %p\n", a, p, this ) ;

	if( fAlias.HasString( a ) )
		fAlias.ReplaceString( a , p ) ;
	else
		fAlias.AddString( a, p ) ;
}

void Scripter :: SetHandler( const char * p, PropertyHandler_f h)
{
	if( fHandlers.HasData( p, B_RAW_TYPE ) )
		fHandlers.ReplaceData( p , B_RAW_TYPE, &h, sizeof(h) ) ;
	else
		fHandlers.AddData( p, B_RAW_TYPE, &h, sizeof(h) ) ;
}

void Scripter :: SetResolver( const char * p, SpecifierResolver_f r)
{
	if( fResolvers.HasData( p, B_RAW_TYPE ) )
		fResolvers.ReplaceData( p , B_RAW_TYPE, &r, sizeof(r) ) ;
	else
		fResolvers.AddData( p, B_RAW_TYPE, &r, sizeof(r) ) ;
}


int32 Scripter :: FindMatch( BMessage * m, int32 i,
							 BMessage * s, int32 w,
							 const char * prop )
{
	if( fOptions & PrintDebug )
		fprintf( stderr, "Finding match for %s (%p)\n", prop, this ) ;
	
	// Due to swapping (alias/lowercase) we need to pointer to fiddle with
	const char * p2 ;
	
	// buffer for lowercase if we need it
  	char * buff = NULL ;

	// Convert input string to caps/lowercase if that option is set
	if( fOptions & Capitalise )
	{
		buff = strdup(prop) ;
		StringUtils::LowerCase( buff ) ;
		StringUtils::Capitalise( buff ) ;
		p2 = buff ;
	}
	if( fOptions & LowerCase )
	{
		buff = strdup(prop) ;
		StringUtils::LowerCase( buff ) ;
		p2 = buff ;
	}
	else
		p2 = prop ;

	// check for aliases :: If LowerCase is set, then aliases must be lowercase too
	if( fAlias.HasString( p2 ) )
		fAlias.FindString( p2, &prop ) ;
	else
		prop = p2 ;

	// Look for a match
	int32 idx = fPropInfo.FindMatch( m, i, s, w, prop) ;

	if( fOptions & PrintDebug )
		fprintf( stderr, "Match for %s is %ld (%p)\n", prop, idx, this ) ;

	// clean up
	delete[] buff ;
	
	// return
	return idx ;
}

bool Scripter::FindResolver( const char * p, SpecifierResolver_f * r )
{
	const void * v ;
	ssize_t sz ;
	if( fResolvers.FindData( p, B_RAW_TYPE, &v, &sz ) != B_OK )
	{
		if( !(fOptions & UseWildcard ) )
			return false ;
			
		if( fResolvers.FindData( "*", B_RAW_TYPE, &v, &sz ) != B_OK )
			return false ;
	}
	
	SpecifierResolver_f * psr = (SpecifierResolver_f *)(v) ;
	(*r) = (*psr) ;
	return true ;
}

bool Scripter::FindHandler( const char * p, PropertyHandler_f * h )
{
	const void * v ;
	ssize_t sz ;
	if( fHandlers.FindData( p, B_RAW_TYPE, &v, &sz ) != B_OK )
	{
		if( !(fOptions & UseWildcard ) )
			return false ;
			
		if( fHandlers.FindData( "*", B_RAW_TYPE, &v, &sz ) != B_OK )
			return false ;
	}

	PropertyHandler_f * pph = (PropertyHandler_f *)(v) ;
		
	(*h) = (*pph) ;
	return true ;
}

status_t Scripter :: GetSupportedSuites( BMessage * msg )
{
	msg->AddString("suites", fSuite ); 
	msg->AddFlat("messages", &fPropInfo); 
	return B_OK ;
}

BHandler * Scripter :: ResolveSpecifier( BMessage * m, int32 i,
		           			             BMessage * s, int32 w, const char * p )
{
	int32 idx = FindMatch( m,i,s,w,p ) ;
	if( idx < 0 )
		return NULL ;

	const char * prop = fPropInfoArray[idx].name ;
	
	SpecifierResolver_f r ;
	if( FindResolver( prop, &r ) )
		return (fpTarget->*r)(m, i, prop, idx) ;
	else
		return fpTarget ;
}

bool Scripter :: HandleMessage( BMessage * msg )
{
	if( msg->what != B_SET_PROPERTY
	 && msg->what != B_GET_PROPERTY
	 && msg->what != B_CREATE_PROPERTY
	 && msg->what != B_DELETE_PROPERTY
	 && msg->what != B_COUNT_PROPERTIES
	 && msg->what != B_EXECUTE_PROPERTY	)
		return false ;
	
	if( fOptions & PrintDebug )
	{
		fprintf( stderr, "Handle Message (%p)\n", this ) ;
		msg->PrintToStream() ;
	}

	int32 idx ;
	BMessage spec ;
	msg->GetCurrentSpecifier( &idx, &spec ) ;

	const char * spec_prop ;
	spec.FindString( "property", &spec_prop ) ;
	
	idx = FindMatch( msg , idx , &spec , spec.what , spec_prop ) ;
	if( idx < 0 )
		return false ;

	const char * prop = fPropInfoArray[idx].name ;

	PropertyHandler_f h ;
	if( FindHandler( prop, &h ) )
		return (fpTarget->*h)(prop, msg) ;
	else
		return false ;
}
