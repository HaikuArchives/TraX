 /*
 * This file is pulbic domain
 * The fast regex package is licensed under the GPL.
 */

#ifndef _EXPORT
 #define	_EXPORT			__declspec(dllexport)
#endif

#ifndef _IMPORT
 #define	_IMPORT			__declspec(dllimport)
#endif

_EXPORT int (*fast_regex_subsys_error)( const char * sys, const char * msg ) ;

_EXPORT int  allocate_fast_regex ( struct fast_regex * ) ;
_EXPORT int  compile_fast_regex  ( struct fast_regex *, const char *, size_t ) ;
_EXPORT int  evaluate_fast_regex ( struct fast_regex *, char *, size_t ) ;
_EXPORT void dispose_fast_regex  ( struct fast_regex * ) ;
