#ifndef THREADOBJECT_H
#define THREADOBJECT_H

/*******************************************************
 * (c) 1999, Tim Vernum <zodsoft@kagi.com>
 * All rights reserved
 *******************************************************
 * File   : ThreadObject.h
 * Author : Tim Vernum
 * Project: Trax Application
 * Purpose: Class to encapsulate BeOS threading
 * Licence: AUL
 *******************************************************/

#include <SupportDefs.h>
#include <OS.h>

namespace Threads
{
	class ThreadObject
	{
		protected:
								 ThreadObject( const char * ) ;
			virtual				~ThreadObject( void ) ;
					void		SetThreadName( const char * ) ;
					
			virtual	int32		PerformThreadTask() = 0 ;
			virtual	void		ThreadFinished( int32 ) { } ;
			virtual	void		ThreadKilled( void ) { } ;
	
		public:
					thread_id	ThreadId() const ;
			
			virtual	thread_id	StartThread( int32 priority = B_NORMAL_PRIORITY ) ;
			virtual	status_t	KillThread( void ) ;
			
			virtual	status_t	SuspendThread( void ) ;
			virtual	status_t	ResumeThread( void ) ;
	
		private:
			static	int32		StartThreadRunning( void * ) ;
					thread_id	fThread ;
					char *		fName ;
	} ;
}

#endif
