/************
 *
 * Filename:    dalh_args.h
 * Purpose:     parsing command line arguments
 * Copyright:   (c) Netgear Inc.
 * 				2019 All rights reserved
 * Author:      @ VVDN TECHNOLOGIES
 *
 ************/

/* Define to prevent recursive inclusion---------------------*/

#ifndef _DALH_ARGS_H_
#define _DALH_ARGS_H_

/*
 * @brief :	parse_args
 *			Parses dalh invokation string
 * @param1 :	argc - num of parameters
 * @param2 :	argv - array of parameters string
 */
int parse_args(int argc, char *argv[]);

#endif //_DALH_ARGS_H_
