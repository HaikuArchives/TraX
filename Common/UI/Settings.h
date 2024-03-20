// Stubbed out Settings.h for Tim Vernum's TraX.

#ifndef SETTINGS_H
#define SETTINGS_H


typedef struct settings {
	enum {
		eShowBrackets,
		eShowJoinPanel,
		eShowNot
	};

	int			fConjunction;
	const char* fDefaultFolder;
	bool		fUseTracker;
	int			fShow;
} TraxSettings;


extern TraxSettings gSettings;

#endif
