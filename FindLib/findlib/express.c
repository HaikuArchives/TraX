/* express.c -- manually add expressions to the expression tree.
   Copyright (C) 1999, Tim Vernum.
	
   Based upon parser.c 
	   Copyright (C) 1990, 91, 92, 93, 94 Free Software Foundation, Inc.
 
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
 
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
 
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */ 

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <grp.h>
#include <pwd.h>
#include <ctype.h>
#include <regex.h>

#include "config.h"
#include "modechange.h"
#include "defs.h"
#include "modetype.h"
#include "findlib.h"

static struct segment **make_segment P_((struct segment **segment, char *format, int len, int kind));
static boolean insert_num P_(( enum comparison_type c_type, unsigned long num, PFB pred));
static int insert_regex_main ( const char * regex, boolean ignore_case ) ;
static int insert_type_main ( unsigned long type_cell, boolean ( *which_pred ) () ) ;
static int insert_fprintf_main ( FILE * fp, boolean ( *func ) () , char * fmt ) ;
static int insert_days ( enum comparison_type c_type , unsigned long num_days, PFB pred ) ;
static int insert_time ( enum comparison_type c_type , time_t t , PFB pred ) ;

int ( *xstat ) () = lstat ;

#define ISDIGIT(c) (isascii (c) && isdigit (c))
#define ISUPPER(c) (isascii (c) && isupper (c))

int findlib_insert_amin ( enum comparison_type c_type ,
                  unsigned long num )
{
	struct predicate * our_pred ;

	our_pred = insert_primary ( pred_amin ) ;
	our_pred->args.info.kind = c_type ;
	our_pred->args.info.l_val = cur_day_start + DAYSECS - num * 60 ;

	return ( 0 ) ;
}

int findlib_insert_and ( void )
{
	struct predicate * our_pred ;

	our_pred = get_new_pred () ;
	our_pred->pred_func = pred_and ;
	our_pred->p_type = BI_OP ;
	our_pred->p_prec = AND_PREC ;
	our_pred->need_stat = false ;

	return ( 0 ) ;
}

int findlib_insert_anewer ( const char * than )
{
	struct predicate * our_pred ;
	struct stat stat_newer ;

	if ( ( *xstat ) ( than, &stat_newer ) != 0 )
		return 1 ;

	our_pred = insert_primary ( pred_anewer ) ;
	our_pred->args.time = stat_newer.st_mtime ;

	return ( 0 ) ;
}

int findlib_insert_adays ( enum comparison_type c_type ,
        		           unsigned long num )
{
	return insert_days ( c_type, num, pred_atime ) ;
}

int findlib_insert_atime (	enum comparison_type c_type ,
     						time_t t )
{
	return insert_time ( c_type, t, pred_atime ) ;
}

int findlib_insert_close_paren ( void )
{
	struct predicate * our_pred ;

	our_pred = get_new_pred () ;
	our_pred->pred_func = pred_close ;
	our_pred->p_type = CLOSE_PAREN ;
	our_pred->p_prec = NO_PREC ;
	our_pred->need_stat = false ;

	return ( 0 ) ;
}

int findlib_insert_cmin ( enum comparison_type c_type ,
                  unsigned long num )
{
	struct predicate * our_pred ;

	our_pred = insert_primary ( pred_cmin ) ;
	our_pred->args.info.kind = c_type ;
	our_pred->args.info.l_val = cur_day_start + DAYSECS - num * 60 ;

	return ( 0 ) ;
}

int findlib_insert_cnewer ( const char * than )
{
	struct predicate * our_pred ;
	struct stat stat_newer ;

	if ( ( *xstat ) ( than, &stat_newer ) != 0 )
		return 1 ;

	our_pred = insert_primary ( pred_cnewer ) ;
	our_pred->args.time = stat_newer.st_mtime ;

	return ( 0 ) ;
}

int findlib_insert_comma ( void )
{
	struct predicate * our_pred ;

	our_pred = get_new_pred () ;
	our_pred->pred_func = pred_comma ;

	our_pred->p_type = BI_OP ;
	our_pred->p_prec = COMMA_PREC ;
	our_pred->need_stat = false ;

	return ( 0 ) ;
}

int findlib_insert_cdays ( enum comparison_type c_type ,
							unsigned long num )
{
	return ( insert_days ( c_type, num, pred_ctime ) ) ;
}

int findlib_insert_ctime ( 	enum comparison_type c_type ,
							time_t t )
{
	return ( insert_time ( c_type, t, pred_ctime ) ) ;
}

int findlib_insert_crdays ( enum comparison_type c_type ,
							unsigned long num )
{
	return ( insert_days ( c_type, num, pred_crtime ) ) ;
}

int findlib_insert_crtime (	enum comparison_type c_type ,
							time_t t )
{
	return ( insert_time ( c_type, t, pred_crtime ) ) ;
}

int findlib_insert_daystart ( void )
{
	struct tm * local ;

	if ( full_days == false )
	{
		cur_day_start += DAYSECS ;
		local = localtime ( &cur_day_start ) ;
		cur_day_start -= local->tm_sec + local->tm_min * 60
                 		+ local->tm_hour * 3600 ;
		full_days = true ;
	}

	return ( 0 ) ;
}

int findlib_insert_depth ( void )
{
	do_dir_first = false ;
	return ( 0 ) ;
}

int findlib_insert_empty ( void )
{
	insert_primary ( pred_empty ) ;
	return ( 0 ) ;
}

/* Unsupported */
#if 0
int findlib_insert_exec ( argv, arg_ptr )
char *argv[] ;
int *arg_ptr ;
{
	return ( insert_exec_ok ( pred_exec, argv, arg_ptr ) ) ;
}
#endif

int findlib_insert_false ( void )
{
	struct predicate * our_pred ;

	our_pred = insert_primary ( pred_false ) ;
	our_pred->need_stat = false ;

	return ( 0 ) ;
}

int findlib_insert_fls ( FILE * fp )
{
	struct predicate * our_pred ;

	our_pred = insert_primary ( pred_fls ) ;
	our_pred->args.stream = fp ;
	our_pred->side_effects = true ;

	return ( 0 ) ;
}

int findlib_insert_fprintf ( FILE * fp , char * fmt )
{
	return insert_fprintf_main( fp, pred_fprintf, fmt ) ;
}

int findlib_insert_follow ( void )
{
	dereference = true ;
	xstat = stat ;
	no_leaf_check = true ;
	return ( 0 ) ;
}

int findlib_insert_fprint ( FILE * fp )
{
	struct predicate * our_pred ;

	our_pred = insert_primary ( pred_fprint ) ;
	our_pred->args.stream = fp ;
	our_pred->side_effects = true ;
	our_pred->need_stat = false ;

	return ( 0 ) ;
}

int findlib_insert_fprint0 ( FILE * fp )
{
	struct predicate * our_pred ;

	our_pred = insert_primary ( pred_fprint0 ) ;
	our_pred->args.stream = fp ;
	our_pred->side_effects = true ;
	our_pred->need_stat = false ;

	return ( 0 ) ;
}

/* Takes possession of string */
int findlib_insert_fstype ( char * fstype )
{
	struct predicate * our_pred ;

	our_pred = insert_primary ( pred_fstype ) ;
	our_pred->args.str = fstype ;

	return ( 0 ) ;
}

int findlib_insert_gid ( enum comparison_type c_type ,
                 unsigned long gid )
{
	return ( insert_num( c_type, gid, pred_gid ) ) ;
}

int findlib_insert_group ( const char * grp )
{
	struct group * cur_gr ;
	struct predicate *our_pred ;
	gid_t gid ;
	int gid_len ;

	cur_gr = getgrnam ( grp ) ;
	endgrent () ;

	if ( cur_gr != NULL )
		gid = cur_gr->gr_gid ;
	else
	{
		gid_len = strspn ( grp, "0123456789" ) ;
		if ( ( gid_len == 0 ) || ( grp[ gid_len ] != '\0' ) )
			return ( 1 ) ;
		gid = atoi( grp ) ;
	}
	our_pred = insert_primary ( pred_group ) ;
	our_pred->args.gid = gid ;

	return ( 0 ) ;
}

/* Takes possession of string */
int findlib_insert_ilname ( char * str )
{
	struct predicate * our_pred ;

	our_pred = insert_primary ( pred_ilname ) ;
	our_pred->args.str = str ;

	return ( 0 ) ;
}

/* Takes possession of string */
int findlib_insert_iname ( char * iname )
{
	struct predicate * our_pred ;

	our_pred = insert_primary ( pred_iname ) ;
	our_pred->need_stat = false ;
	our_pred->args.str = iname ;

	return ( 0 ) ;
}

int findlib_insert_inum ( enum comparison_type c_type ,
                  unsigned long inum )
{
	return ( insert_num ( c_type, inum, pred_inum ) ) ;
}

/* Takes possession of string */
int findlib_insert_ipath ( char * path )
{
	struct predicate * our_pred ;

	our_pred = insert_primary ( pred_ipath ) ;
	our_pred->need_stat = false ;
	our_pred->args.str = path ;

	return ( 0 ) ;
}

int findlib_insert_iregex ( const char * re )
{
	return insert_regex_main ( re, true ) ;
}

int findlib_insert_links ( enum comparison_type c_type ,
                   unsigned long links )
{
	return( insert_num ( c_type, links, pred_links ) ) ;
}

/* Takes possession of string */
int findlib_insert_lname ( char * name )
{
	struct predicate * our_pred ;

	our_pred = insert_primary ( pred_lname ) ;
	our_pred->args.str = name ;

	return ( 0 ) ;
}

int findlib_insert_ls ( void )
{
	struct predicate * our_pred ;

	our_pred = insert_primary ( pred_ls ) ;
	our_pred->side_effects = true ;

	return ( 0 ) ;
}

int findlib_insert_maxdepth ( unsigned long depth )
{
	maxdepth = depth ;
	return ( 0 ) ;
}

int findlib_insert_mindepth ( unsigned long depth )
{
	mindepth = depth ;
	return ( 0 ) ;
}

int findlib_insert_mmin ( enum comparison_type c_type ,
                  unsigned long num )
{
	struct predicate * our_pred ;

	our_pred = insert_primary ( pred_mmin ) ;
	our_pred->args.info.kind = c_type ;
	our_pred->args.info.l_val = cur_day_start + DAYSECS - num * 60 ;

	return ( 0 ) ;
}

int findlib_insert_mdays ( enum comparison_type c_type ,
                   unsigned long num )
{
	return ( insert_days ( c_type, num , pred_mtime ) ) ;
}

int findlib_insert_mtime (	enum comparison_type c_type ,
							time_t t )
{
	return ( insert_time ( c_type, t , pred_mtime ) ) ;
}

/* Takes possession of string */
int findlib_insert_name ( char * name )
{
	struct predicate * our_pred ;

	our_pred = insert_primary ( pred_name ) ;
	our_pred->need_stat = false ;
	our_pred->args.str = name ;

	return ( 0 ) ;
}

int findlib_insert_negate ( void )
{
	struct predicate * our_pred ;

	our_pred = get_new_pred_chk_op () ;
	our_pred->pred_func = pred_negate ;
	our_pred->p_type = UNI_OP ;
	our_pred->p_prec = NEGATE_PREC ;
	our_pred->need_stat = false ;

	return ( 0 ) ;
}

int findlib_insert_newer ( const char * than )
{
	struct predicate * our_pred ;
	struct stat stat_newer ;

	if ( ( *xstat ) ( than, &stat_newer ) != 0 )
		return 1 ;

	our_pred = insert_primary ( pred_newer ) ;
	our_pred->args.time = stat_newer.st_mtime ;

	return ( 0 ) ;
}

int findlib_insert_noleaf ( void )
{
	no_leaf_check = true ;
	return ( 0 ) ;
}

int findlib_insert_nogroup ( void )
{
	struct predicate * our_pred ;

	our_pred = insert_primary ( pred_nogroup ) ;
	return ( 0 ) ;
}

int findlib_insert_nouser ( void )
{
	struct predicate * our_pred ;

	our_pred = insert_primary ( pred_nouser ) ;
	return ( 0 ) ;
}

/* Unsupported */
#if 0
int findlib_insert_ok ( argv, arg_ptr )
char *argv[] ;
int *arg_ptr ;
{
	return ( insert_exec_ok ( pred_ok, argv, arg_ptr ) ) ;
}
#endif

int findlib_insert_open_paren ( void )
{
	struct predicate * our_pred ;

	our_pred = get_new_pred_chk_op () ;
	our_pred->pred_func = pred_open ;

	our_pred->p_type = OPEN_PAREN ;
	our_pred->p_prec = NO_PREC ;
	our_pred->need_stat = false ;

	return ( 0 ) ;
}

int findlib_insert_or ( void )
{
	struct predicate * our_pred ;

	our_pred = get_new_pred () ;
	our_pred->pred_func = pred_or ;
	our_pred->p_type = BI_OP ;
	our_pred->p_prec = OR_PREC ;
	our_pred->need_stat = false ;

	return ( 0 ) ;
}

/* Takes possession of string */
int findlib_insert_path ( char * path )
{
	struct predicate * our_pred ;

	our_pred = insert_primary ( pred_path ) ;
	our_pred->need_stat = false ;
	our_pred->args.str = path ;

	return ( 0 ) ;
}

int findlib_insert_perm_str ( char * perm )
{
	unsigned long perm_val ;
	int mode_start = 0 ;
	struct mode_change *change ;
	struct predicate *our_pred ;

	switch ( perm[ 0 ] )
	{
		case '-':
		case '+':
			mode_start = 1 ;
			break ;
		default:
			/* empty */
			break ;
	}

	change = mode_compile ( perm + mode_start, MODE_MASK_PLUS ) ;
	if ( change == MODE_INVALID )
		return 1 ;
	if ( change == MODE_MEMORY_EXHAUSTED )
		return 2 ;

	perm_val = mode_adjust ( 0, change ) ;
	mode_free ( change ) ;

	our_pred = insert_primary ( pred_perm ) ;

	switch ( perm[ 0 ] )
	{
		case '-':
			/* Set magic flag to indicate true if at least the given bits are set. */
			our_pred->args.perm = ( perm_val & 07777 ) | 010000 ;
			break ;

		case '+':
			/* Set magic flag to indicate true if any of the given bits are set. */
			our_pred->args.perm = ( perm_val & 07777 ) | 020000 ;
			break ;

		default:
			/* True if exactly the given bits are set. */
			our_pred->args.perm = ( perm_val & 07777 ) ;
			break ;
	}

	return ( 0 ) ;
}

int findlib_insert_perm_int ( enum mode_type type , unsigned long perm_val )
{
	struct predicate * our_pred ;

	our_pred = insert_primary ( pred_perm ) ;

	switch ( type )
	{
		case MODE_ALL:
			/* Set magic flag to indicate true if at least the given bits are set. */
			our_pred->args.perm = ( perm_val & 07777 ) | 010000 ;
			break ;

		case MODE_ANY:
			/* Set magic flag to indicate true if any of the given bits are set. */
			our_pred->args.perm = ( perm_val & 07777 ) | 020000 ;
			break ;

		case MODE_EXACT:
			/* True if exactly the given bits are set. */
			our_pred->args.perm = ( perm_val & 07777 ) ;
			break ;
	}

	return ( 0 ) ;
}

int findlib_insert_print ( void )
{
	struct predicate * our_pred ;

	our_pred = insert_primary ( pred_print ) ;
	/* -print has the side effect of printing.  This prevents us
	      from doing undesired multiple printing when the user has
	      already specified -print. */
	our_pred->side_effects = true ;
	our_pred->need_stat = false ;

	return ( 0 ) ;
}

int findlib_insert_print0 ( void )
{
	struct predicate * our_pred ;

	our_pred = insert_primary ( pred_print0 ) ;
	/* -print0 has the side effect of printing.  This prevents us
	      from doing undesired multiple printing when the user has
	      already specified -print0. */
	our_pred->side_effects = true ;
	our_pred->need_stat = false ;

	return ( 0 ) ;
}

int findlib_insert_printf ( char * fmt )
{
	return ( insert_fprintf_main ( stdout , pred_fprintf , fmt ) ) ;
}

int findlib_insert_prune ( void )
{
	struct predicate * our_pred ;

	our_pred = insert_primary ( pred_prune ) ;
	our_pred->need_stat = false ;

	return 0 ;
}

int findlib_insert_regex ( const char * re )
{
	return insert_regex_main ( re, false ) ;
}

static int insert_regex_main ( const char * regex, boolean ignore_case )
{
	struct predicate * our_pred ;
	struct re_pattern_buffer *re ;
	const char *error_message ;

	our_pred = insert_primary ( pred_regex ) ;
	our_pred->need_stat = false ;
	re = ( struct re_pattern_buffer * ) xmalloc ( sizeof ( struct re_pattern_buffer ) ) ;
	our_pred->args.regex = re ;
	our_pred->allocated_arg = ALLOCATED_RE ;
	re->allocated = 100 ;
	re->buffer = ( unsigned char * ) xmalloc ( re->allocated ) ;
	re->fastmap = NULL ;

	if ( ignore_case )
	{
		unsigned i ;

		re->translate = xmalloc ( 256 ) ;
		/* Map uppercase characters to corresponding lowercase ones.  */
		for ( i = 0 ; i < 256 ; i++ )
			re->translate[ i ] = ISUPPER ( i ) ? tolower ( i ) : i ;
	}
	else
		re->translate = NULL ;

	error_message = re_compile_pattern ( regex, strlen ( regex ), re ) ;
	if ( error_message )
		return 1 ;

	return ( 0 ) ;
}

int findlib_insert_size ( enum comparison_type c_type ,
                  unsigned long num ,
                  int blksize )
{
	struct predicate * our_pred ;

	our_pred = insert_primary ( pred_size ) ;
	our_pred->args.size.kind = c_type ;
	our_pred->args.size.blocksize = blksize ;
	our_pred->args.size.size = num ;

	return ( 0 ) ;
}

int findlib_insert_fsize ( enum comparison_type c_type ,
                  float num ,
                  int blksize )
{
	struct predicate * our_pred ;

	our_pred = insert_primary ( pred_size ) ;
	our_pred->args.size.kind = c_type ;
	our_pred->args.size.blocksize = 1 ;
	our_pred->args.size.size = (num * blksize) ;

	return ( 0 ) ;
}

int findlib_insert_true ( void )
{
	struct predicate * our_pred ;

	our_pred = insert_primary ( pred_true ) ;
	our_pred->need_stat = false ;

	return ( 0 ) ;
}

int findlib_insert_type ( unsigned long type )
{
	return insert_type_main ( type, pred_type ) ;
}

int findlib_insert_uid ( enum comparison_type c_type ,
                 unsigned long uid )
{
	return ( insert_num ( c_type, uid, pred_uid ) ) ;
}

int findlib_insert_used ( enum comparison_type c_type ,
                  unsigned long num_days )
{
	struct predicate * our_pred ;

	our_pred = insert_primary ( pred_used ) ;
	our_pred->args.info.kind = c_type ;
	our_pred->args.info.l_val = num_days * DAYSECS ;

	return ( 0 ) ;
}

int findlib_insert_user ( const char * user )
{
	struct passwd * cur_pwd ;
	struct predicate *our_pred ;
	uid_t uid ;
	int uid_len ;

	cur_pwd = getpwnam ( user ) ;
	endpwent () ;
	if ( cur_pwd != NULL )
		uid = cur_pwd->pw_uid ;
	else
	{
		uid_len = strspn ( user, "0123456789" ) ;
		if ( ( uid_len == 0 ) || ( user[ uid_len ] != '\0' ) )
			return ( false ) ;
		uid = atoi ( user ) ;
	}
	our_pred = insert_primary ( pred_user ) ;
	our_pred->args.uid = uid ;

	return ( 0 ) ;
}

int findlib_insert_xdev ( void )
{
	stay_on_filesystem = true ;
	return ( 0 ) ;
}

int findlib_insert_xtype ( unsigned long type_cell )
{
	return insert_type_main ( type_cell, pred_xtype ) ;
}

static int insert_type_main ( unsigned long type_cell,
                       boolean ( *which_pred ) () )
{
	struct predicate * our_pred ;

	our_pred = insert_primary ( which_pred ) ;
	our_pred->args.type = type_cell ;

	return ( 0 ) ;
}

/* If true, we've determined that the current fprintf predicate
   uses stat information. */
static boolean fprintf_stat_needed ;

static int insert_fprintf_main ( FILE * fp, boolean ( *func ) () , char * fmt )
{
	char * format ; /* Beginning of unprocessed format string. */
	register char *scan ; /* Current address in scanning `format'. */
	register char *scan2 ; /* Address inside of element being scanned. */
	struct segment **segmentp ; /* Address of current segment. */
	struct predicate *our_pred ;

	format = fmt ;

	fprintf_stat_needed = false ; /* Might be overridden later. */
	our_pred = insert_primary ( func ) ;
	our_pred->side_effects = true ;
	our_pred->args.printf_vec.stream = fp ;
	our_pred->allocated_arg = ALLOCATED_FPRINTF ;

	segmentp = &our_pred->args.printf_vec.segment ;
	*segmentp = NULL ;

	for ( scan = format ; *scan ; scan++ )
	{
		if ( *scan == '\\' )
		{
			scan2 = scan + 1 ;
			if ( *scan2 >= '0' && *scan2 <= '7' )
			{
				register int n, i ;

				for ( i = n = 0 ;
        					i < 3 && ( *scan2 >= '0' && *scan2 <= '7' ) ;
        					i++, scan2++ )
					n = 8 * n + *scan2 - '0' ;
				scan2-- ;
				*scan = n ;
			}
			else
			{
				switch ( *scan2 )
				{
					case 'a':
						*scan = 7 ;
						break ;

					case 'b':
						*scan = '\b' ;
						break ;

					case 'c':
						make_segment ( segmentp, format, scan - format, KIND_STOP ) ;
						our_pred->need_stat = fprintf_stat_needed ;
						return ( true ) ;

					case 'f':
						*scan = '\f' ;
						break ;

					case 'n':
						*scan = '\n' ;
						break ;

					case 'r':
						*scan = '\r' ;
						break ;

					case 't':
						*scan = '\t' ;
						break ;

					case 'v':
						*scan = '\v' ;
						break ;

					case '\\':
						/* *scan = '\\'; * it already is */
						break ;

					default:
						/*	error (0, 0, "warning: unrecognized escape `\\%c'", *scan2); */
						scan++ ;
						continue ;
				}
			}

			segmentp = make_segment ( segmentp, format, scan - format + 1,
                          			KIND_PLAIN ) ;
			format = scan2 + 1 ; /* Move past the escape. */
			scan = scan2 ; /* Incremented immediately by `for'. */
		}
		else if ( *scan == '%' )
		{
			if ( scan[ 1 ] == '%' )
			{
				segmentp = make_segment ( segmentp, format, scan - format + 1,
                          				KIND_PLAIN ) ;
				scan++ ;
				format = scan + 1 ;
				continue ;
			}

			/* Scan past flags, width and precision, to verify kind. */
			for ( scan2 = scan ; *++scan2 && strchr ( "-+ #", *scan2 ) ; )
				/* Do nothing. */ ;
				
			while ( ISDIGIT ( *scan2 ) )
				scan2++ ;

			if ( *scan2 == '.' )
				for ( scan2++ ; ISDIGIT ( *scan2 ) ; scan2++ )
					/* Do nothing. */ ;
					
			if ( strchr ( "abcdfFgGhHiklmnpPstuU", *scan2 ) )
			{
				segmentp = make_segment ( segmentp, format, scan2 - format,
                          				( int ) * scan2 ) ;
				scan = scan2 ;
				format = scan + 1 ;
			}
			else if ( strchr ( "ACT", *scan2 ) && scan2[ 1 ] )
			{
				segmentp = make_segment ( segmentp, format, scan2 - format,
                          				*scan2 | ( scan2[ 1 ] << 8 ) ) ;
				scan = scan2 + 1 ;
				format = scan + 1 ;
				continue ;
			}
			else
			{
				/* An unrecognized % escape.  Print the char after the %. */
				error ( 0, 0, "warning: unrecognized format directive `%%%c'",
        				*scan2 ) ;
				segmentp = make_segment ( segmentp, format, scan - format,
                          				KIND_PLAIN ) ;
				format = scan + 1 ;
				continue ;
			}
		}
	}

	if ( scan > format )
		make_segment ( segmentp, format, scan - format, KIND_PLAIN ) ;
	our_pred->need_stat = fprintf_stat_needed ;

	return ( 0 ) ;
}

/* Create a new fprintf segment in *SEGMENT, with type KIND,
   from the text in FORMAT, which has length LEN.
   Return the address of the `next' pointer of the new segment. */ 

static struct segment **
			make_segment ( segment, format, len, kind )
			struct segment **segment ;
char *format ;
int len, kind ;
{
	char *fmt ;

	*segment = ( struct segment * ) xmalloc ( sizeof ( struct segment ) ) ;

	( *segment ) ->kind = kind ;
	( *segment ) ->next = NULL ;
	( *segment ) ->text_len = len ;

	fmt = ( *segment ) ->text = xmalloc ( len + 3 ) ; /* room for "ld\0" */
	strncpy ( fmt, format, len ) ;
	fmt += len ;

	switch ( kind & 0xff )
	{
		case KIND_PLAIN: /* Plain text string, no % conversion. */
		case KIND_STOP: /* Terminate argument, no newline. */
			break ;

		case 'a': /* atime in `ctime' format */
		case 'c': /* ctime in `ctime' format */
		case 'F': /* filesystem type */
		case 'g': /* group name */
		case 'l': /* object of symlink */
		case 't': /* mtime in `ctime' format */
		case 'u': /* user name */
		case 'A': /* atime in user-specified strftime format */
		case 'C': /* ctime in user-specified strftime format */
		case 'T': /* mtime in user-specified strftime format */
			fprintf_stat_needed = true ;
			/* FALLTHROUGH */
		case 'f': /* basename of path */
		case 'h': /* leading directories part of path */
		case 'H': /* ARGV element file was found under */
		case 'p': /* pathname */
		case 'P': /* pathname with ARGV element stripped */
			*fmt++ = 's' ;
			break ;

		case 'b': /* size in 512-byte blocks */
		case 'k': /* size in 1K blocks */
		case 's': /* size in bytes */

#if FS_IS64BIT
			*fmt++ = 'L' ;
#else
			*fmt++ = 'l' ;
#endif

			/*FALLTHROUGH*/
		case 'n': /* number of links */
			fprintf_stat_needed = true ;
			/* FALLTHROUGH */
		case 'd': /* depth in search tree (0 = ARGV element) */
			*fmt++ = 'd' ;
			break ;

		case 'i': /* inode number */
#if FS_IS64BIT
			*fmt++ = 'L' ;
#else
			*fmt++ = 'l' ;
#endif
			/*FALLTHROUGH*/
		case 'G': /* GID number */
		case 'U': /* UID number */
			*fmt++ = 'u' ;
			fprintf_stat_needed = true ;
			break ;

		case 'm': /* mode as octal number (perms only) */
			*fmt++ = 'o' ;
			fprintf_stat_needed = true ;
			break ;
	}
	*fmt = '\0' ;

	return ( &( *segment ) ->next ) ;
}

/* Not currently supported in FindLib */
#if 0
static boolean
insert_exec_ok ( func, argv, arg_ptr )
boolean ( *func ) () ;
char *argv[] ;
int *arg_ptr ;
{
	int start, end ; /* Indexes in ARGV of start & end of cmd. */
	int num_paths ; /* Number of args with path replacements. */
	int path_pos ; /* Index in array of path replacements. */
	int vec_pos ; /* Index in array of args. */
	struct predicate *our_pred ;
	struct exec_val *execp ; /* Pointer for efficiency. */

	if ( ( argv == NULL ) || ( argv[ *arg_ptr ] == NULL ) )
		return ( false ) ;

	/* Count the number of args with path replacements, up until the ';'. */
	start = *arg_ptr ;
	for ( end = start, num_paths = 0 ;
        		( argv[ end ] != NULL )
        		&& ( ( argv[ end ][ 0 ] != ';' ) || ( argv[ end ][ 1 ] != '\0' ) ) ;
        		end++ )
		if ( strstr ( argv[ end ], "{}" ) )
			num_paths++ ;
	/* Fail if no command given or no semicolon found. */
	if ( ( end == start ) || ( argv[ end ] == NULL ) )
	{
		*arg_ptr = end ;
		return ( false ) ;
	}

	our_pred = insert_primary ( func ) ;
	our_pred->side_effects = true ;
	execp = &our_pred->args.exec_vec ;
	execp->paths =
    	( struct path_arg * ) xmalloc ( sizeof ( struct path_arg ) * ( num_paths + 1 ) ) ;
	execp->vec = ( char ** ) xmalloc ( sizeof ( char * ) * ( end - start + 1 ) ) ;
	/* Record the positions of all args, and the args with path replacements. */
	for ( end = start, path_pos = vec_pos = 0 ;
        		( argv[ end ] != NULL )
        		&& ( ( argv[ end ][ 0 ] != ';' ) || ( argv[ end ][ 1 ] != '\0' ) ) ;
        		end++ )
	{
		register char *p ;

		execp->paths[ path_pos ].count = 0 ;
		for ( p = argv[ end ] ; *p ; ++p )
			if ( p[ 0 ] == '{' && p[ 1 ] == '}' )
			{
				execp->paths[ path_pos ].count++ ;
				++p ;
			}
		if ( execp->paths[ path_pos ].count )
		{
			execp->paths[ path_pos ].offset = vec_pos ;
			execp->paths[ path_pos ].origarg = argv[ end ] ;
			path_pos++ ;
		}
		execp->vec[ vec_pos++ ] = argv[ end ] ;
	}
	execp->paths[ path_pos ].offset = -1 ;
	execp->vec[ vec_pos ] = NULL ;

	if ( argv[ end ] == NULL )
		* arg_ptr = end ;
	else
		*arg_ptr = end + 1 ;
	return ( true ) ;
}
#endif


/* Insert a time predicate PRED.
 
   A new predicate node is assigned, along with an argument node
   obtained with malloc.
 
   Used by -atime, -ctime, and -mtime parsers. */ 

static int insert_days ( enum comparison_type c_type ,
                  unsigned long num_days,
                  PFB pred )
{
	struct predicate * our_pred ;

	our_pred = insert_primary ( pred ) ;
	our_pred->args.info.kind = c_type ;
	our_pred->args.info.l_val = cur_day_start - num_days * DAYSECS
                            	+ ( ( c_type == COMP_GT ) ? DAYSECS - 1 : 0 ) ;

	return ( 0 ) ;
}

static int insert_time (	enum comparison_type c_type ,
							time_t t ,
							PFB pred )
{
	struct predicate * our_pred ;

	our_pred = insert_primary ( pred ) ;
	our_pred->args.info.kind = c_type ;
	our_pred->args.info.l_val = t ;

	return ( 0 ) ;
}

/* Insert a number predicate.
   *PRED is the predicate processor to insert.
 
   A new predicate node is assigned, along with an argument node
   obtained with malloc.
 
   Used by -inum and -links parsers. */ 

static int insert_num (	enum comparison_type c_type ,
						unsigned long num ,
						PFB pred )
{
	struct predicate *our_pred ;

	our_pred = insert_primary ( pred ) ;
	our_pred->args.info.kind = c_type ;
	our_pred->args.info.l_val = num ;

	return ( 0 ) ;
}

static FILE * open_output_file ( char * path )
{
	FILE *f ;

	if ( strcmp ( path, "/dev/stderr" ) == 0 )
		return ( stderr ) ;
	else if ( strcmp ( path, "/dev/stdout" ) == 0 )
		return ( stdout ) ;

	f = fopen ( path, "w" ) ;
	if ( f == NULL )
		return NULL ;
	return ( f ) ;
}

