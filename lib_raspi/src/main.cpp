#include <iostream>
#include <cstring>
#include <unistd.h>
#include "main.h"
using namespace std;

//g++ main.cpp main.h -pthread -o play

pthread_t Audio::thread_command;
pthread_t Audio::thread_state;
int Audio::state;
pthread_mutex_t Audio::mutex_state;
pthread_cond_t Audio::signal_cmd = PTHREAD_COND_INITIALIZER;
pthread_mutex_t Audio::lock_audio = PTHREAD_MUTEX_INITIALIZER;



Audio::Audio() {
	this->state = AUDIO_STATE_IDLE;
}



void Audio::setState(int st) {
	pthread_mutex_lock(&mutex_state);
	state = st;
	pthread_cond_signal(&signal_cmd);
	pthread_mutex_unlock(&mutex_state);
}

int Audio::getState() {
	return state;
}


void Audio::showInfo() {
	cout << "============== Audio Info ===========" << endl;
	cout << "State: " << state << endl;
	cout << "=======================================" << endl;
}


void* Audio::command(void* obj)
{
	int ct = -1; // 1 2
    while (true) {
    	cout << "input: ";
    	scanf("%d", &ct);
    	int cur_state = Audio::getState();

		if (ct == AUDIO_STATE_PLAY && cur_state == AUDIO_STATE_PLAY)
		{
			cout << "audio playing cann't play\n";
		}
		else if (ct == AUDIO_STATE_PLAY && cur_state == AUDIO_STATE_STOP)
		{
			cout << "audio start ok\n";
			Audio::setState(AUDIO_STATE_PLAY);
		}
		else if (ct == AUDIO_STATE_STOP && cur_state == AUDIO_STATE_PLAY)
		{
			cout << "audio stop ok\n";
			Audio::setState(AUDIO_STATE_STOP);
		}
		else if (ct == AUDIO_STATE_STOP && cur_state == AUDIO_STATE_STOP)
		{
			cout << "already stop \n";
		} 
		else if (ct == AUDIO_STATE_PLAY && cur_state == AUDIO_STATE_IDLE)
		{
			cout << "audio start ok\n";
			Audio::setState(AUDIO_STATE_PLAY);
		} 
		else if (ct == AUDIO_STATE_STOP && cur_state == AUDIO_STATE_IDLE)
		{
			cout << "already stop \n";
		} 
		else {
			cout << "Not support yet: "<< " " <<cur_state << endl;
		}
    	sleep(1);
    }

    return NULL;
}


void* Audio::state_audio(void* obj){
	bool is_runing = true;
	while(is_runing){
		pthread_mutex_lock(&lock_audio);
		
		pthread_cond_wait(&signal_cmd, &lock_audio);
		int cur_state = Audio::getState();
		pthread_mutex_unlock(&lock_audio);
		
		switch(cur_state){
			case AUDIO_STATE_IDLE:
				cout << "AUDIO_STATE_IDLE\n";

				break;
			case AUDIO_STATE_STOP:
				cout << "AUDIO_STATE_STOP\n";
				system("sudo killall mpg123");
				break;
			case AUDIO_STATE_PLAY:
				cout << "AUDIO_STATE_PLAY\n";
				system("../bin/./mpg123 TruyenThaiYMasewRemix-NgoKienHuyMasew-6064089.mp3 >/dev/null 2>&1");
				break;
		}
		//sleep(1);
	}
}



bool Audio::InitCommand()
{
    if (pthread_create(&thread_command, NULL, Audio::command, NULL)) {
        return false;
    }
    
    return true;
}

bool Audio::InitState()
{
    if (pthread_create(&thread_state, NULL, Audio::state_audio, NULL)) {
        return false;
    }
    
    return true;
}
bool Audio::pthread_join_command(){
    if (pthread_join(thread_command,NULL)) {
    	
        return false;
    }
    
    return true;	
}
bool Audio::pthread_join_state(){
    if (pthread_join(thread_state,NULL)) {
    	
        return false;
    }
    
    return true;	
}

int main() {

	
	Audio s; 
	s.InitCommand();
	s.InitState();
	s.pthread_join_command();
	s.pthread_join_state();
	return 0;
}