/************
 *
 * Filename:	dalh_internal.h
 * Purpose:     function declarations used in dalh process
 * Copyright:   (c) Netgear Inc.
 *              2019 All rights reserved
 * Author:      @ VVDN TECHNOLOGIES
 *
 ************/

/* Define to prevent recursive inclusion---------------------*/

#ifndef _INTERNAL_H_
#define _INTERNAL_H_

#include <dalh_def.h>
#include <stdbool.h>

struct context {
  char loglevel[BUFFER_SIZE_16];
};

int get_featureId();
int get_processId();
int get_ndnIndex();
int get_taggedDeviceMac();
int get_AllowDeviceMac();
int get_BlockDeviceMac();
int get_taggedStatus(int);
int dalh_subscribe(void);
int get_bstStatusUP();
int get_allowStatus();
int get_blockStatus();
//int get_deviceMAC_info(void); //will be used further
int dalh_dal_init(void);
void dalh_dal_destroy(void);
int dalh_cb_on_change(const char *tbl, const char *fld, void (*foo)(void *),
                          void *up);
void *dalh_dal_get_handle(void);
void good_bye(char *fd);
void print_err(enum check_d2_error err);
const char *error_message(enum check_d2_error err);
void show_d2_err(enum d2_error err);

#endif /* _INTERNAL_H_ */
