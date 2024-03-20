#ifndef _BARBERPOLE_H_
#define _BARBERPOLE_H_

#include <View.h>

const int FROM_RIGHT_TO_LEFT = 0;
const int FROM_LEFT_TO_RIGHT = 1;

class BarberPole : public BView {
public:
	BarberPole(BRect pRect, const char *pName, uint32 resizingMode, uint32 flags = 0, int pDirection = FROM_LEFT_TO_RIGHT);
	~BarberPole();

	void	Start();
	void	Stop();

	bool	IsRunning();

protected:
	thread_id	spinning_thread_id;
	bool		is_running;
	
private:
			int		direction;
	static 	int32 	spinningThread(void *data);
};

#endif