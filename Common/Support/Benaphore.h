/*--------------------------------------------------------------------*\
  File:      Benaphore.h
  Creator:   Matt Bogosian <mattb@columbia.edu>
  Copyright: (c)1997, Matt Bogosian. All rights reserved.
  Description: Header file for Benaphore.cpp.
  ID:        $Id: Benaphore.h,v 1.2 1999/06/21 18:38:09 zod Exp zod $
  Conventions:
      #defines - all uppercase letters with words separated by
          underscores.
          (E.G., #define MY_DEFINE 5).
      New data types (classes, structs, typedefs, etc.) - begin with
          an uppercase letter followed by lowercase words separated by
          uppercase letters.
          (E.G., typedef int MyTypedef;).
      Global constants (declared with const) - begin with "k_"
          followed by lowercase words separated by underscores.
          (E.G., const int k_my_constant = 5;).
      Global variables - begin with "g_" followed by lowercase words
          separated by underscores.
          (E.G., int g_my_global;).
      Local variables - begin with a lowercase letter followed by
          lowercase words separated by underscores.
          (E.G., int my_local;).
      Argument variables - begin with "a_" followed by lowercase words
          separated by underscores.
          (E.G., ...int a_my_argument, ...).
      Member variables - begin with "m_" followed by lowercase words
          separated by underscores.
          (E.G., int m_my_member;).
      Functions - begin with a lowercase letter followed by lowercase
          words separated by uppercase letters.
          (E.G., void myFunction(void);).
      Member Functions - begin with an uppercase letter followed by
          lowercase words separated by uppercase letters.
          (E.G., void MyClass::MyFunction(void);).
\*--------------------------------------------------------------------*/

/*
	Changes (Grab function) added by Tim Vernum 1999,
		placed in the public domain
*/

/*--------------------------------------------------------------------*\
  =-=-=-=-=-=-=-=-=-=-=-=-=- Included Files -=-=-=-=-=-=-=-=-=-=-=-=-=
\*--------------------------------------------------------------------*/

#include <OS.h>


#ifndef BENAPHORE_H
#define BENAPHORE_H


/*--------------------------------------------------------------------*\
  =-=-=-=-=-=-=-= Definitions, Enums, Typedefs, Consts =-=-=-=-=-=-=-=
\*--------------------------------------------------------------------*/

const char * const k_benaphore_default_name = "Benaphore";


/*--------------------------------------------------------------------*\
  =-=-=-=-=-=-=-=-=-=-=-=-= Structs, Classes =-=-=-=-=-=-=-=-=-=-=-=-=
\*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*\
  Class name:       Benaphore
  Inherits from:    none
  New data members: private sem_id m_sem_id - the semaphore used in
                        the Benaphore.
                    private bool m_valid - true if the Benaphore is
                        valid, false otherwise.
                    private int32 m_ben_cnt - the count of entities
                        who have aquired or are waiting to aquire the
                        Benaphore.
  Description: Class to implement a signal-safe "Benaphore" variant
      semaphore for critical section mutex-wrapping. Benaphores are
      described in issue #26 of the Be Newsletter <http://www.be.com/
      aboutbe/benewsletter/Issue26.html> and the BeOS Kernel Kit API
      <http://www.be.com/documentation/be_book/kernel/sems.html
      #Benaphores>.
      
      The implementation of a Benaphore described in both the Be
      Newsletter and the BeOS Kernel Kit API is not signal-safe; if a
      thread waiting on acquiring a Benaphore received a signal, the
      Benaphore would fail, and may enter a state in which it became
      invalid. This implementation corrects that problem by catching
      the value returned by acquire_sem() and trying again if that
      value is B_INTERRUPTED. As a result of this change, if a thread
      receives a signal while waiting for a Benaphore, it may be moved
      to the end of the line to acquire it. If you expect a lot of
      traffic on the Benaphore, you might wish to consider using
      semaphores instead.
      
      To use this class, simply instatiate it (usually as a static
      class member or a global variable), and make sure it is valid:
      
        Benaphore g_my_benaphore;
        
        if (!g_my_benaphore.IsValid())
          cleanupAndExit();
      
      Then, when a thread wishes to enter a critical section, all it
      has to do is call Lock() and Unlock():
      
        if (g_my_benaphore.Lock() == B_NO_ERROR)
        {
          // Critical code goes here
          
          g_my_benaphore.Unlock();
        }
      
      Note that once one call to Lock() fails, all subsequent calls to
      Lock() (on the same Benaphore) will fail and IsValid() will
      return false. This is to avoid continuing use of a Benaphore
      which may have been corrupted. To avoid this, never call Lock()
      on a Benaphore to which you do not have permissions (i.e.,
      Benaphores should not be shared between teams); the only reason
      why Lock() should fail on a valid Benaphore is if a calling
      thread did not have permissions to acquire the semaphore.
\*--------------------------------------------------------------------*/

class Benaphore
{
	public:
	
		// Public member functions
		
/*--------------------------------------------------------------------*\
  Function name: Benaphore
  Member of:     public Benaphore
  Defined in:    Benaphore.cpp
  Arguments:     const char * const a_name - the name given to the
                     semaphore associated with the Benaphore. Default:
                     k_benaphore_default_name.
  Returns:       n/a
  Description: Default class constructor.
\*--------------------------------------------------------------------*/
		
		Benaphore(const char * const a_name = k_benaphore_default_name);
		
/*--------------------------------------------------------------------*\
  Function name: ~Benaphore
  Member of:     public Benaphore
  Defined in:    Benaphore.cpp
  Arguments:     none
  Returns:       n/a
  Description: Class destructor.
\*--------------------------------------------------------------------*/
		
		~Benaphore(void);
		
/*--------------------------------------------------------------------*\
  Function name: inline IsValid const
  Member of:     public Benaphore
  Arguments:     none
  Returns:       bool
  Description: Function to check if the Benaphore is valid. This
      function must be called after creating the Benaphore to insure
      that its instantiation was successful.
\*--------------------------------------------------------------------*/
		
		inline bool IsValid(void) const
		{
			return m_valid;
		};
		
/*--------------------------------------------------------------------*\
  Function name: Lock
  Member of:     public Benaphore
  Defined in:    Benaphore.cpp
  Arguments:     none
  Returns:       status_t - returns B_NO_ERROR on success, an error on
                     failure.
  Description: Function to aquire the Benaphore. If the Benaphore is
      valid (verified by the IsValid() member function), then this
      function will block until the Benaphore is locked, upon which
      time the function will return B_NO_ERROR. If the Benaphore is
      not valid, the function will return a value less than
      B_NO_ERROR. A successful call to Lock() should always be
      mirrored by exactly on call to Unlock().
\*--------------------------------------------------------------------*/
		
		status_t Lock(void);
		
/*--------------------------------------------------------------------*\
  Function name: Grab
  Member of:     public Benaphore
  Defined in:    Benaphore.cpp
  Arguments:     none
  Returns:       status_t - returns B_NO_ERROR on success, an error on
                     failure.
  Description: Function to aquire the Benaphore. If the Benaphore is
      valid (verified by the IsValid() member function), then this
      function will attempt to obtain the Benaphore without locking.
      If the Benaphore is not valid, the function will return a value
      less than B_NO_ERROR.
      If the benaphore is in use, the function will return 1
      If the benaphore is obtained, the function will return 0
      A successful call to Grab() should always be mirrored by exactly
      one call to Unlock().
  Added: Tim Vernum 1999, placed in Public Domain
\*--------------------------------------------------------------------*/
		
		status_t Grab(void);

/*--------------------------------------------------------------------*\
  Function name: inline Unlock
  Member of:     public Benaphore
  Defined in:    Benaphore.cpp
  Arguments:     none
  Returns:       none
  Description: Function to release the Benaphore. There should be
      exactly one call to Unlock() for every successful call to
      Lock().
\*--------------------------------------------------------------------*/
		
		void Unlock(void);
	
	private:
	
		// Private data members
		sem_id m_sem_id;
		bool m_valid;
		int32 m_ben_cnt;
};

#endif    // BENAPHORE_H
