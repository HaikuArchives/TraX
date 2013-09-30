/*--------------------------------------------------------------------*\
  File:      Benaphore.cpp
  Creator:   Matt Bogosian <mattb@columbia.edu>
  Copyright: (c)1997, Matt Bogosian. All rights reserved.
  Description: Source file containing the member functions for the
      Benaphore class.
  ID:        $Id: Benaphore.cpp,v 1.2 1999/06/21 18:38:09 zod Exp zod $
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

#include "Benaphore.h"


/*--------------------------------------------------------------------*\
  =-=-=-=-=-=-=-=-=-=-=-= Function Definitions =-=-=-=-=-=-=-=-=-=-=-=
\*--------------------------------------------------------------------*/

/*====================================================================*/
Benaphore::Benaphore(const char * const a_name) :
/*====================================================================*/
	m_sem_id(create_sem(0, a_name)),
	m_valid(m_sem_id >= B_NO_ERROR),
	m_ben_cnt(0)
{
	;
}

/*====================================================================*/
Benaphore::~Benaphore(void)
/*====================================================================*/
{
	delete_sem(m_sem_id);
}

/*====================================================================*/
status_t Benaphore::Lock(void)
/*====================================================================*/
{
	// Check to see if the Benaphore is valid
	if (!m_valid)
	{
		if (m_sem_id < B_NO_ERROR)
			return m_sem_id;
		
		return B_ERROR;
	}
	
	status_t err = B_NO_ERROR;
	
	// Increment the benaphore count and aquire the semaphore if
	// necessary. Note: atomic_add() returns the previous value of
	// m_ben_cnt.
	if (atomic_add(&m_ben_cnt, 1) > 0)
		while ((err = acquire_sem(m_sem_id)) == B_INTERRUPTED)
			;
	
	// Invalidate on failure. This is only to disallow the Benaphore
	// from being used by anyone not caught in the crossfire. At this
	// point there is still a risk of being in a bizarre state.
	if (err != B_NO_ERROR)
	{
		atomic_add(&m_ben_cnt, -1);
		m_valid = false;
	}
	
	return err;
}

/*====================================================================*/
status_t Benaphore::Grab(void)
//  Added: Tim Vernum 1999, placed in Public Domain
/*====================================================================*/
{
	// Check to see if the Benaphore is valid
	if (!m_valid)
	{
		if (m_sem_id < B_NO_ERROR)
			return m_sem_id;
		
		return B_ERROR;
	}
	
	status_t err = B_NO_ERROR;
	
	// Increment the benaphore count and aquire the semaphore if
	// necessary. Note: atomic_add() returns the previous value of
	// m_ben_cnt.
	if (atomic_add(&m_ben_cnt, 1) > 0)
	{
		do
		{
			err = acquire_sem_etc(m_sem_id, 1, B_TIMEOUT, 0 ) ;
		} while (err == B_INTERRUPTED) ;
	}

	// Invalidate on failure. This is only to disallow the Benaphore
	// from being used by anyone not caught in the crossfire. At this
	// point there is still a risk of being in a bizarre state.
	if (err != B_NO_ERROR)
	{
		atomic_add(&m_ben_cnt, -1);

		if( err == B_WOULD_BLOCK )
		{
			return 1 ;
		}
		else
		{
			m_valid = false ;
			return err ;
		}
	}
	
	return 0 ;
}

/*====================================================================*/
void Benaphore::Unlock(void)
/*====================================================================*/
{
	// Decrement the benaphore count and release the semaphore if
	// necessary. Note: atomic_add() returns the previous value of
	// m_ben_cnt.
	if (atomic_add(&m_ben_cnt, -1) > 1)
		release_sem(m_sem_id);
}
