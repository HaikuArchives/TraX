#ifndef MESSAGES_H
#define MESSAGES_H

/*
 *              Messages.h
 *   Copyright: Tim Vernum 1999, All rights reserved
 *     Licence: GPL
 */

#include "WinMsg.h"

namespace Messages
{
	const uint32 StartFind = 'Find' ;
	const uint32 AddPanel = 'AddP' ;
	const uint32 RemovePanel = 'RmvP' ;
	const uint32 SetPosition = 'SPos' ;
	const uint32 LibInUse = 'LIUt' ; 
	const uint32 LibNotInUse = 'LIUf' ;
	const uint32 Settings = 'Sett' ;
	const uint32 SettingsChanged = 'StCh' ;
	const uint32 Broadcast = 'BrdC' ;
	const uint32 NewWindow = 'NewW' ;
}

#endif