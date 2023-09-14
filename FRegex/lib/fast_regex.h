#ifndef FAST_REGEX_H
#define FAST_REGEX_H

/*
 * fast_regex performs fast regexp matches by using
 *  a keyword set there possible, and then a DFA,
 *  only resorting to regex where there is a high
 *  probability of a match
 *
 * Idea taken from GNU grep
 *
 * (C) Copyright 1999, Tim Vernum
 * This file is licenced under the LGPL.
 * The fast regex package is under the GPL.
 */

enum EFastRegexOptions
{
	FRE_NO_KWSET            = 0x001 ,
	FRE_NO_DFA              = 0x002 ,
	FRE_NO_REGEX            = 0x004 ,
	
	FRE_CASE_INSENSITIVE    = 0x008 ,
	
	FRE_STRING_SEARCH       = (FRE_NO_DFA | FRE_NO_REGEX)
} ;

struct _fregex ;
struct fast_regex
{
	struct _fregex * data ;
	unsigned long options ;
} ;

typedef struct fast_regex fast_regex_t ;
 
//int (*fast_regex_subsys_error)( const char * sys, const char * msg ) ;

int  allocate_fast_regex ( fast_regex_t * ) ;
int  compile_fast_regex  ( fast_regex_t *, const char *, size_t ) ;
int  evaluate_fast_regex ( fast_regex_t *, char *, size_t ) ;
void dispose_fast_regex  ( fast_regex_t * ) ;

#endif
