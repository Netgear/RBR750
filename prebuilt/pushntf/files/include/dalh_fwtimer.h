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



#define EPOCHTIME_DAY   86400
#define EPOCHTIME_HOUR  3600
#define INITIAL_TIMER   1
#define TRUNCATED_TIMER 2
#define COUNTDOWN_DEFAULT 255

/* Defines for the return values */
#define ERROR_FWUPGRADE_TIMER -17

//global variables
pthread_t fwtimer_thread;

//functions
void fwtimerhandler(void );
void* check_and_trigger_timer(void *dummy);
