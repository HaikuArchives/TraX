#ifndef FINDLIBTHREAD_H
#define FINDLIBTHREAD_H

#ifndef   THREADOBJECT_H
#include "ThreadObject.h"
#endif

#ifndef   BENAPHORE_H
#include "Benaphore.h"
#endif

#include <Path.h>

/*
 *       Class: FindLibThread
 *   Copyright: Tim Vernum 1999, All rights reserved
 *     Licence: GPL
 */

class FindLibThread : public Threads::ThreadObject
{
	public:
		FindLibThread( ) ;
		
		BPath Path ;

		static	bool	InitFind( const char * ) ;
		static	bool	UninitFind( ) ;
		
	protected:
		
		int32	PerformThreadTask( void ) ;
		void	ThreadFinished( int32 ) ;
		void	ThreadKilled( void ) ;

	private:
		static	Benaphore fLock ;

} ;

#endif
