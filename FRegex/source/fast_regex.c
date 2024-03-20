#include "defs.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int (*fast_regex_subsys_error)( const char * ,const char * ) ;
int fprintf_error( const char * sys, const char * error )
{
	fprintf( stderr, "(%s)%s", sys , error ) ;
	return 1 ;
}

#define HAS_REGEX( _o ) (!( (_o) & FRE_NO_REGEX ))
#define HAS_DFA( _o )   (!( (_o) & FRE_NO_DFA   ))
#define HAS_KWSET( _o ) (!( (_o) & FRE_NO_KWSET ))

/*
 * We require a safe tolower.
 * ie one that gives (eg) 'a' -> 'a'
 *                        'A' -> 'a'
 *                        '?' -> '?'
 * The C standard requires this. If your system is broken, then you'll have
 *   to define BROKEN_TOLOWER and use the version below
 */
#ifdef BROKEN_TOLOWER
int sensible_tolower( int c )
{
	/* Some systems give incorrect results of isupper(c) when c isn't an alpha
	 * since tolower is already broken, just assume isupper is too */ 
	if( isalpha(c) && isupper(c) )
		c = tolower(c) ;
	return c ;
}
#define TOLOWER sensible_tolower
#else
#define TOLOWER tolower
#endif

static char ichars[256] = { 0 };
static void build_ichars( void )
{
	int c ;
	for( c=0; c<256; c++)
		ichars[c] = TOLOWER(c) ;
}

int build_kws_from_dfa( struct _fregex * fre )
{
	struct dfamust * dm ;
	char * errstr ;
	int errval ;

//	struct _fregex * fre ;
//	fre = (struct _fregex *)( fre_t->data ) ;
	
	if (!fre )
	{
		return 0 ;
	}
	if(!fre->dfa.musts)
	{
		fre->kwset = 0 ;
		return 0 ;
	}

	/* Foreach exact string */
	for (dm = fre->dfa.musts; dm; dm = dm->next)
	{
		if (!dm->exact)
			continue;
		errstr = kwsincr( fre->kwset, dm->must, strlen(dm->must) );
		if( errstr )
		{
			errval = (*fast_regex_subsys_error)( "kwset", errstr ) ;
			if( errval )
				return errval ;
		}
		else
			fre->num_exact_kws ++ ;
	}

	/* Foreach inexact string -- will require further parsing */
	for (dm = fre->dfa.musts; dm; dm = dm->next)
	{
		if ( dm->exact )
			continue ;
		errstr = kwsincr( fre->kwset, dm->must, strlen(dm->must) );
		if( errstr )
		{
			errval = (*fast_regex_subsys_error)( "kwset", errstr ) ;
			if( errval )
				return errval ;
		}
	}
	errstr = kwsprep( fre->kwset );
	if( errstr )
	{
		errval = (*fast_regex_subsys_error)( "kwset", errstr ) ;
		if( errval )
			return errval ;
	}
	return 0 ;
}

int allocate_fast_regex ( fast_regex_t * fre_t )
{
	struct _fregex * fre = (struct _fregex*)( malloc( sizeof(struct _fregex) ) ) ;
	fre_t->data = fre ;

/*
		fprintf( stderr, "allocate_fast_regex( %p : %p, %p, %p, %hu, %lu )\n",
		fre_t , &fre->regex, &fre->dfa, fre->kwset,
		fre->num_exact_kws, fre->options ) ;
*/
	
	if( !(ichars['A']) )
		build_ichars() ;

/*
		fprintf( stderr, "(bi) - allocate_fast_regex( %p[%p] )\n",
		fre_t , fre->kwset ) ;
*/

	if( HAS_KWSET(fre_t->options) )
		fre->kwset = kwsalloc( (fre_t->options & FRE_CASE_INSENSITIVE) ? ichars : NULL ) ;
	else
		fre->kwset = NULL ;
	fre->num_exact_kws = 0 ;

/*
		fprintf( stderr, "(kw) - allocate_fast_regex( %p[%p] )\n",
		fre_t , fre->kwset ) ;
*/

	if( HAS_DFA(fre_t->options) )
		dfainit( &(fre->dfa) );

/*
		fprintf( stderr, "(df) - allocate_fast_regex( %p[%p] )\n",
		fre_t , fre->kwset ) ;
*/

	if( HAS_REGEX(fre_t->options) )
	{
		fre->regex.allocated = 0 ;
		fre->regex.buffer = NULL ;
		fre->regex.fastmap = NULL ;

		if ( fre_t->options & FRE_CASE_INSENSITIVE )
			fre->regex.translate = ichars ;
		else
			fre->regex.translate = NULL ;
	}

/*
		fprintf( stderr, "(re) - allocate_fast_regex( %p[%p] )\n",
		fre_t , fre->kwset ) ;
*/
	return 0 ;
}

int compile_fast_regex( fast_regex_t * fre_t, const char * sre, size_t len )
{
	struct _fregex * fre = (struct _fregex *)( fre_t->data ) ;

	int errval ;
	char * errstr ;
	int sublen ;
	char * substr ;
	
	errval = 0 ;

		/*	fprintf( stderr, "compile_fast_regex( %p[%p] , %s )\n", fre_t, fre->kwset, sre ) ; */

	if( fast_regex_subsys_error == NULL )
		fast_regex_subsys_error = fprintf_error ;
	
	if( fre_t->options & FRE_STRING_SEARCH )
	{
		if( fre_t->options & FRE_NO_KWSET )
		{
			return -1 ;
		}
			
		/*
		 * straight string match
		 * 'sre' represents a series of newline separated strings to search for
		 */
		while( sre )
		{
			substr = strchr( sre, '\n' ) ;
			if( substr == NULL )
				sublen = len ;
			else
				sublen = (substr - sre) ;

			errstr = kwsincr( fre->kwset, sre, sublen );
			if( errstr )
			{
				errval = (*fast_regex_subsys_error)( "kwset" , errstr ) ;
				if( errval )
					return errval ;
			}
			else
				fre->num_exact_kws ++ ;
		
			len -= (sublen+1) ;
			sre = (substr) ? (substr + 1) : NULL ;
		}
		errstr = kwsprep( fre->kwset );
		if( errstr )
		{
			errval = (*fast_regex_subsys_error)( "kwset", errstr ) ;
			if( errval )
				return errval ;
		}
		return 0 ;
	}
	
	if( HAS_DFA(fre_t->options) || HAS_KWSET(fre_t->options) )
	{
		dfasyntax( RE_SYNTAX_GREP | RE_HAT_LISTS_NOT_NEWLINE, ( fre_t->options & FRE_CASE_INSENSITIVE ) ) ;
		dfacomp( sre, len, &(fre->dfa), 1 ) ;

		if( HAS_KWSET(fre_t->options) )
		{
			errval = build_kws_from_dfa( fre ) ;
			if( fre_t->options & FRE_NO_DFA )
			{  /* We used the DFA only to get the keywords */
				dfafree( &(fre->dfa) );
			}
		}
		else
		{
			fre->kwset = 0 ;
		}
	}

	if( errval )
		return errval ;

	if( HAS_REGEX(fre_t->options) )
	{
		re_set_syntax(RE_SYNTAX_GREP | RE_HAT_LISTS_NOT_NEWLINE);
		errstr = re_compile_pattern( sre, len, &(fre->regex) ) ;
		if( errstr )
		{
			errval = (*fast_regex_subsys_error)( "re", errstr ) ;
			if( errval )
				return errval ;
		}
	}
	
	return errval ;
}		

int evaluate_fast_regex( struct fast_regex * fre_t, char * str, size_t len )
{
	char * sub ;
	struct _fregex * fre = (struct _fregex *)( fre_t->data ) ;

	if( fre->kwset )
	{
		struct kwsmatch kwsm ;
		sub = kwsexec( fre->kwset, (char *)str, len, &kwsm) ;
		if( sub == NULL )
			return 0 ;
		if( kwsm.index < fre->num_exact_kws )
		{
			return 1 ;
		}
	}

	if( HAS_DFA(fre_t->options) )
	{
		int backref = 0 ;
		sub = dfaexec( &(fre->dfa), str, (str+len), 0, NULL, &backref) ;
		if( sub == NULL )
			return 0 ;
		if ( !backref || (fre_t->options & FRE_NO_REGEX) )
			return 1 ;
	}

	return re_match( &fre->regex , str, len, 0, NULL ) > 0 ;
}

void dispose_fast_regex( struct fast_regex * fre_t )
{
	struct _fregex * fre = (struct _fregex *)( fre_t->data ) ;

	if( HAS_REGEX( fre_t->options ) )
	{
		fre->regex.translate = NULL ;
		regfree( &fre->regex ) ;
	}
	if( HAS_DFA( fre_t->options ) )
		dfafree( &fre->dfa ) ;
	if( fre->kwset )
		kwsfree( fre->kwset ) ;
}

void dfaerror( char * msg )
{
	(*fast_regex_subsys_error)( "dfa", msg ) ;
	exit(0) ;
}
