/************
 *
 * Filename:    dalh_fwtimer.h
 * Purpose:     declarations for fwtimer functions and structures
 * Copyright:   (c) Netgear Inc.
 * 	       2019 All rights reserved
 * Author:      @ VVDN TECHNOLOGIES
 *
 ************/
#include <pthread.h>

#define EPOCHTIME_DAYTIME   86400
#define TIMER_INTERVAL  3600
#define WEEKLYDAY  6

#define SECONDS 60
#define MAX_MINUTE_LENGTH 59
#define MAX_HOUR_LENGTH   23
#define REFRESH_WINDOW 10800 // 3600+3600+3600

//global variable 
pthread_t wrtimer_thread; 
//functions
void wrtimerhandler(void );
