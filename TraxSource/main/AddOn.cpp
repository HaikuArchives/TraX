#include <add-ons/tracker/TrackerAddOn.h>
#include <Roster.h>
   
/*
 *        FILE: AddOn.cpp
 *     Licence: Public domain
 */


void process_refs( entry_ref dir_ref, BMessage *msg, void * ) 
{
	be_roster->Launch( "application/x-vnd.ZOD-TraX", msg ) ;
}
