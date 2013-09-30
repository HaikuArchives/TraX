#ifndef FREGEX_DEFS_H
#define FREGEX_DEFS_H

/*
 * (C) Copyright 1999, Tim Vernum
 * This file is licenced under the LGPL.
 * The fast regex package is under the GPL.
 */

#include <sys/types.h>
#include <regex.h>

#include "../lib/fast_regex.h"

#include "dfa.h"
#include "kwset.h"
#include "build.h"

struct _fregex
{
	struct re_pattern_buffer regex ;
	struct dfa dfa ;
	kwset_t kwset ;
	unsigned short num_exact_kws ; 
	unsigned long options ;
} ;

#endif
