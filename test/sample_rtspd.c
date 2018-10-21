#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include "../include/rtspd_api.h"


int main(int argc, char **argv)
{
	//CHAR version[32];
	//signal(SIGINT, (VOID*)sig_exit);

	//getrtspd_version(version);
	//printf("rtspd version is %s\n",version);
		
	rtspd_init();
	if (argc > 1)
		rtsp_init(argv[1], 8554);
	else
		rtsp_init("127.0.0.1", 8554);
	if (pthread_create(&rtspd_vthread, NULL, vd_rtspd_func,NULL) < 0){			
		printf("pthread_create rtcp error:\n");
		return -1;
	}
	pause();
	printf("rtspd start release resources\n");
	rtsp_freeall();
	rtsp_free();
	rtspd_free();

	return 0;
}

