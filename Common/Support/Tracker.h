#ifndef TRACKER_H
#define TRACKER_H

/*******************************************************
 * (c) 1999, Tim Vernum <zodsoft@kagi.com>
 * All rights reserved
 *******************************************************
 * File   : Tracker.h
 * Author : Tim Vernum
 * Project: Trax Application
 * Purpose: Functions for interfacing with Tracker
 * Licence: AUL
 *******************************************************/

struct entry_ref ;
class BMessenger ;

/* Get a messenger for the Tracker window displaying
 *  'ref', where 'ref' is a directory.
 * Returns the window in as a messenger 'msgr'.
 * Returns 'true' if a window could be found, 'false' otherwise.
 */
bool GetTrackerWindow( const entry_ref * ref , BMessenger * msgr ) ;

#endif
