#include <iostream>
#include <pthread.h>

#define AUDIO_STATE_IDLE 0 
#define AUDIO_STATE_PLAY 1 
#define AUDIO_STATE_STOP 2 


class Audio {
private:
	static int state;
    static pthread_mutex_t     mutex_state;
    static pthread_t thread_command;
    static pthread_t thread_state;
    static pthread_cond_t signal_cmd;
    static pthread_mutex_t lock_audio;
public:
	Audio();
	void showInfo();
	static void setState(int);
	static void setControl(int);
	static int getState();
	static int getControl();
	static bool InitCommand();
	static bool InitState();
	static bool pthread_join_state();
	static bool pthread_join_command();
    static void* command(void* obj);
    static void* state_audio(void* obj);
    
};