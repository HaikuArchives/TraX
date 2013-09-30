#ifndef SUPPORT_H
#define SUPPORT_H

/*******************************************************
 * (c) 1999, Tim Vernum <zodsoft@kagi.com>
 * All rights reserved
 *******************************************************
 * File   : Support.h
 * Author : Tim Vernum
 * Project: Trax Application
 * Purpose: General Support functions
 * Licence: AUL
 *******************************************************/

//=================
// Local Headers
#include "Benaphore.h"

//=================
// BeOS Headers
#include <SupportDefs.h>

//=================
// ANSI / POSIX Headers
#include <ctype.h>

namespace MathUtils
{
	// Equivalent to strcmp - useful in sorting
	int		intcmp( int i1, int i2 ) ;

	double min( double a, double b ) ;
	double max( double a, double b ) ;

	long min( long a, long b ) ;
	long max( long a, long b ) ;
}

namespace StringUtils
{
	// Thread Safe strerror
	char * ErrorString( status_t, char * ) ;
	// Thread Safe strerror
	char * ErrorString( status_t, char *, size_t ) ;
	// Verbose Thread Safe strerror
	char * VerboseErrorString( status_t, char * ) ;

	// Pointer to first '\0' in arg
	char * StrEnd( char * ) ;
	
	// make all chars lowercase
	char * LowerCase( char * ) ;
	// make initial char uppercase
	char * Capitalise( char * ) ;
	
	extern Benaphore fStrErrorLock ;
}

namespace ObjectUtils
{
	// Duplicates an object from another pointer
	template <class T>
	inline T* Dup( T* )
	{
		return new T( *t ) ;
	}  

	template <class T>
	inline void Swap( T& , T& )
	{
		T c = a ;
		a = b ;
		b = c ;
	}
	
	
}

//=================
// Math Utils

inline int MathUtils:: intcmp( int i1, int i2 )
{
	if( i1 < i2 ) return -1 ;
	return i1 != i2 ;
}

inline double MathUtils:: min( double a, double b )
{
	if( a < b ) return a ;
	return b ;
}
inline double MathUtils:: max( double a, double b )
{
	if( a > b ) return a ;
	return b ;
}

inline long MathUtils:: min( long a, long b )
{
	if( a < b ) return a ;
	return b ;
}
inline long MathUtils:: max( long a, long b )
{
	if( a > b ) return a ;
	return b ;
}

//=================
//String Utils
inline char * StringUtils:: StrEnd( char * s )
{
	while( *s )
		s++ ;
	return s ;
}

inline char * StringUtils:: Capitalise( char * s )
{
	*s = toupper(*s) ;
	return s ;
}


#endif
