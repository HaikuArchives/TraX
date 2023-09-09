#ifndef CONFIG_H
#define CONFIG_H

/*
 *	This sort of stuff should end up being autoconf generated.
 *	Well, maybe one day....
 *
 */

#define HAVE_ALLOCA 1

/* Define if you have <alloca.h> and it should be used (not on Ultrix).  */
#define HAVE_ALLOCA_H 1

/* Define if your struct stat has st_rdev.  */
#define HAVE_ST_RDEV 1

/* Define if you have the strftime function.  */
#define HAVE_STRFTIME 1

/* Define if you have <sys/wait.h> that is POSIX.1 compatible.  */
#define HAVE_SYS_WAIT_H 1

/* Define if your struct tm has tm_zone.  */
#define HAVE_TM_ZONE 1

/* Define if you have the vprintf function.  */
#define HAVE_VPRINTF 1

/* Define if major, minor, and makedev are declared in <sysmacros.h>.  */
#define MAJOR_IN_SYSMACROS 1

/* If using the C implementation of alloca, define if you know the
   direction of stack growth for your system; otherwise it will be
   automatically deduced at run-time.
	STACK_DIRECTION > 0 => grows toward higher addresses
	STACK_DIRECTION < 0 => grows toward lower addresses
	STACK_DIRECTION = 0 => direction of growth unknown
 */
#define STACK_DIRECTION -1

/* Define if you have the ANSI C header files.  */
#define STDC_HEADERS 1

/* Define if you have the getcwd function.  */
#define HAVE_GETCWD 1

/* Define if you have the strerror function.  */
#define HAVE_STRERROR 1

/* Define if you have the <dirent.h> header file.  */
#define HAVE_DIRENT_H 1

/* Define if you have the <sys/dir.h> header file.  */
#define HAVE_SYS_DIR_H 1

/* Define if you have the <fcntl.h> header file.  */
#define HAVE_FCNTL_H 1

/* Define if you have the <limits.h> header file.  */
#define HAVE_LIMITS_H 1

/* Define if you have the <string.h> header file.  */
#define HAVE_STRING_H 1

/* Define if you have the <unistd.h> header file.  */
#define HAVE_UNISTD_H 1

/* For 64bit filesystems, we need to put 'L' in printf, rather than a 'l' */
#undef FS_IS_64BIT

#if __BEOS__ || __HAIKU__
	#define FS_IS_64BIT 1
#else
	#define FS_IS_64BIT 0
#endif


#endif		/* CONFIG_H */


