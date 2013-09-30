#ifndef PREDICATES_H
#define PREDICATES_H

#include <sys/stat.h>
#include "findlib.h"

/*
 *              Predicates.h
 *   Copyright: Tim Vernum 1999, All rights reserved
 *     Licence: GPL
 */

const char * kStrCmpTests[] = 
	{
		"contains" ,
		"equals" ,
		"begins with" ,
		"ends with" ,
		"regex"
	} ;
enum EStringCompare
	{
		eStrContains = 0 ,
		eStrEquals ,
		eStrBegins ,
		eStrEnds ,
		eStrRegex
	} ;
	

const char * kSizeCmpTests[] = 
	{
		"greater than" ,
		"less than" ,
		"equals"
	} ;
const enum comparison_type kSizeCmpValues[] =
	{
		COMP_GT,
	    COMP_LT,
	    COMP_EQ
	} ;

const char * kDateCmpTests[] = 
	{
		"after" ,
		"before" ,
		"on"
	} ;
const enum comparison_type kDateCmpValues[] =
	{
		COMP_GT,
	    COMP_LT,
	    COMP_EQ
	} ;


const char * kSimpleCmpTests[] = 
	{
		"is" ,
		"isn't"
	} ;
const bool kSimpleCmpValues[] =
	{
		true ,
		false
	} ;
	
const char * kSubStrCmpTests[] = 
	{
		"string" ,
		"regex"
	} ;
enum ESubStringCompare
	{
		eSubString = 0 ,
		eSubRegex
	} ;


const char * kStrCmpOptions[] = 
	{
		"case insensitive" ,
		"case sensitive" ,
	} ;
const bool kStrCmpValues[] = 
	{
		false ,	// !sensitive
		true , 	//  sensitive
	} ;

const char * kSizeCmpOptions[] = 
	{
		"bytes" ,
		"kb" ,
		"Mb" ,
		"Gb"
	} ;	
const uint32 kSizeOptValues[] =
	{
		1,
		1024,
		1024 * 1024 ,
		1024 * 1024 * 1024
	} ;

const char * kKindCmpOptions[] = 
	{
		"directory" ,
		"symbolic link" ,
		"plain file"
	} ;	
const uint32 kKindCmpValues[] =
	{
		S_IFDIR ,
		S_IFLNK ,
		S_IFREG
	} ;

const char * kActionStrings[] = 
	{
		"Name" ,
		"Extension" ,
		"Size" ,
		"Modified" ,
		"Created" ,
		"Kind" ,
		"Contains" ,
	} ;
enum EAction
	{
		eName = 0 ,
		eExtension ,
		eSize ,
		eModified ,
		eCreated ,
		eKind ,
		eContains
	} ;
	
const char * const * kTests[] =
	{
		kStrCmpTests ,
		kSimpleCmpTests ,
		kSizeCmpTests ,
		kDateCmpTests ,
		kSimpleCmpTests ,
		kSubStrCmpTests
	};
size_t kNumberTests[] =
	{
		sizeof(kStrCmpTests)/sizeof(kStrCmpTests[0]) ,
		sizeof(kSimpleCmpTests)/sizeof(kSimpleCmpTests[0]) ,
		sizeof(kSizeCmpTests)/sizeof(kSizeCmpTests[0]) ,
		sizeof(kDateCmpTests)/sizeof(kDateCmpTests[0]) ,
		sizeof(kSimpleCmpTests)/sizeof(kSimpleCmpTests[0]) ,
		sizeof(kSubStrCmpTests)/sizeof(kSubStrCmpTests[0])
	};

const char * const * kOptions[] =
	{
		kStrCmpOptions ,
		kStrCmpOptions ,
		kSizeCmpOptions ,
		NULL ,
		kKindCmpOptions ,
		kStrCmpOptions
	};
size_t kNumberOptions[] =
	{
		sizeof(kStrCmpOptions)/sizeof(kStrCmpOptions[0]) ,
		sizeof(kStrCmpOptions)/sizeof(kStrCmpOptions[0]) ,
		sizeof(kSizeCmpOptions)/sizeof(kSizeCmpOptions[0]) ,
		0 ,
		sizeof(kKindCmpOptions)/sizeof(kKindCmpOptions[0]) ,
		sizeof(kStrCmpOptions)/sizeof(kStrCmpOptions[0])
	}; 

enum EActionType
	{
		eTypeString = 0,
		eTypeSimpleString ,
		eTypeSize ,
		eTypeDate ,
		eTypeKind ,
		eTypeSubString
	} ;
const EActionType kActionTypes[] =
	{
		eTypeString ,
		eTypeSimpleString ,
		eTypeSize ,
		eTypeDate ,
		eTypeDate ,
		eTypeKind ,
		eTypeSubString ,
	} ;
	
#endif
