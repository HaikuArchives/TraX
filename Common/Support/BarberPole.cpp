#include "BarberPole.h"


BarberPole::BarberPole(BRect pRect, const char *pName, uint32 resizingMode, uint32 flags, int pDirection)
		: BView(pRect, pName, resizingMode, flags) {
		
	spinning_thread_id = spawn_thread(spinningThread, pName, B_DISPLAY_PRIORITY, this);
	is_running = false;
	direction = pDirection;
};

BarberPole::~BarberPole(){
	kill_thread(spinning_thread_id);
}

void BarberPole::Start(){
	resume_thread(spinning_thread_id); 
	is_running = true;
}

void BarberPole::Stop(){
	suspend_thread(spinning_thread_id); 
	is_running = false;
}

bool BarberPole::IsRunning(){
	return is_running;
}

int32 BarberPole::spinningThread(void *data){
	BarberPole	*lBarberPole = (BarberPole*)data;
	pattern		lStripes;

	lStripes.data[0] = 0x0f;
	lStripes.data[1] = 0x1e;
	lStripes.data[2] = 0x3c;
	lStripes.data[3] = 0x78;
	lStripes.data[4] = 0xf0;
	lStripes.data[5] = 0xe1;
	lStripes.data[6] = 0xc3;
	lStripes.data[7] = 0x87;	
	
	while(1==1){
		lBarberPole->LockLooper();
		lBarberPole->FillRect(lBarberPole->Bounds(), lStripes);
		lBarberPole->UnlockLooper();

		if(lBarberPole->direction == FROM_RIGHT_TO_LEFT){
			uchar tmp = lStripes.data[0];
			for (int j = 0; j < 7; ++j) {
	  			lStripes.data[j] = lStripes.data[j+1];
			}
			lStripes.data[7] = tmp;
		} else {
			uchar tmp = lStripes.data[7];
			for (int j = 7; j > 0; --j) {
	  			lStripes.data[j] = lStripes.data[j-1];
			}
			lStripes.data[0] = tmp;
		
		}
		snooze(25000);
	}
};