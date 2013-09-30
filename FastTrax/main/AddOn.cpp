#include <add-ons/tracker/TrackerAddOn.h>
#include <Roster.h>
   
/*
 *       File: AddOn.cpp
 *    Licence: Public Domain
 */

void process_refs( entry_ref dir_ref, BMessage *msg, void * ) 
{
	be_roster->Launch( "application/x-vnd.ZOD-FastTraX", msg ) ;
}
