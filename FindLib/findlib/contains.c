#include "defs.h"
#include "fast_regex.h"

#include <stdio.h>
#include <string.h>

/*
 * We limit the search_buffer to 5k
 *  if a line is over that length, then the regexp matching will screw up,
 *  but for our purposes that is ok.
 * It's better (IMHO) than the method grep uses, which leaks memory
 */
#define BUFFER_SIZE (5*1024)

boolean search_file( FILE * file , struct fast_regex * fre )
{
 /* If this gets used in a larger app, then BeOS might scream at wasting 5k of
    the stack. Deal with it then */
	char search_buffer[BUFFER_SIZE] ;
	char * current_pos ;
	char * eos; 
	int buffer_limit = BUFFER_SIZE - 2 ;
	int space ;

	memset( search_buffer, BUFFER_SIZE, 0 ) ;
	space = buffer_limit ;
		
	current_pos = search_buffer ;
	eos = &search_buffer[ buffer_limit ] ;
	
	while( !feof( file ) )
	{
		char * end ;
		int rd ;
		
		rd = fread( current_pos, 1, space, file ) ;
		if( rd <= 0 )
			break ;
		current_pos[rd] = 0 ;
		eos = &current_pos[ rd ] ;
		
		space = buffer_limit  ;
		current_pos = search_buffer ;
		end = current_pos ;
		for( ; ; )
		{
			int len ;

			while( *end != '\n' && end != eos )
				end++ ;
				
			if( end == eos && current_pos != search_buffer )
				break ;
			
			len = end - current_pos ;
			if( evaluate_fast_regex( fre, current_pos, len ) )
				return 1;
				
			while( *end == '\n' && end != eos )
				end++ ;
			len = end - current_pos ;
			space -= (len) ;
			current_pos = end ;
			
			if( !space )
			{
				space = 128 ;
				break ;
			}
		}
		
		memmove( search_buffer, current_pos, space ) ;
		current_pos = search_buffer + space ;
		space = buffer_limit - space ;
	}
	return 0 ;
}
	
static boolean pred_contains ( char * pathname, struct stat * stat_buf, struct predicate * pred_ptr )
{
	FILE * file ;

	file = fopen( pathname, "r" ) ;
	if( file )
	{
		boolean ok ;
		ok = ( search_file( file, pred_ptr->args.fregex ) > 0 ) ;
		fclose( file ) ;
		return ok ;
	}

	return 0 ;
}

static int insert_contains_main( const char * str, boolean regex, boolean ignore_case )
{
	struct predicate * our_pred ;
	struct fast_regex * fre ;

	our_pred = insert_primary ( pred_contains ) ;
	our_pred->need_stat = 0 ;

	fre = ( struct fast_regex * ) xmalloc ( sizeof ( struct fast_regex ) ) ;
	our_pred->args.fregex = fre ;
	our_pred->allocated_arg = ALLOCATED_FRE ;
	
	if( ignore_case )
		fre->options = FRE_CASE_INSENSITIVE ;
	else
		fre->options = 0 ;

	if( !regex )
		fre->options |= FRE_STRING_SEARCH ;
		
	allocate_fast_regex( fre ) ;
	compile_fast_regex( fre, str, strlen(str) ) ;
	return 0 ;
}	
	
int compile_fast_regex( struct fast_regex *, const char *, size_t len ) ;
	
int findlib_insert_contains_str( const char * str )
{
	return insert_contains_main( str, 0, 0 ) ;
}
int findlib_insert_contains_istr( const char * str )
{
	return insert_contains_main( str, 0, 1 ) ;
}
int findlib_insert_contains_re( const char * str )
{
	return insert_contains_main( str, 1, 0 ) ;
}
int findlib_insert_contains_ire( const char * str )
{
	return insert_contains_main( str, 1, 1 ) ;
}
