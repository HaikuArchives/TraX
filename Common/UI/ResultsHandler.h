// Stubbed out ResultsHandler.{h,cpp} for Tim Vernum's TraX.

#ifndef RESULTSHANDLER_H
#define RESULTSHANDLER_H


class ResultsHandler {
public:
					ResultsHandler();

			void	Init();

			void	NewSearch();
			void	EndSearch();

	static	int		AddResult(const char*);
};


#endif
