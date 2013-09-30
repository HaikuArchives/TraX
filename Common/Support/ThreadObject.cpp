#ifndef   THREADOBJECT_H
#include "ThreadObject.h"
#endif

/*******************************************************
 * (c) 1999, Tim Vernum <zodsoft@kagi.com>
 * All rights reserved
 *******************************************************
 * File   : ThreadObject.cpp
 * Author : Tim Vernum
 * Project: Trax Application
 * Purpose: Class to encapsulate BeOS threading
 * Licence: AUL
 *******************************************************/

#include <string.h>

//===================================================
// class ThreadObject
//===================================================

Threads:: ThreadObject:: ThreadObject( const char * name  )
: fThread( -1 ) , fName( name ? strdup(name) : NULL )
{
}

Threads:: ThreadObject:: ~ThreadObject( void )
{
	KillThread() ;
	delete[] fName ;
}


void Threads:: ThreadObject:: SetThreadName( const char * n )
{
	delete[] fName ;
	fName = n ? strdup( n ) : NULL ;
}
				
thread_id Threads:: ThreadObject:: ThreadId( void ) const
{
	return fThread ;
}
		
thread_id Threads:: ThreadObject:: StartThread( int32 priority  )
{
	fThread = spawn_thread( StartThreadRunning, fName, priority, this ) ;
	if( fThread > -1 )
		resume_thread( fThread ) ;
	return fThread ;
}

status_t Threads:: ThreadObject:: KillThread( void ) 
{
	status_t s = kill_thread( fThread ) ;
	if( s == B_OK )
		ThreadKilled() ;
	return s ;
}

status_t Threads:: ThreadObject:: SuspendThread( void )
{
	return suspend_thread( fThread ) ;
}
status_t Threads:: ThreadObject:: ResumeThread( void )
{
	return resume_thread( fThread ) ;
}

int32 Threads:: ThreadObject:: StartThreadRunning( void * v )
{
	ThreadObject * to = reinterpret_cast<ThreadObject *>( v ) ;
	int32 code = to->PerformThreadTask() ;
	to->ThreadFinished( code ) ;
	return code ;
}
