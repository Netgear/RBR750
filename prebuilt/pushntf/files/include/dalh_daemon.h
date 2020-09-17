/************
 *
 * Filename:    dalh_daemon.h
 * Purpose:     locking and unlocking of the dalh process
 * Copyright:   (c) Netgear Inc.
 * 				2019 All rights reserved
 * Author:      @ VVDN TECHNOLOGIES
 *
 ************/

/* Define to prevent recursive inclusion---------------------*/

#ifndef _DALH_DAEMON_H_
#define _DALH_DAEMON_H_

/*
 * @brief :	lock_process
 * @param :	[in] pid_file
 * @return :	'-1' if failed to lock process with pid file creation, lock file
 * 				descriptor on success
 */
int lock_process(const char *const lock_file);

/*
 * @brief :	unlock_process
 * @param :	[in] pid_file
 * @return :	'-1' if failed to unlock process with pid file creation, '0' on
 * 				success
 */
int unlock_process(const char *const lock_file, int lfd);

#endif //_DALH_DAEMON_H_
