#ifndef MESSAGES_H
#define MESSAGES_H

/*
 *   Namespace: Messages
 *   Copyright: Tim Vernum 1999, All rights reserved
 *     Licence: GPL
 */

#include "WinMsg.h"

namespace Messages
{
	const uint32 StartFind = 'Find' ;
	const uint32 LibInUse = 'LIUt' ; 
	const uint32 LibNotInUse = 'LIUf' ;
	
	const uint32 SetTab = 'Tab\0' ;
	
	const uint32 SettingsRequested = 'SetR' ;
	const uint32 SettingsChanged   = 'SetC' ;
	const uint32 Broadcast = 'BrdC' ;
}

#endif