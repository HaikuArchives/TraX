#ifndef FINDLIB_H
#define FINDLIB_H

/*
 *        File: findlib.h
 *   Copyright: Tim Vernum 1999, All rights reserved
 *   Copyright: (C) 1990, 91, 92, 93, 94 Free Software Foundation, Inc.
 *     Licence: GPL (includes parts of GNU find)
 */

#include <stdio.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef _FINDLIB_EXPORT
#define _FINDLIB_EXPORT
#endif

/* findlib.c */
_FINDLIB_EXPORT void findlib_init( const char * program ) ;

typedef int( *print_function)( const char * ) ;
_FINDLIB_EXPORT void findlib_set_print( print_function fn ) ;

_FINDLIB_EXPORT void findlib_start_predicates( void ) ;
_FINDLIB_EXPORT void findlib_end_predicates( void ) ;

_FINDLIB_EXPORT int findlib_find( int pathc, char ** pathv ) ;

_FINDLIB_EXPORT void findlib_end( void ) ;

/* exress.c */
enum mode_type
{
    MODE_ALL ,
    MODE_ANY ,
    MODE_EXACT
} ;

enum comparison_type
{
    COMP_GT,
    COMP_LT,
    COMP_EQ
} ;

_FINDLIB_EXPORT int findlib_insert_amin ( enum comparison_type c_type , unsigned long num );
_FINDLIB_EXPORT int findlib_insert_and ( void );
_FINDLIB_EXPORT int findlib_insert_anewer ( const char * than );
_FINDLIB_EXPORT int findlib_insert_adays ( enum comparison_type c_type , unsigned long num );
_FINDLIB_EXPORT int findlib_insert_atime ( enum comparison_type c_type , time_t t );
_FINDLIB_EXPORT int findlib_insert_close_paren ( void );
_FINDLIB_EXPORT int findlib_insert_cmin ( enum comparison_type c_type , unsigned long num );
_FINDLIB_EXPORT int findlib_insert_cnewer ( const char * than );
_FINDLIB_EXPORT int findlib_insert_comma ( void );
_FINDLIB_EXPORT int findlib_insert_cdays ( enum comparison_type c_type , unsigned long num );
_FINDLIB_EXPORT int findlib_insert_ctime ( enum comparison_type c_type , time_t t );
_FINDLIB_EXPORT int findlib_insert_crdays ( enum comparison_type c_type , unsigned long num );
_FINDLIB_EXPORT int findlib_insert_crtime ( enum comparison_type c_type , time_t t );
_FINDLIB_EXPORT int findlib_insert_daystart ( void );
_FINDLIB_EXPORT int findlib_insert_depth ( void );
_FINDLIB_EXPORT int findlib_insert_empty ( void );
_FINDLIB_EXPORT int findlib_insert_false ( void );
_FINDLIB_EXPORT int findlib_insert_fls ( FILE * fp );
_FINDLIB_EXPORT int findlib_insert_fprintf ( FILE * fp , char * fmt );
_FINDLIB_EXPORT int findlib_insert_follow ( void );
_FINDLIB_EXPORT int findlib_insert_fprint ( FILE * fp );
_FINDLIB_EXPORT int findlib_insert_fprint0 ( FILE * fp );
_FINDLIB_EXPORT int findlib_insert_fstype ( char * fstype );
_FINDLIB_EXPORT int findlib_insert_gid ( enum comparison_type c_type , unsigned long num );
_FINDLIB_EXPORT int findlib_insert_group ( const char * grp );
_FINDLIB_EXPORT int findlib_insert_ilname ( char * str );
_FINDLIB_EXPORT int findlib_insert_iname ( char * iname );
_FINDLIB_EXPORT int findlib_insert_inum ( enum comparison_type c_type , unsigned long num );
_FINDLIB_EXPORT int findlib_insert_ipath ( char * path );
_FINDLIB_EXPORT int findlib_insert_iregex ( const char * re );
_FINDLIB_EXPORT int findlib_insert_links ( enum comparison_type c_type , unsigned long num );
_FINDLIB_EXPORT int findlib_insert_lname ( char * name );
_FINDLIB_EXPORT int findlib_insert_ls ( void );
_FINDLIB_EXPORT int findlib_insert_maxdepth ( unsigned long depth );
_FINDLIB_EXPORT int findlib_insert_mindepth ( unsigned long depth );
_FINDLIB_EXPORT int findlib_insert_mmin ( enum comparison_type c_type , unsigned long num );
_FINDLIB_EXPORT int findlib_insert_mdays ( enum comparison_type c_type , unsigned long num );
_FINDLIB_EXPORT int findlib_insert_mtime ( enum comparison_type c_type , time_t num );
_FINDLIB_EXPORT int findlib_insert_name ( char * name );
_FINDLIB_EXPORT int findlib_insert_negate ( void );
_FINDLIB_EXPORT int findlib_insert_newer ( const char * than );
_FINDLIB_EXPORT int findlib_insert_noleaf ( void );
_FINDLIB_EXPORT int findlib_insert_nogroup ( void );
_FINDLIB_EXPORT int findlib_insert_nouser ( void );
_FINDLIB_EXPORT int findlib_insert_open_paren ( void );
_FINDLIB_EXPORT int findlib_insert_or ( void );
_FINDLIB_EXPORT int findlib_insert_path ( char * path );
_FINDLIB_EXPORT int findlib_insert_perm_str ( char * perm );
_FINDLIB_EXPORT int findlib_insert_perm_int ( enum mode_type type , unsigned long perm_val );
_FINDLIB_EXPORT int findlib_insert_print ( void );
_FINDLIB_EXPORT int findlib_insert_print0 ( void );
_FINDLIB_EXPORT int findlib_insert_printf ( char * fmt );
_FINDLIB_EXPORT int findlib_insert_prune ( void );
_FINDLIB_EXPORT int findlib_insert_regex ( const char * re );
_FINDLIB_EXPORT int findlib_insert_size ( enum comparison_type c_type , unsigned long num, int blksize );
_FINDLIB_EXPORT int findlib_insert_fsize ( enum comparison_type c_type , float num, int blksize );
_FINDLIB_EXPORT int findlib_insert_true ( void );
_FINDLIB_EXPORT int findlib_insert_type ( unsigned long type );
_FINDLIB_EXPORT int findlib_insert_uid ( enum comparison_type c_type , unsigned long num );
_FINDLIB_EXPORT int findlib_insert_used ( enum comparison_type c_type , unsigned long num );
_FINDLIB_EXPORT int findlib_insert_user ( const char * user );
_FINDLIB_EXPORT int findlib_insert_xdev ( void );
_FINDLIB_EXPORT int findlib_insert_xtype ( unsigned long type_cell );

_FINDLIB_EXPORT int findlib_insert_contains_str( const char * str ) ;
_FINDLIB_EXPORT int findlib_insert_contains_istr( const char * str ) ;
_FINDLIB_EXPORT int findlib_insert_contains_re( const char * str ) ;
_FINDLIB_EXPORT int findlib_insert_contains_ire( const char * str ) ;


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif