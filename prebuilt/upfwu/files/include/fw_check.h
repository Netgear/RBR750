/************
 *
 * Filename:	fw_check.h
 *
 * Purpose:	API calls of fw_check library
 *
 * Copyright:	(c) Netgear Inc.
 *		2017 All rights reserved
 *
 * Author:	@ VVDN TECHNOLOGIES
 *
 ************/

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
  * Supported Error Types
  */

enum fw_check_error {

  ERROR_OK,
  ERROR_DAL,
  ERROR_INIT,
  ERROR_TIMEOUT,
  ERROR_CONNECT,
  ERROR_INVALID_INPUT,
  ERROR_INTERNAL,
  ERROR_NO_URL,
  ERROR_UNKNOWN,
};

/*
 * If one wishes to redirect logging, they can by overwriting debug ptr
 * e.g.
 *      debug = myCustomLogFunction;
 */
enum logLev { LIB_ERROR, LIB_DEBUG, LIB_INFO };


void (*fw_debug)(enum logLev ll, const char *fmt, ...);

/**
  * Supported reasons for library call
  */
typedef enum call_reason {

  CHECK_UNSPECIFIED,
  CHECK_AUTO,
  CHECK_ON_DEMAND_UPAPP,
  CHECK_ON_DEMAND_GUI,
} reason_type;

/*
 * Library Function declarations below
 */

const char *error_message(enum fw_check_error);

/**
  * Init Check_new_fw library
  *
  * @return enum fw_check_error type
  **/

enum fw_check_error fw_check_init(void);

/**
  * UnInit Check_new_fw library
  *
  * @return enum fw_check_error type
  **/

enum fw_check_error fw_check_uninit(void);

/**
  * Function that should be called by the caller in case of
  * new Firmware check.
  *
  * @param: enum call_reason type mode, boolean type beta_acceptance, char
  *pointer to be filled with URL, url length
  *
  * @return fw_check_error type
  **/

enum fw_check_error get_check_fw(const enum call_reason mode,
                                 bool beta_acceptance, char *url,
                                 int url_length);

/**
  * Function that should be called by the caller to notify
  * after the upgrade procedure is attempted
  *
  * @param: none	// Only CHECK_BOOT applicable
  *
  * @return fw_check_error type
  **/

enum fw_check_error boot_notify();

#ifdef __cplusplus
};
#endif
