/*	$KAME: dhcp6c.c,v 1.164 2006/01/10 02:46:09 jinmei Exp $	*/
/*
 * Copyright (C) 1998 and 1999 WIDE Project.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the project nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE PROJECT AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <sys/queue.h>
#include <errno.h>
#include <limits.h>
#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif
#include <net/if.h>
#ifdef __FreeBSD__
//#if defined(__FreeBSD__) && __FreeBSD__ >= 3
#include <net/if_var.h>
#endif

#include <netinet/in.h>
#ifdef __KAME__
#include <net/if_dl.h>
#include <netinet6/in6_var.h>
#endif

#include <arpa/inet.h>
#include <netdb.h>

#include <signal.h>
#include <stdio.h>
#include <stdarg.h>
#include <syslog.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <err.h>
#include <ifaddrs.h>

#include "dhcp6.h"
#include "config.h"
#include "common.h"
#include "timer.h"
#include "dhcp6c.h"
#include "control.h"
#include "dhcp6_ctl.h"
#include "dhcp6c_ia.h"
#include "prefixconf.h"
#include "auth.h"
/* Foxconn Harry remove start 2018/10/15, redefined */
//#include "in6_pktinfo.h"    /* for R8000P compatibility */
/* Foxconn Harry remove end 2018/10/15*/

/* Foxconn Bernie added start, 2014/11/27 */
int recv_type;
int get_napd_fail_to_resolicit = 0;
int get_na_fail = 0;
int get_pd_fail = 0;
long retrans_time = 1000;
extern struct iana_information ianaInfo;
extern struct iapd_information iapdInfo;
extern int iana_option;
char oldprefix[50]={""};
int noaddr_haspd = 0; /* Foxconn Bernie added, 2015/01/16 */
/* Foxconn Bernie added end, 2014/11/27 */
static int debug = 0;
static int exit_ok = 0;
volatile sig_atomic_t sig_flags = 0;
#define SIGF_TERM 0x1
#define SIGF_HUP 0x2
#if DHCP6_DEBUG
#define dprintf2(a, b, fmt, arg...)       do { printf("%s [%d]: "fmt, __FILE__, __LINE__, ##arg); printf("\n"); } while (0)
#endif
#define ddprintf2(a, b, fmt, arg...)       do { printf("%s [%d]: "fmt, __FILE__, __LINE__, ##arg); printf("\n"); } while (0)

const dhcp6_mode_t dhcp6_mode = DHCP6_MODE_CLIENT;

int sock;	/* inbound/outbound udp port */
int rtsock;	/* routing socket */
int ctlsock = -1;		/* control TCP port */
char *ctladdr = DEFAULT_CLIENT_CONTROL_ADDR;
char *ctlport = DEFAULT_CLIENT_CONTROL_PORT;

#define DEFAULT_KEYFILE SYSCONFDIR "/dhcp6cctlkey"
#define CTLSKEW 300

long sol_max_rt = SOL_MAX_RT; /*Foxconn Bernie added , 2015/9/14@  used in receive SOL_MAX_RT from DHCPv6 Server*/
static char *conffile = DHCP6C_CONF;

static const struct sockaddr_in6 *sa6_allagent;
static struct duid client_duid;
static struct duid server_duid; /* Foxconn Bernie added, 2014/11/27 */
static char *pid_file = DHCP6C_PIDFILE;

static char *ctlkeyfile = DEFAULT_KEYFILE;
static struct keyinfo *ctlkey = NULL;
static int ctldigestlen;

static int infreq_mode = 0;

static inline int get_val32 __P((char **, int *, u_int32_t *));
static inline int get_ifname __P((char **, int *, char *, int));

static void usage __P((void));
static void client6_init __P((char*)); /* Foxconn Bernie modified start, 2014/11/27 @ for get duid*/
static void client6_startall __P((int));
static void free_resources __P((struct dhcp6_if *));
static iatype_t check_iatype __P((struct dhcp6_if *));/* Foxconn Bernie added, 2014/11/27 */
static void client6_mainloop __P((void));
static int client6_do_ctlcommand __P((char *, ssize_t));
static void client6_reload __P((void));
static int client6_ifctl __P((char *ifname, u_int16_t));
static void check_exit __P((void));
static void process_signals __P((void));
static struct dhcp6_serverinfo *find_server __P((struct dhcp6_event *,
						 struct duid *));
static struct dhcp6_serverinfo *select_server __P((struct dhcp6_event *));
static void client6_recv __P((void));
static int client6_recvadvert __P((struct dhcp6_if *, struct dhcp6 *,
				   ssize_t, struct dhcp6_optinfo *));
static int client6_recvreply __P((struct dhcp6_if *, struct dhcp6 *,
				  ssize_t, struct dhcp6_optinfo *));
static void client6_signal __P((int));
static struct dhcp6_event *find_event_withid __P((struct dhcp6_if *,
						  u_int32_t));
static int construct_confdata __P((struct dhcp6_if *, struct dhcp6_event *));
static int construct_reqdata __P((struct dhcp6_if *, struct dhcp6_optinfo *,
    struct dhcp6_event *));
static void destruct_iadata __P((struct dhcp6_eventdata *));
static void tv_sub __P((struct timeval *, struct timeval *, struct timeval *));
static struct dhcp6_timer *client6_expire_refreshtime __P((void *));
static int process_auth __P((struct authparam *, struct dhcp6 *dh6, ssize_t,
    struct dhcp6_optinfo *));
static int set_auth __P((struct dhcp6_event *, struct dhcp6_optinfo *));
static int set_vendorclass __P((struct dhcp6_optinfo *));
static int set_vendor __P((struct dhcp6_optinfo *));

struct dhcp6_timer *client6_timo __P((void *));
int client6_start __P((struct dhcp6_if *));
static void info_printf __P((const char *, ...));

extern int client6_script __P((char *, int, struct dhcp6_optinfo *));

#define MAX_ELAPSED_TIME 0xffff

/* Foxconn Bernie added start, 2014/11/27 */
#define RECONFIG_PHASE_NO	0
#define RECONFIG_PHASE_YES	1
#define DHCP_PD	"/tmp/dhcp_pd"
int reconfig_phase = RECONFIG_PHASE_NO;
static struct dhcp6_optinfo reconfig_optinfo;
static struct keyinfo *reconfig_key = NULL;

static void free_reconfig_resource()
{
	/* send lease package whih not reconfig accept option */
	if (reconfig_phase == RECONFIG_PHASE_YES)
		reconfig_phase = RECONFIG_PHASE_NO;

	/* clear reconfig optinfo */
	dhcp6_clear_options(&reconfig_optinfo);

	/* free the rconfig key */
	if (reconfig_key != NULL){
		if (reconfig_key->secret != NULL)
			free(reconfig_key->secret);
		free(reconfig_key);
	}

	return;
}

static int
note_reconfig_option(optinfo)
	struct dhcp6_optinfo *optinfo;
{
	if (optinfo == NULL) {
		printf("optinfo is NULL \n");
		return (-1);
	}

	dhcp6_clear_options(&reconfig_optinfo);
	/* copy options */
	dhcp6_init_options(&reconfig_optinfo);
	if (dhcp6_copy_options(&reconfig_optinfo, optinfo)) {
		dprintf2(LOG_ERR, FNAME, "failed to copy options");
		return (-1);
	}

	/*reset the reconfig phase flag*/
	reconfig_phase = RECONFIG_PHASE_NO;

	return 0;
}

extern char dhcp6_interface[16];

static int
create_reconfig_event(ifp, dh6, len, optinfo)
	struct dhcp6_if *ifp;
	struct dhcp6 *dh6;
	ssize_t len;
	struct dhcp6_optinfo *optinfo;
{
	int dhcpstate;
	struct authparam *authparam;
	struct dhcp6_event *ev;

	switch (*(optinfo->recmsg_msg)) {
		case DH6_RENEW:
			dhcpstate = DHCP6S_RENEW;
			break;
		case DH6_INFORM_REQ:
			dhcpstate = DHCP6S_INFOREQ;
			break;
		default:
			dprintf2(LOG_NOTICE, FNAME, "the recmsg type is wrong");
			return -1;
	}

	if ((ev = dhcp6_create_event(ifp, dhcpstate)) == NULL) {
		dprintf2(LOG_NOTICE, FNAME, "failed to create a new event");
		goto fail;
	}
	ev->iatype = dhcpstate;
	TAILQ_INSERT_TAIL(&ifp->event_list, ev, link);

	/* add timer */
	if ((ev->timer = dhcp6_add_timer(client6_timo, ev)) == NULL) {
		dprintf2(LOG_NOTICE, FNAME, "failed to create a new event timer");
		goto fail;
	}

	if (dhcpstate != DHCP6S_INFOREQ){
		if (construct_reqdata(ifp, &reconfig_optinfo, ev)) {
			dprintf2(LOG_NOTICE, FNAME, "failed to construct request data");
			goto fail;
		}
		if (duidcpy(&ev->serverid,&optinfo->serverID)) {
			dprintf2(LOG_NOTICE, FNAME, "can't copy server ID");
			return (-1); /* XXX: better recovery? */
		}
	}

	/* init event auth */
	if ((authparam = malloc(sizeof(struct authparam))) == NULL) {
		dprintf2(LOG_NOTICE, FNAME, "malloc authparam fail\n");
	}

	authparam->authproto = DHCP6_AUTHPROTO_RECONFIG;
	authparam->authalgorithm = DHCP6_AUTHALG_HMACMD5;
	authparam->authrdm = DHCP6_AUTHRDM_MONOCOUNTER;
	ev->authparam = authparam;

	ev->timeouts = 0;

	/* set timer */
	dhcp6_set_timeoparam(ev);
	dhcp6_reset_timer(ev);
	client6_send(ev);
	return 0;
fail:
	if (ev)
		dhcp6_remove_event(ev);
	return -1;
}

static int
client6_recvreconfig(ifp, pi, dh6, len, optinfo)
	struct dhcp6_if *ifp;
	struct in6_pktinfo *pi;
	struct dhcp6 *dh6;
	ssize_t len;
	struct dhcp6_optinfo *optinfo;
{
	int recmsg_type;

	/* packet validation based on Section 19.4.1 of RFC3315. */
	if (reconfig_phase == RECONFIG_PHASE_YES) {
		dprintf2(LOG_INFO, FNAME,"the client has received a Reconfigure");
		return -1;
	}
	reconfig_phase = RECONFIG_PHASE_YES;
	/* packet validation based on Section 15.11 of RFC3315. */
	if (IN6_IS_ADDR_MULTICAST(&pi->ipi6_addr)){
		dprintf2(LOG_INFO, FNAME, "the message was not unicast to the client");
		return (-1);
	}
	if (optinfo->serverID.duid_len == 0) {
		dprintf2(LOG_INFO, FNAME, "no server ID option");
		return (-1);
	} else {
		dprintf2(LOG_DEBUG, FNAME, "server ID: %s, pref=%d",
				duidstr(&optinfo->serverID),optinfo->pref);
	}

	if (optinfo->clientID.duid_len == 0) {
		dprintf2(LOG_INFO, FNAME, "no client ID option");
		return (-1);
	}
	if (duidcmp(&optinfo->clientID, &client_duid)) {
		dprintf2(LOG_INFO, FNAME, "client DUID mismatch");
		return (-1);
	}

	if (optinfo->recmsg_len == 0) {
		dprintf2(LOG_INFO, FNAME,"no reconfigure message option");
		return -1;
	}

	recmsg_type = *(optinfo->recmsg_msg);
	if ((recmsg_type != DH6_RENEW) && (recmsg_type != DH6_INFORM_REQ)) {
		dprintf2(LOG_INFO, FNAME,"the reconfig message type must be a valid value");
		return -1;
	}

	if (optinfo->authproto == DHCP6_AUTHPROTO_UNDEF) {
		dprintf2(LOG_INFO, FNAME,"the message does not contain an authentication option");
		return -1;
	}

	/* validate authentication */
	if (process_auth(NULL, dh6, len, optinfo)) {
		dprintf2(LOG_INFO, FNAME, "failed to process authentication");
		return (-1);
	}

	/* create the event to respond to server for reconfig message*/
	if (create_reconfig_event(ifp, dh6, len, optinfo)) {
		dprintf2(LOG_INFO, FNAME, "create the reconfig message fail");
		return -1;
	}

	return 0;
}
/* Foxconn Bernie added end, 2014/11/27 */

int
main(argc, argv)
	int argc;
	char **argv;
{
	int ch, pid;
	char *progname;
	FILE *pidfp;
	struct dhcp6_if *ifp;

#ifndef HAVE_ARC4RANDOM
	srandom(time(NULL) & getpid());
#endif

	if ((progname = strrchr(*argv, '/')) == NULL)
		progname = *argv;
	else
		progname++;

	while ((ch = getopt(argc, argv, "c:dDfik:p:")) != -1) {
		switch (ch) {
		case 'c':
			conffile = optarg;
			break;
		case 'd':
			debug = 1;
			break;
		case 'D':
			debug = 2;
			break;
		case 'f':
			foreground++;
			break;
		case 'i':
			infreq_mode = 1;
			break;
		case 'k':
			ctlkeyfile = optarg;
			break;
		case 'p':
			pid_file = optarg;
			break;
		default:
			usage();
			exit(0);
		}
	}
	argc -= optind;
	argv += optind;

	if (argc == 0) {
		usage();
		exit(0);
	}

/* Foxconn Bernie modified start, 2014/11/27 */
	//if (foreground == 0)
		//openlog(progname, LOG_NDELAY|LOG_PID, LOG_DAEMON);
	strcpy(dhcp6_interface, argv[1]);

	//printf("\n%s, %s, %s\n", argv[0], argv[1], dhcp6_interface);
	setloglevel(debug);
	client6_init(argv[0]);
	/*The first argv is wan_eth, use for get wan_eth MAC address,
	  because of pppoe virtual interface have no MAC address */
	argv = argv + 1;
	while (argc-- > 1) {
	//while (argc-- > 0) {
		if ((ifp = ifinit(argv[0])) == NULL) {
			dprintf2(LOG_ERR, FNAME, "failed to initialize %s",
			    argv[0]);
			exit(1);
		}
		argv++;
	}
/* Foxconn Bernie modified end, 2014/11/27 */

	if (infreq_mode == 0 && (cfparse(conffile)) != 0) {
		dprintf2(LOG_ERR, FNAME, "failed to parse configuration file");
		exit(1);
	}

	if (foreground == 0 && infreq_mode == 0) {
#if 1
	printf("Cannot background in uClinux (yet)\n");
#else /* __uClinux__ */
		if (daemon(0, 0) < 0)
			err(1, "daemon");
#endif
	}

	/* dump current PID */
	pid = getpid();
	if ((pidfp = fopen(pid_file, "w")) != NULL) {
		fprintf(pidfp, "%d\n", pid);
		fclose(pidfp);
	}
	printf("\n[%s,%d],Use wide-dhcpv6.\n",__FUNCTION__,__LINE__);
/* Foxconn Bernie added start, 2014/11/27 */
	system("nvram set ipv6_sip_servers=\"\"");
	system("nvram set ipv6_ntp_servers=\"\"");
/* Foxconn Bernie added end, 2014/11/27 */

	client6_startall(0);
	client6_mainloop();
	exit(0);
}

static void
usage()
{

	fprintf(stderr, "usage: dhcp6c [-c configfile] [-dDfi] "
	    "[-p pid-file] interface [interfaces...]\n");
}

/*------------------------------------------------------------*/

void
client6_init(char *ifname)
{
	struct addrinfo hints, *res;
	static struct sockaddr_in6 sa6_allagent_storage;
	int error, on = 1;

	/* get our DUID */ //Bernie M
	if (get_duid(DUID_FILE, &client_duid, ifname)) {
		dprintf2(LOG_ERR, FNAME, "failed to get a DUID");
		exit(1);
	}

/* Foxconn Bernie modified start, 2014/11/27 @ for avoid show warning message*/
#if 0
//	if (dhcp6_ctl_authinit(ctlkeyfile, &ctlkey, &ctldigestlen) != 0) {
//		dprintf2(LOG_NOTICE, FNAME,
//		    "failed initialize control message authentication");
//		/* run the server anyway */
//	}
#endif

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = PF_INET6;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;
	hints.ai_flags = AI_PASSIVE;
	error = getaddrinfo(NULL, DH6PORT_DOWNSTREAM, &hints, &res);
	if (error) {
		dprintf2(LOG_ERR, FNAME, "getaddrinfo: %s",
		    gai_strerror(error));
		exit(1);
	}
	sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sock < 0) {
		dprintf2(LOG_ERR, FNAME, "socket");
		exit(1);
	}
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEPORT,
		       &on, sizeof(on)) < 0) {
		dprintf2(LOG_ERR, FNAME,
		    "setsockopt(SO_REUSEPORT): %s", strerror(errno));
		exit(1);
	}
#ifdef IPV6_RECVPKTINFO
	if (setsockopt(sock, IPPROTO_IPV6, IPV6_RECVPKTINFO, &on,
		       sizeof(on)) < 0) {
		dprintf2(LOG_ERR, FNAME,
			"setsockopt(IPV6_RECVPKTINFO): %s",
			strerror(errno));
		exit(1);
	}
#else
	if (setsockopt(sock, IPPROTO_IPV6, IPV6_PKTINFO, &on,
		       sizeof(on)) < 0) {
		dprintf2(LOG_ERR, FNAME,
		    "setsockopt(IPV6_PKTINFO): %s",
		    strerror(errno));
		exit(1);
	}
#endif
	if (setsockopt(sock, IPPROTO_IPV6, IPV6_MULTICAST_LOOP, &on,
		       sizeof(on)) < 0) {
		dprintf2(LOG_ERR, FNAME,
		    "setsockopt(sock, IPV6_MULTICAST_LOOP): %s",
		    strerror(errno));
		exit(1);
	}
#ifdef IPV6_V6ONLY
	if (setsockopt(sock, IPPROTO_IPV6, IPV6_V6ONLY,
	    &on, sizeof(on)) < 0) {
		dprintf2(LOG_ERR, FNAME, "setsockopt(IPV6_V6ONLY): %s",
		    strerror(errno));
		exit(1);
	}
#endif

	/*
	 * According RFC3315 2.2, only the incoming port should be bound to UDP
	 * port 546.  However, to have an interoperability with some servers,
	 * the outgoing port is also bound to the DH6PORT_DOWNSTREAM.
	 */
	if (bind(sock, res->ai_addr, res->ai_addrlen) < 0) {
		dprintf2(LOG_ERR, FNAME, "bind: %s", strerror(errno));
		exit(1);
	}
	freeaddrinfo(res);

	/* open a routing socket to watch the routing table */
	if ((rtsock = socket(PF_ROUTE, SOCK_RAW, 0)) < 0) {
		dprintf2(LOG_ERR, FNAME, "open a routing socket: %s",
		    strerror(errno));
		exit(1);
	}

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = PF_INET6;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;
	error = getaddrinfo(DH6ADDR_ALLAGENT, DH6PORT_UPSTREAM, &hints, &res);
	if (error) {
		dprintf2(LOG_ERR, FNAME, "getaddrinfo: %s",
		    gai_strerror(error));
		exit(1);
	}
	memcpy(&sa6_allagent_storage, res->ai_addr, res->ai_addrlen);
	sa6_allagent = (const struct sockaddr_in6 *)&sa6_allagent_storage;
	freeaddrinfo(res);

	/* set up control socket */
	if (ctlkey == NULL)
		dprintf2(LOG_NOTICE, FNAME, "skip opening control port");
	else if (dhcp6_ctl_init(ctladdr, ctlport,
	    DHCP6CTL_DEF_COMMANDQUEUELEN, &ctlsock)) {
		dprintf2(LOG_ERR, FNAME,
		    "failed to initialize control channel");
		exit(1);
	}

	if (signal(SIGHUP, client6_signal) == SIG_ERR) {
		dprintf2(LOG_WARNING, FNAME, "failed to set signal: %s",
		    strerror(errno));
		exit(1);
	}
	if (signal(SIGTERM, client6_signal) == SIG_ERR) {
		dprintf2(LOG_WARNING, FNAME, "failed to set signal: %s",
		    strerror(errno));
		exit(1);
	}
}

int
client6_start(ifp)
	struct dhcp6_if *ifp;
{
	struct dhcp6_event *ev;

	/* make sure that the interface does not have a timer */
	if (ifp->timer != NULL) {
		dprintf2(LOG_DEBUG, FNAME,
		    "removed existing timer on %s", ifp->ifname);
		dhcp6_remove_timer(&ifp->timer);
	}

	/* create an event for the initial delay */
	if ((ev = dhcp6_create_event(ifp, DHCP6S_INIT)) == NULL) {
		dprintf2(LOG_NOTICE, FNAME, "failed to create an event");
		return (-1);
	}
	TAILQ_INSERT_TAIL(&ifp->event_list, ev, link);

	if ((ev->authparam = new_authparam(ifp->authproto,
	    ifp->authalgorithm, ifp->authrdm)) == NULL) {
		dprintf2(LOG_WARNING, FNAME, "failed to allocate "
		    "authentication parameters");
		dhcp6_remove_event(ev);
		return (-1);
	}

	if ((ev->timer = dhcp6_add_timer(client6_timo, ev)) == NULL) {
		dprintf2(LOG_NOTICE, FNAME, "failed to add a timer for %s",
		    ifp->ifname);
		dhcp6_remove_event(ev);
		return (-1);
	}
	dhcp6_reset_timer(ev);

	return (0);
}

static void
client6_startall(isrestart)
	int isrestart;
{
	struct dhcp6_if *ifp;

	for (ifp = dhcp6_if; ifp; ifp = ifp->next) {
		if (isrestart &&ifreset(ifp)) {
			dprintf2(LOG_NOTICE, FNAME, "failed to reset %s",
			    ifp->ifname);
			continue; /* XXX: try to recover? */
		}
		if (client6_start(ifp))
			exit(1); /* initialization failure.  we give up. */
	}
}

static void
free_resources(freeifp)
	struct dhcp6_if *freeifp;
{
	struct dhcp6_if *ifp;

	for (ifp = dhcp6_if; ifp; ifp = ifp->next) {
		struct dhcp6_event *ev, *ev_next;

		if (freeifp != NULL && freeifp != ifp)
			continue;

		/* release all IAs as well as send RELEASE message(s) */
		release_all_ia(ifp);

		/*
		 * Cancel all outstanding events for each interface except
		 * ones being released.
		 */
		for (ev = TAILQ_FIRST(&ifp->event_list); ev; ev = ev_next) {
			ev_next = TAILQ_NEXT(ev, link);

			if (ev->state == DHCP6S_RELEASE)
				continue; /* keep it for now */

			dhcp6_remove_event(ev);
		}
	}
}

static void
check_exit()
{
	struct dhcp6_if *ifp;

	if (!exit_ok)
		return;

	for (ifp = dhcp6_if; ifp; ifp = ifp->next) {
		/*
		 * Check if we have an outstanding event.  If we do, we cannot
		 * exit for now.
		 */
		if (!TAILQ_EMPTY(&ifp->event_list))
			return;
	}
	printf("\n%s,(%d), queue is empty, exiting!!!!!\n",__FUNCTION__, __LINE__);

	/* We have no existing event.  Do exit. */
	dprintf2(LOG_INFO, FNAME, "exiting");

	exit(0);
}

static void
process_signals()
{
	//printf("\n%s,%d,enter process_signals sig_flags=%d\n",sig_flags);
	if ((sig_flags == SIGF_TERM)) {
		printf("\n%s,%d,preprare exit.. sig_flags=%d\n",sig_flags);
		exit_ok = 1;
		free_reconfig_resource();
		free_resources(NULL);
		unlink("/tmp/iana");
		check_exit();
	}
	if ((sig_flags == SIGF_HUP)) {
		dprintf2(LOG_INFO, FNAME, "restarting");
		free_resources(NULL);
		client6_startall(1);
	}

	sig_flags = 0;
}

static void
client6_mainloop()
{
	struct timeval *w;
	int ret, maxsock;
	fd_set r;

	while(1) {
		if (sig_flags)
			process_signals();

		w = dhcp6_check_timer();

		FD_ZERO(&r);
		FD_SET(sock, &r);
		maxsock = sock;
		if (ctlsock >= 0) {
			FD_SET(ctlsock, &r);
			maxsock = (sock > ctlsock) ? sock : ctlsock;
			(void)dhcp6_ctl_setreadfds(&r, &maxsock);
		}

		ret = select(maxsock + 1, &r, NULL, NULL, w);

		switch (ret) {
		case -1:
			if (errno != EINTR) {
				dprintf2(LOG_ERR, FNAME, "select: %s",
				    strerror(errno));
				exit(1);
			}
			continue;
		case 0:	/* timeout */
			break;	/* dhcp6_check_timer() will treat the case */
		default:
			break;
		}
		if (FD_ISSET(sock, &r))
			client6_recv();
		if (ctlsock >= 0) {
			if (FD_ISSET(ctlsock, &r)) {
				(void)dhcp6_ctl_acceptcommand(ctlsock,
				    client6_do_ctlcommand);
			}
			(void)dhcp6_ctl_readcommand(&r);
		}
	}
}

static inline int
get_val32(bpp, lenp, valp)
	char **bpp;
	int *lenp;
	u_int32_t *valp;
{
	char *bp = *bpp;
	int len = *lenp;
	u_int32_t i32;

	if (len < sizeof(*valp))
		return (-1);

	memcpy(&i32, bp, sizeof(i32));
	*valp = ntohl(i32);

	*bpp = bp + sizeof(*valp);
	*lenp = len - sizeof(*valp);

	return (0);
}

static inline int
get_ifname(bpp, lenp, ifbuf, ifbuflen)
	char **bpp;
	int *lenp;
	char *ifbuf;
	int ifbuflen;
{
	char *bp = *bpp;
	int len = *lenp, ifnamelen;
	u_int32_t i32;

	if (get_val32(bpp, lenp, &i32))
		return (-1);
	ifnamelen = (int)i32;

	if (*lenp < ifnamelen || ifnamelen > ifbuflen)
		return (-1);

	memset(ifbuf, 0, sizeof(ifbuf));
	memcpy(ifbuf, *bpp, ifnamelen);
	if (ifbuf[ifbuflen - 1] != '\0')
		return (-1);	/* not null terminated */

	*bpp = bp + sizeof(i32) + ifnamelen;
	*lenp = len - (sizeof(i32) + ifnamelen);

	return (0);
}

static int
client6_do_ctlcommand(buf, len)
	char *buf;
	ssize_t len;
{
	struct dhcp6ctl *ctlhead;
	u_int16_t command, version;
	u_int32_t p32, ts, ts0;
	int commandlen;
	char *bp;
	char ifname[IFNAMSIZ];
	time_t now;

	memset(ifname, 0, sizeof(ifname));

	ctlhead = (struct dhcp6ctl *)buf;

	command = ntohs(ctlhead->command);
	commandlen = (int)(ntohs(ctlhead->len));
	version = ntohs(ctlhead->version);
	if (len != sizeof(struct dhcp6ctl) + commandlen) {
		dprintf2(LOG_ERR, FNAME,
		    "assumption failure: command length mismatch");
		return (DHCP6CTL_R_FAILURE);
	}

	/* replay protection and message authentication */
	if ((now = time(NULL)) < 0) {
		dprintf2(LOG_ERR, FNAME, "failed to get current time: %s",
		    strerror(errno));
		return (DHCP6CTL_R_FAILURE);
	}
	ts0 = (u_int32_t)now;
	ts = ntohl(ctlhead->timestamp);
	if (ts + CTLSKEW < ts0 || (ts - CTLSKEW) > ts0) {
		dprintf2(LOG_INFO, FNAME, "timestamp is out of range");
		return (DHCP6CTL_R_FAILURE);
	}

	if (ctlkey == NULL) {	/* should not happen!! */
		dprintf2(LOG_ERR, FNAME, "no secret key for control channel");
		return (DHCP6CTL_R_FAILURE);
	}
	if (dhcp6_verify_mac(buf, len, DHCP6CTL_AUTHPROTO_UNDEF,
	    DHCP6CTL_AUTHALG_HMACMD5, sizeof(*ctlhead), ctlkey) != 0) {
		dprintf2(LOG_INFO, FNAME, "authentication failure");
		return (DHCP6CTL_R_FAILURE);
	}

	bp = buf + sizeof(*ctlhead) + ctldigestlen;
	commandlen -= ctldigestlen;

	if (version > DHCP6CTL_VERSION) {
		dprintf2(LOG_INFO, FNAME, "unsupported version: %d", version);
		return (DHCP6CTL_R_FAILURE);
	}

	switch (command) {
	case DHCP6CTL_COMMAND_RELOAD:
		if (commandlen != 0) {
			dprintf2(LOG_INFO, FNAME, "invalid command length "
			    "for reload: %d", commandlen);
			return (DHCP6CTL_R_DONE);
		}
		client6_reload();
		break;
	case DHCP6CTL_COMMAND_START:
		if (get_val32(&bp, &commandlen, &p32))
			return (DHCP6CTL_R_FAILURE);
		switch (p32) {
		case DHCP6CTL_INTERFACE:
			if (get_ifname(&bp, &commandlen, ifname,
			    sizeof(ifname))) {
				return (DHCP6CTL_R_FAILURE);
			}
			if (client6_ifctl(ifname, DHCP6CTL_COMMAND_START))
				return (DHCP6CTL_R_FAILURE);
			break;
		default:
			dprintf2(LOG_INFO, FNAME,
			    "unknown start target: %ul", p32);
			return (DHCP6CTL_R_FAILURE);
		}
		break;
	case DHCP6CTL_COMMAND_STOP:
		if (commandlen == 0) {
			exit_ok = 1;
			free_resources(NULL);
			unlink(pid_file);
			check_exit();
		} else {
			if (get_val32(&bp, &commandlen, &p32))
				return (DHCP6CTL_R_FAILURE);

			switch (p32) {
			case DHCP6CTL_INTERFACE:
				if (get_ifname(&bp, &commandlen, ifname,
				    sizeof(ifname))) {
					return (DHCP6CTL_R_FAILURE);
				}
				if (client6_ifctl(ifname,
				    DHCP6CTL_COMMAND_STOP)) {
					return (DHCP6CTL_R_FAILURE);
				}
				break;
			default:
				dprintf2(LOG_INFO, FNAME,
				    "unknown start target: %ul", p32);
				return (DHCP6CTL_R_FAILURE);
			}
		}
		break;
	default:
		dprintf2(LOG_INFO, FNAME,
		    "unknown control command: %d (len=%d)",
		    (int)command, commandlen);
		return (DHCP6CTL_R_FAILURE);
	}

  	return (DHCP6CTL_R_DONE);
}

static void
client6_reload()
{
	/* reload the configuration file */
	if (cfparse(conffile) != 0) {
		dprintf2(LOG_WARNING, FNAME,
		    "failed to reload configuration file");
		return;
	}

	dprintf2(LOG_NOTICE, FNAME, "client reloaded");

	return;
}

static int
client6_ifctl(ifname, command)
	char *ifname;
	u_int16_t command;
{
	struct dhcp6_if *ifp;

	if ((ifp = find_ifconfbyname(ifname)) == NULL) {
		dprintf2(LOG_INFO, FNAME,
		    "failed to find interface configuration for %s",
		    ifname);
		return (-1);
	}

	dprintf2(LOG_DEBUG, FNAME, "%s interface %s",
	    command == DHCP6CTL_COMMAND_START ? "start" : "stop", ifname);

	switch(command) {
	case DHCP6CTL_COMMAND_START:
		free_resources(ifp);
		if (client6_start(ifp)) {
			dprintf2(LOG_NOTICE, FNAME, "failed to restart %s",
			    ifname);
			return (-1);
		}
		break;
	case DHCP6CTL_COMMAND_STOP:
		free_resources(ifp);
		if (ifp->timer != NULL) {
			dprintf2(LOG_DEBUG, FNAME,
			    "removed existing timer on %s", ifp->ifname);
			dhcp6_remove_timer(&ifp->timer);
		}
		break;
	default:		/* impossible case, should be a bug */
		dprintf2(LOG_ERR, FNAME, "unknown command: %d", (int)command);
		break;
	}

	return (0);
}

static struct dhcp6_timer *
client6_expire_refreshtime(arg)
	void *arg;
{
	struct dhcp6_if *ifp = arg;

	dprintf2(LOG_DEBUG, FNAME,
	    "information refresh time on %s expired", ifp->ifname);

	dhcp6_remove_timer(&ifp->timer);
	client6_start(ifp);

	return (NULL);
}

struct dhcp6_timer *
client6_timo(arg)
	void *arg;
{
	struct dhcp6_event *ev = (struct dhcp6_event *)arg;
	struct dhcp6_if *ifp;
	int state = ev->state;

	ifp = ev->ifp;
	ev->timeouts++;

	/*
	 * Unless MRC is zero, the message exchange fails once the client has
	 * transmitted the message MRC times.
	 * [RFC3315 14.]
	 */
	/* Added by Fxconn Bernie start, when dhcp6c didn't receive reply of release in change mode then we kill the daemon.*/
	if (exit_ok == 1 && ev->max_retrans_cnt && ev->timeouts >= 2)
	{
		printf("\n%s,(%d),state=%d, exit_ok=1, kill 6c exiting..\n",__FUNCTION__, __LINE__, state);
		dhcp6_remove_event(ev);

		/* reset the reconfig phase flag */
		reconfig_phase = RECONFIG_PHASE_NO;

		if (state == DHCP6S_RELEASE)
			check_exit();

		return (NULL);

	}
	/* Added by Fxconn Bernie end, when dhcp6c didn't receive reply of release in change mode then we kill the daemon.*/
	if (ev->max_retrans_cnt && ev->timeouts >= ev->max_retrans_cnt) {
		dprintf2(LOG_INFO, FNAME, "no responses were received");
		dhcp6_remove_event(ev);

/* Foxconn Bernie added start, 2014/11/27 */
		/* reset the reconfig phase flag */
		reconfig_phase = RECONFIG_PHASE_NO;
/* Foxconn Bernie added end, 2014/11/27 */

		if (state == DHCP6S_RELEASE)
			check_exit();

		return (NULL);
	}

	switch(ev->state) {
	case DHCP6S_INIT:
		ev->timeouts = 0; /* indicate to generate a new XID. */
		if ((ifp->send_flags & DHCIFF_INFO_ONLY) || infreq_mode)
			ev->state = DHCP6S_INFOREQ;
		else {
			ev->state = DHCP6S_SOLICIT;
			if (construct_confdata(ifp, ev)) {
				dprintf2(LOG_ERR, FNAME, "can't send solicit");
				exit(1); /* XXX */
			}
		}
		dhcp6_set_timeoparam(ev); /* XXX */
		ev->max_retrans_time = sol_max_rt; /* Foxconn Bernie added , 2015/9/14@ support sol max rt option */
/* Foxconn Bernie added start, 2014/11/27 */
		client6_send(ev);
		/*ev->retrans = retrans_time;
		if (retrans_time != 1000)
			ev->timeouts++;*/
        ev->init_retrans = retrans_time;
	 	if (retrans_time != 1000) {
			ev->timeouts++;
			ev->retrans = retrans_time;
		}
		break;
/* Foxconn Bernie added end, 2014/11/27 */
		/* fall through */
	case DHCP6S_REQUEST:
	case DHCP6S_RELEASE:
	case DHCP6S_INFOREQ:
/* Foxconn Bernie modified start, 2014/11/27 @ State is rebind, when timeout need to send solicit.*/
	case DHCP6S_AFTER_DECLINE:
		client6_send(ev);
		break;
	case DHCP6S_DECLINE:
	case DHCP6S_RENEW:
	case DHCP6S_CONFIRM:
	case DHCP6S_REBIND:
		if (!TAILQ_EMPTY(&ev->data_list))
			client6_send(ev);
		else {
			dprintf2(LOG_INFO, FNAME,
			    "all information to be updated was canceled");
			if(ev->state == DHCP6S_REBIND )
			{
			//printf("\nstate is rebind,timeout.send solicit...\n");
			ev->state = DHCP6S_SOLICIT;
			ev->retrans = retrans_time;
			if (retrans_time == 1000) {
				ev->timeouts = 0;
				ev->init_retrans = 1000;
			}
			dhcp6_reset_timer(ev);
			retrans_time = ev->retrans;
			ev->state = DHCP6S_INIT;
			return (ev->timer);
			}
			//printf("\ntimeout.send remove event...\n");
			dhcp6_remove_event(ev);
			return (NULL);
		}
		break;
/* Foxconn Bernie added end, 2014/11/27 */

	case DHCP6S_SOLICIT:
		if (ev->servers) {
			/*
			 * Send a Request to the best server.
			 * Note that when we set Rapid-commit in Solicit,
			 * but a direct Reply has been delayed (very much),
			 * the transition to DHCP6S_REQUEST (and the change of
			 * transaction ID) will invalidate the reply even if it
			 * ever arrives.
			 */
			ev->current_server = select_server(ev);
			if (ev->current_server == NULL) {
				/* this should not happen! */
				dprintf2(LOG_NOTICE, FNAME,
				    "can't find a server");
				exit(1); /* XXX */
			}
			if (duidcpy(&ev->serverid,
			    &ev->current_server->optinfo.serverID)) {
				dprintf2(LOG_NOTICE, FNAME,
				    "can't copy server ID");
				return (NULL); /* XXX: better recovery? */
			}
			ev->timeouts = 0;
			ev->state = DHCP6S_REQUEST;
			dhcp6_set_timeoparam(ev);

			if (ev->authparam != NULL)
				free(ev->authparam);
			ev->authparam = ev->current_server->authparam;
			ev->current_server->authparam = NULL;

			if (construct_reqdata(ifp,
			    &ev->current_server->optinfo, ev)) {
				dprintf2(LOG_NOTICE, FNAME,
				    "failed to construct request data");
				break;
			}
		} else {
			/* Foxconn Bernie added , 2015/9/14@ If dhcp lost it server, keep the retrans time.*/
			ev->max_retrans_time = sol_max_rt;
			ev->retrans = retrans_time;
			dhcp6_reset_timer(ev);
			retrans_time = ev->retrans;
			client6_send(ev);
			return (ev->timer);
		}
		client6_send(ev);
		break;
	}

	dhcp6_reset_timer(ev);

	return (ev->timer);
}

static int
construct_confdata(ifp, ev)
	struct dhcp6_if *ifp;
	struct dhcp6_event *ev;
{
	struct ia_conf *iac;
	struct dhcp6_eventdata *evd = NULL;
	struct dhcp6_list *ial = NULL, pl;
	struct dhcp6_ia iaparam;

	TAILQ_INIT(&pl);	/* for safety */

	for (iac = TAILQ_FIRST(&ifp->iaconf_list); iac;
	    iac = TAILQ_NEXT(iac, link)) {
		/* ignore IA config currently used */
		if (!TAILQ_EMPTY(&iac->iadata))
			continue;

		evd = NULL;
		if ((evd = malloc(sizeof(*evd))) == NULL) {
			dprintf2(LOG_NOTICE, FNAME,
			    "failed to create a new event data");
			goto fail;
		}
		memset(evd, 0, sizeof(evd));

		memset(&iaparam, 0, sizeof(iaparam));
		iaparam.iaid = iac->iaid;
		switch (iac->type) {
		case IATYPE_PD:
			ial = NULL;
			if ((ial = malloc(sizeof(*ial))) == NULL)
				goto fail;
			TAILQ_INIT(ial);

			TAILQ_INIT(&pl);
			dhcp6_copy_list(&pl,
			    &((struct iapd_conf *)iac)->iapd_prefix_list);
			if (dhcp6_add_listval(ial, DHCP6_LISTVAL_IAPD,
			    &iaparam, &pl) == NULL) {
				goto fail;
			}
			dhcp6_clear_list(&pl);

			evd->type = DHCP6_EVDATA_IAPD;
			evd->data = ial;
			evd->event = ev;
			evd->destructor = destruct_iadata;
			TAILQ_INSERT_TAIL(&ev->data_list, evd, link);
			break;
		case IATYPE_NA:
			ial = NULL;
			if ((ial = malloc(sizeof(*ial))) == NULL)
				goto fail;
			TAILQ_INIT(ial);

			TAILQ_INIT(&pl);
			dhcp6_copy_list(&pl,
			    &((struct iana_conf *)iac)->iana_address_list);
			if (dhcp6_add_listval(ial, DHCP6_LISTVAL_IANA,
			    &iaparam, &pl) == NULL) {
				goto fail;
			}
			dhcp6_clear_list(&pl);

			evd->type = DHCP6_EVDATA_IANA;
			evd->data = ial;
			evd->event = ev;
			evd->destructor = destruct_iadata;
			TAILQ_INSERT_TAIL(&ev->data_list, evd, link);
			break;
		default:
			dprintf2(LOG_ERR, FNAME, "internal error");
			exit(1);
		}
	}

	return (0);

  fail:
	if (evd)
		free(evd);
	if (ial)
		free(ial);
	dhcp6_remove_event(ev);	/* XXX */

	return (-1);
}

static int
construct_reqdata(ifp, optinfo, ev)
	struct dhcp6_if *ifp;
	struct dhcp6_optinfo *optinfo;
	struct dhcp6_event *ev;
{
	struct ia_conf *iac;
	struct dhcp6_eventdata *evd = NULL;
	struct dhcp6_list *ial = NULL;
	struct dhcp6_ia iaparam;

	/* discard previous event data */
	dhcp6_remove_evdata(ev);

	if (optinfo == NULL)
		return (0);

	for (iac = TAILQ_FIRST(&ifp->iaconf_list); iac;
	    iac = TAILQ_NEXT(iac, link)) {
		struct dhcp6_listval *v;

/* Foxconn Bernie modified start, 2014/11/27 */
		/* ignore IA config currently used and it is not the reconfig message request*/
		if ((!TAILQ_EMPTY(&iac->iadata)) && (reconfig_phase != RECONFIG_PHASE_YES))
			continue;

/* Foxconn Bernie modified end, 2014/11/27 */
		memset(&iaparam, 0, sizeof(iaparam));
		iaparam.iaid = iac->iaid;

		ial = NULL;
		evd = NULL;

		switch (iac->type) {
		case IATYPE_PD:
			if ((v = dhcp6_find_listval(&optinfo->iapd_list,
			    DHCP6_LISTVAL_IAPD, &iaparam, 0)) == NULL)
				continue;

			if ((ial = malloc(sizeof(*ial))) == NULL)
				goto fail;

			TAILQ_INIT(ial);
			if (dhcp6_add_listval(ial, DHCP6_LISTVAL_IAPD,
			    &iaparam, &v->sublist) == NULL) {
				goto fail;
			}

			if ((evd = malloc(sizeof(*evd))) == NULL)
				goto fail;
			memset(evd, 0, sizeof(*evd));
			evd->type = DHCP6_EVDATA_IAPD;
			evd->data = ial;
			evd->event = ev;
			evd->destructor = destruct_iadata;
			TAILQ_INSERT_TAIL(&ev->data_list, evd, link);
			break;
		case IATYPE_NA:
			if ((v = dhcp6_find_listval(&optinfo->iana_list,
			    DHCP6_LISTVAL_IANA, &iaparam, 0)) == NULL)
				continue;

			if ((ial = malloc(sizeof(*ial))) == NULL)
				goto fail;

			TAILQ_INIT(ial);
			if (dhcp6_add_listval(ial, DHCP6_LISTVAL_IANA,
			    &iaparam, &v->sublist) == NULL) {
				goto fail;
			}

			if ((evd = malloc(sizeof(*evd))) == NULL)
				goto fail;
			memset(evd, 0, sizeof(*evd));
			evd->type = DHCP6_EVDATA_IANA;
			evd->data = ial;
			evd->event = ev;
			evd->destructor = destruct_iadata;
			TAILQ_INSERT_TAIL(&ev->data_list, evd, link);
			break;
		default:
			dprintf2(LOG_ERR, FNAME, "internal error");
			exit(1);
		}
	}

	return (0);

  fail:
	if (evd)
		free(evd);
	if (ial)
		free(ial);
	dhcp6_remove_event(ev);	/* XXX */

	return (-1);
}

static void
destruct_iadata(evd)
	struct dhcp6_eventdata *evd;
{
	struct dhcp6_list *ial;

	if (evd->type != DHCP6_EVDATA_IAPD && evd->type != DHCP6_EVDATA_IANA) {
		dprintf2(LOG_ERR, FNAME, "assumption failure %d", evd->type);
		exit(1);
	}

	ial = (struct dhcp6_list *)evd->data;
	dhcp6_clear_list(ial);
	free(ial);
}

static struct dhcp6_serverinfo *
select_server(ev)
	struct dhcp6_event *ev;
{
	struct dhcp6_serverinfo *s;

	/*
	 * pick the best server according to RFC3315 Section 17.1.3.
	 * XXX: we currently just choose the one that is active and has the
	 * highest preference.
	 */
	for (s = ev->servers; s; s = s->next) {
		if (s->active) {
			dprintf2(LOG_DEBUG, FNAME, "picked a server (ID: %s)",
			    duidstr(&s->optinfo.serverID));
			return (s);
		}
	}

	return (NULL);
}

static void
client6_signal(sig)
	int sig;
{

	switch (sig) {
	case SIGTERM:
		sig_flags |= SIGF_TERM;
		break;
	case SIGHUP:
		sig_flags |= SIGF_HUP;
		break;
	}
}

void
client6_send(ev)
	struct dhcp6_event *ev;
{
	struct dhcp6_if *ifp;
	char buf[BUFSIZ];
	struct sockaddr_in6 dst;
	struct dhcp6 *dh6;
	struct dhcp6_optinfo optinfo;
	ssize_t optlen, len;
	struct dhcp6_eventdata *evd;
/* Foxconn Bernie added start, 2014/11/27 */
	struct dhcp6_listval *ent;
	int count = 0;
/* Foxconn Bernie added end, 2014/11/27 */
	ifp = ev->ifp;

	dh6 = (struct dhcp6 *)buf;
	memset(dh6, 0, sizeof(*dh6));

	switch(ev->state) {
	case DHCP6S_SOLICIT:
		dh6->dh6_msgtype = DH6_SOLICIT;
		break;
	case DHCP6S_REQUEST:
		dh6->dh6_msgtype = DH6_REQUEST;
		break;
	case DHCP6S_RENEW:
		dh6->dh6_msgtype = DH6_RENEW;
		break;
	case DHCP6S_REBIND:
		dh6->dh6_msgtype = DH6_REBIND;
		break;
/* Foxconn Bernie added start, 2014/11/27 */
	case DHCP6S_DECLINE:
		dh6->dh6_msgtype = DH6_DECLINE;
		break;
	case DHCP6S_AFTER_DECLINE:
		dh6->dh6_msgtype = DH6_REQUEST;
		break;
	case DHCP6S_CONFIRM:
		dh6->dh6_msgtype = DH6_CONFIRM;
		break;
/* Foxconn Bernie added end, 2014/11/27 */
	case DHCP6S_RELEASE:
		dh6->dh6_msgtype = DH6_RELEASE;
		break;
	case DHCP6S_INFOREQ:
		dh6->dh6_msgtype = DH6_INFORM_REQ;
		break;
	default:
		dprintf2(LOG_ERR, FNAME, "unexpected state");
		exit(1);	/* XXX */
	}

	if (ev->timeouts == 0) {
		/*
		 * A client SHOULD generate a random number that cannot easily
		 * be guessed or predicted to use as the transaction ID for
		 * each new message it sends.
		 *
		 * A client MUST leave the transaction-ID unchanged in
		 * retransmissions of a message. [RFC3315 15.1]
		 */
#ifdef HAVE_ARC4RANDOM
		ev->xid = arc4random() & DH6_XIDMASK;
#else
		ev->xid = random() & DH6_XIDMASK;
#endif
		dprintf2(LOG_DEBUG, FNAME, "a new XID (%x) is generated",
		    ev->xid);
	}
	dh6->dh6_xid &= ~ntohl(DH6_XIDMASK);
	dh6->dh6_xid |= htonl(ev->xid);
	len = sizeof(*dh6);

	/*
	 * construct options
	 */
	dhcp6_init_options(&optinfo);

	/* server ID */
	switch (ev->state) {
	case DHCP6S_REQUEST:
	case DHCP6S_DECLINE:
	case DHCP6S_AFTER_DECLINE:
	case DHCP6S_RENEW:
	case DHCP6S_RELEASE:
		if (duidcpy(&optinfo.serverID, &ev->serverid)) {
			dprintf2(LOG_ERR, FNAME, "failed to copy server ID");
			goto end;
		}
		break;
	}

	/* client ID */
	if (duidcpy(&optinfo.clientID, &client_duid)) {
		dprintf2(LOG_ERR, FNAME, "failed to copy client ID");
		goto end;
	}

	/* rapid commit (in Solicit only) */
	if (ev->state == DHCP6S_SOLICIT &&
	    (ifp->send_flags & DHCIFF_RAPID_COMMIT)) {
		optinfo.rapidcommit = 1;
	}

/* Foxconn Bernie modified start, 2014/11/27 @ Use getDevtime function.*/
	/* elapsed time */
	if (ev->timeouts == 0) {
		//gettimeofday(&ev->tv_start, NULL);
		getDevTime(&ev->tv_start, NULL);
		optinfo.elapsed_time = 0;
	} else{
		struct timeval now, tv_diff;
		long et;

		//gettimeofday(&now, NULL);
		memset(&now,0,sizeof(now));
		memset(&tv_diff,0,sizeof(tv_diff));
		getDevTime(&now, NULL);
/* Foxconn Bernie modified end, 2014/11/27 */
		tv_sub(&now, &ev->tv_start, &tv_diff);

		/*
		 * The client uses the value 0xffff to represent any elapsed
		 * time values greater than the largest time value that can be
		 * represented in the Elapsed Time option.
		 * [RFC3315 22.9.]
		 */
		if (tv_diff.tv_sec >= (MAX_ELAPSED_TIME / 100) + 1) {
			/*
			 * Perhaps we are nervous too much, but without this
			 * additional check, we would see an overflow in 248
			 * days (of no responses).
			 */
			et = MAX_ELAPSED_TIME;
		} else {
			et = tv_diff.tv_sec * 100 + tv_diff.tv_usec / 10000;
			if (et >= MAX_ELAPSED_TIME)
				et = MAX_ELAPSED_TIME;
		}
		optinfo.elapsed_time = (int32_t)et;
	}

	/* option request options */
	if (ev->state != DHCP6S_RELEASE &&
	    dhcp6_copy_list(&optinfo.reqopt_list, &ifp->reqopt_list)) {
		dprintf2(LOG_ERR, FNAME, "failed to copy requested options");
		goto end;
	}

	/* configuration information specified as event data */
	for (evd = TAILQ_FIRST(&ev->data_list); evd;
	     evd = TAILQ_NEXT(evd, link)) {
		switch(evd->type) {
/* Foxconn Bernie modified start, 2014/11/27 */
		case DHCP6_EVDATA_IAPD:
			if (ev->state != DHCP6S_DECLINE && ev->state != DHCP6S_AFTER_DECLINE &&
			    dhcp6_copy_list(&optinfo.iapd_list, (struct dhcp6_list *)evd->data)) {
				dprintf2(LOG_NOTICE, FNAME,
				    "failed to add an IAPD");
				goto end;
			}
			break;
		case DHCP6_EVDATA_IANA:
			if (ev->state == DHCP6S_AFTER_DECLINE){
				for (ent = TAILQ_FIRST((struct dhcp6_list *)evd->data); ent; ent = TAILQ_NEXT(ent, link)) {
					dhcp6_clear_list(&ent->sublist);
				}
				ev->state == DHCP6S_REQUEST;
			}

			if (dhcp6_copy_list(&optinfo.iana_list,
			    (struct dhcp6_list *)evd->data)) {
				dprintf2(LOG_NOTICE, FNAME,
				    "failed to add an IANA");
/* Foxconn Bernie modified end, 2014/11/27 */
				goto end;
			}
			break;

		default:
			dprintf2(LOG_ERR, FNAME, "unexpected event data (%d)",
			    evd->type);
			exit(1);
		}
	}

	/* vendor class */
	if (set_vendorclass(&optinfo)) {
		dprintf2(LOG_INFO, FNAME,
			"failed to set vendor class");
		goto end;
	}

	/* vendor information */
	if (set_vendor(&optinfo)) {
		dprintf2(LOG_INFO, FNAME,
			"failed to set vendor option");
		goto end;
	}

	/* authentication information */
	if (set_auth(ev, &optinfo)) {
		dprintf2(LOG_INFO, FNAME,
		    "failed to set authentication option");
		goto end;
	}

	/* set reconfig accept option flag*/
/* Foxconn Bernie added start, 2014/11/27 */
	/*  Always include reconfig accept option in solicit packet (RFC3315 17.1.1)
	*/
	if(ev->state == DHCP6S_DECLINE || ev->state == DHCP6S_AFTER_DECLINE)
		optinfo.reconfig_flag = 0;
	else
		optinfo.reconfig_flag = 1;
/* Foxconn Bernie added end, 2014/11/27 */

	/* set options in the message */
	if ((optlen = dhcp6_set_options(dh6->dh6_msgtype,
	    (struct dhcp6opt *)(dh6 + 1),
	    (struct dhcp6opt *)(buf + sizeof(buf)), &optinfo)) < 0) {
		dprintf2(LOG_INFO, FNAME, "failed to construct options");
		goto end;
	}
	len += optlen;

	/* calculate MAC if necessary, and put it to the message */
	if (ev->authparam != NULL) {
		switch (ev->authparam->authproto) {
		case DHCP6_AUTHPROTO_DELAYED:
			if (ev->authparam->key == NULL)
				break;

			if (dhcp6_calc_mac((char *)dh6, len,
			    optinfo.authproto, optinfo.authalgorithm,
			    optinfo.delayedauth_offset + sizeof(*dh6),
			    ev->authparam->key)) {
				dprintf2(LOG_WARNING, FNAME,
				    "failed to calculate MAC");
				goto end;
			}
			break;
/* Foxconn Bernie added start, 2014/11/27 */
		case DHCP6_AUTHPROTO_RECONFIG:
			if (reconfig_key == NULL)
				break;

			if (dhcp6_calc_mac((char *)dh6, len,
			    optinfo.authproto, optinfo.authalgorithm,
			    optinfo.reconfigauth_offset + sizeof(*dh6),
			    reconfig_key)) {
				dprintf2(LOG_WARNING, FNAME,
				    "failed to calculate MAC");
				goto end;
			}
			break;
/* Foxconn Bernie added end, 2014/11/27 */
		default:
			break;	/* do nothing */
		}
	}

	/*
	 * Unless otherwise specified in this document or in a document that
	 * describes how IPv6 is carried over a specific type of link (for link
	 * types that do not support multicast), a client sends DHCP messages
	 * to the All_DHCP_Relay_Agents_and_Servers.
	 * [RFC3315 Section 13.]
	 */
	dst = *sa6_allagent;
	dst.sin6_scope_id = ifp->linkid;

try_again:
	if (sendto(sock, buf, len, 0, (struct sockaddr *)&dst,
	    sysdep_sa_len((struct sockaddr *)&dst)) == -1) {
		dprintf2(LOG_ERR, FNAME,
		    "transmit failed: %s", strerror(errno));
		count++;
		sleep(1);
		if(count < 5)
			goto try_again;
		else
		goto end;
	}

	dprintf2(LOG_DEBUG, FNAME, "send %s to %s",
	    dhcp6msgstr(dh6->dh6_msgtype), addr2str((struct sockaddr *)&dst));

  end:
	dhcp6_clear_options(&optinfo);
	return;
}

/* result will be a - b */
static void
tv_sub(a, b, result)
	struct timeval *a, *b, *result;
{
	if (a->tv_sec < b->tv_sec ||
	    (a->tv_sec == b->tv_sec && a->tv_usec < b->tv_usec)) {
		result->tv_sec = 0;
		result->tv_usec = 0;

		return;
	}

	result->tv_sec = a->tv_sec - b->tv_sec;
	if (a->tv_usec < b->tv_usec) {
		result->tv_usec = a->tv_usec + 1000000 - b->tv_usec;
		result->tv_sec -= 1;
	} else
		result->tv_usec = a->tv_usec - b->tv_usec;

	return;
}
/* Foxconn Bernie added start, 2014/11/27 @ check state code no prefixavail*/
int check_iapdStatCode(iatype, ialist, ifp)
	iatype_t iatype;
	struct dhcp6_list *ialist;
	struct dhcp6_if *ifp;
{
	struct ia *ia;
	struct ia_conf *iac;
	struct dhcp6_listval *iav, *siav;
	for (iav = TAILQ_FIRST(ialist); iav; iav = TAILQ_NEXT(iav, link)) {
			/* if we're not interested in this IA, ignore it. */
			if ((iac = find_iaconf(&ifp->iaconf_list, iatype,
				iav->val_ia.iaid)) == NULL) {
				continue;
			}

		for (siav = TAILQ_FIRST(&iav->sublist); siav;
   		 siav = TAILQ_NEXT(siav, link)){
			switch (siav->type) {
			case DHCP6_LISTVAL_STCODE:
				dprintf2(LOG_INFO, FNAME,
				    "status code for %s-%lu: %s",
				    iastr(iatype), iav->val_ia.iaid,
				    dhcp6_stcodestr(siav->val_num16));
				if ((siav->val_num16 == DH6OPT_STCODE_NOPREFIXAVAIL) || (siav->val_num16 == DH6OPT_STCODE_NOADDRSAVAIL)) {

					dprintf2(LOG_INFO, FNAME,
					    "receive iapd no prefix or iana no address\n");
					return 1 ;
				}
				break;
			default:
				dprintf2(LOG_ERR, FNAME, "impossible case");
				goto nextia;
			}
		}
nextia:
		;
	}

	return 0;

}
/* Foxconn Bernie added end, 2014/11/27 */

static void
client6_recv()
{
	char rbuf[BUFSIZ], cmsgbuf[BUFSIZ];
	struct msghdr mhdr;
	struct iovec iov;
	struct sockaddr_storage from;
	struct dhcp6_if *ifp;
	struct dhcp6opt *p, *ep;
	struct dhcp6_optinfo optinfo;
	ssize_t len;
	struct dhcp6 *dh6;
	struct cmsghdr *cm;
	struct in6_pktinfo *pi = NULL;

	memset(&iov, 0, sizeof(iov));
	memset(&mhdr, 0, sizeof(mhdr));

	iov.iov_base = (caddr_t)rbuf;
	iov.iov_len = sizeof(rbuf);
	mhdr.msg_name = (caddr_t)&from;
	mhdr.msg_namelen = sizeof(from);
	mhdr.msg_iov = &iov;
	mhdr.msg_iovlen = 1;
	mhdr.msg_control = (caddr_t)cmsgbuf;
	mhdr.msg_controllen = sizeof(cmsgbuf);
	if ((len = recvmsg(sock, &mhdr, 0)) < 0) {
		dprintf2(LOG_ERR, FNAME, "recvmsg: %s", strerror(errno));
		return;
	}

	/* detect receiving interface */
	for (cm = (struct cmsghdr *)CMSG_FIRSTHDR(&mhdr); cm;
	     cm = (struct cmsghdr *)CMSG_NXTHDR(&mhdr, cm)) {
		if (cm->cmsg_level == IPPROTO_IPV6 &&
		    cm->cmsg_type == IPV6_PKTINFO &&
		    cm->cmsg_len == CMSG_LEN(sizeof(struct in6_pktinfo))) {
			pi = (struct in6_pktinfo *)(CMSG_DATA(cm));
		}
	}
	if (pi == NULL) {
		dprintf2(LOG_NOTICE, FNAME, "failed to get packet info");
		return;
	}

	if ((ifp = find_ifconfbyid((unsigned int)pi->ipi6_ifindex)) == NULL) {
		dprintf2(LOG_INFO, FNAME, "unexpected interface (%d)",
		    (unsigned int)pi->ipi6_ifindex);
		return;
	}

	if (len < sizeof(*dh6)) {
		dprintf2(LOG_INFO, FNAME, "short packet (%d bytes)", len);
		return;
	}

	dh6 = (struct dhcp6 *)rbuf;

	dprintf2(LOG_DEBUG, FNAME, "receive %s from %s on %s",
	    dhcp6msgstr(dh6->dh6_msgtype),
	    addr2str((struct sockaddr *)&from), ifp->ifname);

	/* get options */
	dhcp6_init_options(&optinfo);
	p = (struct dhcp6opt *)(dh6 + 1);
	ep = (struct dhcp6opt *)((char *)dh6 + len);
	if (dhcp6_get_options(p, ep, &optinfo) < 0) {
		dprintf2(LOG_INFO, FNAME, "failed to parse options");
		return;
	}

	switch(dh6->dh6_msgtype) {
	case DH6_ADVERTISE:
		(void)client6_recvadvert(ifp, dh6, len, &optinfo);
		break;
	case DH6_REPLY:
		(void)client6_recvreply(ifp, dh6, len, &optinfo);
		break;
/* Foxconn Bernie added start, 2014/11/27 */
	case DH6_RECONFIGURE:
		(void)client6_recvreconfig(ifp, pi, dh6, len, &optinfo);
		break;
/* Foxconn Bernie added end, 2014/11/27 */
	default:
		dprintf2(LOG_INFO, FNAME, "received an unexpected message (%s) "
		    "from %s", dhcp6msgstr(dh6->dh6_msgtype),
		    addr2str((struct sockaddr *)&from));
		break;
	}

	dhcp6_clear_options(&optinfo);
	return;
}

static int
client6_recvadvert(ifp, dh6, len, optinfo)
	struct dhcp6_if *ifp;
	struct dhcp6 *dh6;
	ssize_t len;
	struct dhcp6_optinfo *optinfo;
{
	struct dhcp6_serverinfo *newserver, **sp;
	struct dhcp6_event *ev;
	struct dhcp6_eventdata *evd;
	struct authparam *authparam = NULL, authparam0;
	/* Foxconn Bernie added start, 2015/01/16 @ ce logo*/
	int req_pd = 0;
	int ia_pd_isempty = 1;
	struct ia_conf *iac;
	struct dhcp6_listval *ibv;
	noaddr_haspd =0;
	/* Foxconn Bernie added end, 2015/01/16 */

	/*  Foxconn Bernie added start , 2015/9/14@ support sol max rt option , SOL_MAX_RT from DHCPv6 server*/
    if ((optinfo->sol_max_rt > 0) && ( (optinfo->sol_max_rt>=60) && (optinfo->sol_max_rt<=86400) ))
	{
		printf("\n[DBG]:%s,%d,optinfo->sol_max_rt=%d, set solmaxrt.\n",__FUNCTION__,__LINE__,optinfo->sol_max_rt);
            sol_max_rt = optinfo->sol_max_rt * 1000;
	}
	/*  Foxconn Bernie added end, 2015/9/14@ support sol max rt option , SOL_MAX_RT from DHCPv6 server*/


	/* Foxconn Bernie added start, 2015/01/20 @ Support option95*/
    /* SOL_MAP_MIN from DHCPv6 server*/
    if (strlen(sol_option95))
    {
    	FILE *fp_option95;
    	printf("\n%s,%d, sol_option95=%s\n",__func__,__LINE__,sol_option95);
		if((fp_option95=fopen("/tmp/dhcp6c_option95","w"))!=NULL){
			fprintf(fp_option95,
				"%s\n",	// dhcp_option95
				sol_option95);
			fclose(fp_option95);
			system("nvram set mapmin_enable=1");
			system("killall udhcpc");
			system("nvram commit");
		}
		memset(sol_option95,0,sizeof(sol_option95));
    }
	/* Foxconn Bernie added end, 2015/01/20 @ Support option95*/


	/* find the corresponding event based on the received xid */
	ev = find_event_withid(ifp, ntohl(dh6->dh6_xid) & DH6_XIDMASK);
	if (ev == NULL) {
		dprintf2(LOG_INFO, FNAME, "XID mismatch");
		return (-1);
	}

	/* packet validation based on Section 15.3 of RFC3315. */
	if (optinfo->serverID.duid_len == 0) {
		dprintf2(LOG_INFO, FNAME, "no server ID option");
		return (-1);
	} else if (optinfo->serverID.duid_len > 128){
		dprintf2(LOG_INFO, FNAME, "server ID is over 128 octets");
		return (-1);
	} else {
		dprintf2(LOG_DEBUG, FNAME, "server ID: %s, pref=%d",
		    duidstr(&optinfo->serverID),
		    optinfo->pref);
	}
	if (optinfo->clientID.duid_len == 0) {
		dprintf2(LOG_INFO, FNAME, "no client ID option");
		return (-1);
	}
	if (duidcmp(&optinfo->clientID, &client_duid)) {
		dprintf2(LOG_INFO, FNAME, "client DUID mismatch");
		return (-1);
	}

	if (ev->authparam == NULL)
			return (-1);


	/* validate authentication */
	authparam0 = *ev->authparam;
	if (process_auth(&authparam0, dh6, len, optinfo)) {
		dprintf2(LOG_INFO, FNAME, "failed to process authentication");
		return (-1);
	}

	/* Foxconn Bernie added start, 2015/01/16 @ ce logo*/
	for (evd = TAILQ_FIRST(&ev->data_list); evd;evd = TAILQ_NEXT(evd, link)) {
		switch (evd->type) {
		case DHCP6_EVDATA_IAPD:
			req_pd = 1;
			//printf("\n%s,%d,req_pd=%d\n",__func__,__LINE__,req_pd);
			break;
		default:
			break;
		}
	}
	if (req_pd)
	{
		for (ibv = TAILQ_FIRST(&optinfo->iapd_list); ibv; ibv = TAILQ_NEXT(ibv, link))
		{
			if ((iac = find_iaconf(&ifp->iaconf_list, IATYPE_PD,
			    ibv->val_ia.iaid)) != NULL)
			{
			    ia_pd_isempty = 0;
				//printf("\n:%s,%d,ia_pd_isempty=%d\n",__func__,__LINE__,ia_pd_isempty);
			    break;
			}
		}
	}
	//printf("\n%s,%d,req_pd=%d,ia_pd_isempty=%d\n",__func__,__LINE__,req_pd,ia_pd_isempty);
	/* Foxconn Bernie added end, 2015/01/16*/

	/*
	 * The requesting router MUST ignore any Advertise message that
	 * includes a Status Code option containing the value NoPrefixAvail
	 * [RFC3633 Section 11.1].
	 * Likewise, the client MUST ignore any Advertise message that includes
	 * a Status Code option containing the value NoAddrsAvail.
	 * [RFC3315 Section 17.1.3].
	 * We only apply this when we are going to request an address or
	 * a prefix.
	 */

	for (evd = TAILQ_FIRST(&ev->data_list); evd;
	    evd = TAILQ_NEXT(evd, link)) {
		u_int16_t stcode;
		char *stcodestr;

		switch (evd->type) {
		case DHCP6_EVDATA_IAPD:
			stcode = DH6OPT_STCODE_NOPREFIXAVAIL;
			stcodestr = "NoPrefixAvail";
			break;
		case DHCP6_EVDATA_IANA:
			stcode = DH6OPT_STCODE_NOADDRSAVAIL;
			stcodestr = "NoAddrsAvail";
			break;
		default:
			continue;
		}
/* Foxconn Bernie added start, 2014/11/27 */

		if (dhcp6_find_listval(&optinfo->stcode_list,
		    DHCP6_LISTVAL_STCODE, &stcode, 0)) {
			dprintf2(LOG_INFO, FNAME,
			    "advertise contains %s status", stcodestr);
			/* Foxconn Bernie added start, 2015/01/16 @ ce logo*/
			if(stcode == DH6OPT_STCODE_NOADDRSAVAIL){
				if(req_pd==1 && ia_pd_isempty==0)
				{
					printf("\n%s,%d,OutsideStatcode include noaddrsavail. But it has iapd, so keepgoing.\n",__func__,__LINE__);
					noaddr_haspd =1;
					goto keepgoing;
				}else if ( access(DHCP_PD, F_OK) != 0)
				{
               		free_resources(NULL);
           	    	(void)client6_start(ifp);
					return(0);
				}
			}
			/* Foxconn Bernie added end, 2015/01/16 @ ce logo*/
			return (-1);
		}
	}

/* Foxconn Bernie modified start, 2015/01/16 @ ce logo*/
keepgoing:

	if(check_iapdStatCode(IATYPE_PD, &optinfo->iapd_list, ifp))
	{
		printf("\n%s,%d,recv pd no prefix,return\n",__func__,__LINE__);
		return (-1);
	}
	if(check_iapdStatCode(IATYPE_NA, &optinfo->iana_list, ifp))
	{
		printf("\n%s,%d,recv iana no address,return\n",__func__,__LINE__);
		if(req_pd==1 && ia_pd_isempty==0)
		{
			printf("\n%s,%d,ianastatcode include noaddrsavail. But it has iapd, so keepgoing.\n",__func__,__LINE__);
			noaddr_haspd =1;
		}
		else
		{
			return (-1);
		}
	}
/* Foxconn Bernie modified end, 2015/01/16 @ ce logo*/


/* Foxconn Bernie added end, 2014/11/27 */

	if (ev->state != DHCP6S_SOLICIT ||
	    (ifp->send_flags & DHCIFF_RAPID_COMMIT) || infreq_mode) {
		/*
		 * We expected a reply message, but do actually receive an
		 * Advertise message.  The server should be configured not to
		 * allow the Rapid Commit option.
		 * We process the message as if we expected the Advertise.
		 * [RFC3315 Section 17.1.4]
		 */
		dprintf2(LOG_INFO, FNAME, "unexpected advertise");
		/* proceed anyway */
	}

	/* ignore the server if it is known */
	if (find_server(ev, &optinfo->serverID)) {
		dprintf2(LOG_INFO, FNAME, "duplicated server (ID: %s)",
		    duidstr(&optinfo->serverID));
		return (-1);
	}

	/* keep the server */
	if ((newserver = malloc(sizeof(*newserver))) == NULL) {
		dprintf2(LOG_WARNING, FNAME,
		    "memory allocation failed for server");
		return (-1);
	}
	memset(newserver, 0, sizeof(*newserver));

	/* remember authentication parameters */
	newserver->authparam = ev->authparam;
	newserver->authparam->flags = authparam0.flags;
	newserver->authparam->prevrd = authparam0.prevrd;
	newserver->authparam->key = authparam0.key;

	/* allocate new authentication parameter for the soliciting event */
	if ((authparam = new_authparam(ev->authparam->authproto,
	    ev->authparam->authalgorithm, ev->authparam->authrdm)) == NULL) {
		dprintf2(LOG_WARNING, FNAME, "memory allocation failed "
		    "for authentication parameters");
		free(newserver);
		return (-1);
	}
	ev->authparam = authparam;

	/* copy options */
	dhcp6_init_options(&newserver->optinfo);
	if (dhcp6_copy_options(&newserver->optinfo, optinfo)) {
		dprintf2(LOG_ERR, FNAME, "failed to copy options");
		if (newserver->authparam != NULL)
			free(newserver->authparam);
		free(newserver);
		return (-1);
	}
	if (optinfo->pref != DH6OPT_PREF_UNDEF)
		newserver->pref = optinfo->pref;
	newserver->active = 1;
	for (sp = &ev->servers; *sp; sp = &(*sp)->next) {
		if ((*sp)->pref != DH6OPT_PREF_MAX &&
		    (*sp)->pref < newserver->pref) {
			break;
		}
	}
	newserver->next = *sp;
	*sp = newserver;

	if (newserver->pref == DH6OPT_PREF_MAX) {
		printf("\n %s,%d,include preference option\n",__func__,__LINE__);
		/*
		 * If the client receives an Advertise message that includes a
		 * Preference option with a preference value of 255, the client
		 * immediately begins a client-initiated message exchange.
		 * [RFC3315 Section 17.1.2]
		 */
		ev->current_server = newserver;
		if (duidcpy(&ev->serverid,
		    &ev->current_server->optinfo.serverID)) {
			dprintf2(LOG_NOTICE, FNAME, "can't copy server ID");
			return (-1); /* XXX: better recovery? */
		}
		if (construct_reqdata(ifp, &ev->current_server->optinfo, ev)) {
			dprintf2(LOG_NOTICE, FNAME,
			    "failed to construct request data");
			return (-1); /* XXX */
		}

		ev->timeouts = 0;
		ev->state = DHCP6S_REQUEST;

		free(ev->authparam);
		ev->authparam = newserver->authparam;
		newserver->authparam = NULL;

		client6_send(ev);

		dhcp6_set_timeoparam(ev);
		dhcp6_reset_timer(ev);

	} else if (ev->servers->next == NULL) {
		struct timeval *rest, elapsed, tv_rt, tv_irt, timo;

		printf("\n %s,%d, not include Preference option\n",__func__,__LINE__);
		/*
		 * If this is the first advertise, adjust the timer so that
		 * the client can collect other servers until IRT elapses.
		 * XXX: we did not want to do such "low level" timer
		 *      calculation here.
		 */
		rest = dhcp6_timer_rest(ev->timer);
		tv_rt.tv_sec = (ev->retrans * 1000) / 1000000;
		tv_rt.tv_usec = (ev->retrans * 1000) % 1000000;
		tv_irt.tv_sec = (ev->init_retrans * 1000) / 1000000;
		tv_irt.tv_usec = (ev->init_retrans * 1000) % 1000000;
		timeval_sub(&tv_rt, rest, &elapsed);
		if (TIMEVAL_LEQ(elapsed, tv_irt))
			timeval_sub(&tv_irt, &elapsed, &timo);
		else
			timo.tv_sec = timo.tv_usec = 0;

		dprintf2(LOG_DEBUG, FNAME, "reset timer for %s to %d.%06d",
		    ifp->ifname, (int)timo.tv_sec, (int)timo.tv_usec);

		dhcp6_set_timer(&timo, ev->timer);
	}
/* Foxconn Bernie modified start, 2014/12/2 @ The time between 1st Solicit and Request message was correct. Must IRT < RT.*/
	//ev->timeouts = 0;
	//ev->init_retrans = 300;
	//dhcp6_reset_timer(ev);
/* Foxconn Bernie modified end, 2014/12/2 */

	return (0);
}

static struct dhcp6_serverinfo *
find_server(ev, duid)
	struct dhcp6_event *ev;
	struct duid *duid;
{
	struct dhcp6_serverinfo *s;

	for (s = ev->servers; s; s = s->next) {
		if (duidcmp(&s->optinfo.serverID, duid) == 0)
			return (s);
	}

	return (NULL);
}

static int
client6_recvreply(ifp, dh6, len, optinfo)
	struct dhcp6_if *ifp;
	struct dhcp6 *dh6;
	ssize_t len;
	struct dhcp6_optinfo *optinfo;
{
	struct dhcp6_listval *lv, *ibv, *sibv=NULL; /* Foxconn Bernie modified, 2014/11/27 */
	struct dhcp6_event *ev;
	int state;
/* Foxconn Bernie added start, 2014/11/27 */
	struct ia_conf *iac;
	struct ia *ia, *ia_next;
	u_int16_t state_code = 0;
	struct dhcp6_eventdata *evd;
	int req_na = 0;
	int req_pd = 0;
	get_na_fail = 0;
	get_pd_fail = 0;
	int ia_na_isempty = 1;
	int ia_pd_isempty = 1;
/* Foxconn Bernie added end, 2014/11/27 */

	/* find the corresponding event based on the received xid */
	ev = find_event_withid(ifp, ntohl(dh6->dh6_xid) & DH6_XIDMASK);
	if (ev == NULL) {
		dprintf2(LOG_INFO, FNAME, "XID mismatch");
		return (-1);
	}

	state = ev->state;
	if (state != DHCP6S_INFOREQ &&
	    state != DHCP6S_REQUEST &&
	    state != DHCP6S_RENEW &&
	    state != DHCP6S_REBIND &&
	    state != DHCP6S_RELEASE &&
	    state != DHCP6S_DECLINE && /* Foxconn Bernie added start, 2014/11/27 */
	    state != DHCP6S_AFTER_DECLINE &&
	    state != DHCP6S_CONFIRM && /* Foxconn Bernie added end, 2014/11/27 */
	    (state != DHCP6S_SOLICIT ||
	     (ifp->send_flags & DHCIFF_RAPID_COMMIT))) {
		dprintf2(LOG_INFO, FNAME, "unexpected reply");
		return (-1);
	}

	/* A Reply message must contain a Server ID option */
	if (optinfo->serverID.duid_len == 0) {
		dprintf2(LOG_INFO, FNAME, "no server ID option");
		return (-1);
	}else
		duidcpy(&server_duid, &optinfo->serverID);

	/*
	 * DUID in the Client ID option (which must be contained for our
	 * client implementation) must match ours.
	 */
	if (optinfo->clientID.duid_len == 0) {
		dprintf2(LOG_INFO, FNAME, "no client ID option");
		return (-1);
	}
	if (duidcmp(&optinfo->clientID, &client_duid)) {
		dprintf2(LOG_INFO, FNAME, "client DUID mismatch");
		return (-1);
	}

/* Foxconn Bernie modified start, 2014/11/27 */
	/* validate authentication */
	if (ev->authparam && process_auth(ev->authparam, dh6, len, optinfo)) {
/* Foxconn Bernie modified end, 2014/11/27 */
		dprintf2(LOG_INFO, FNAME, "failed to process authentication");
		return (-1);
	}

	/*
	 * If the client included a Rapid Commit option in the Solicit message,
	 * the client discards any Reply messages it receives that do not
	 * include a Rapid Commit option.
	 * (should we keep the server otherwise?)
	 * [RFC3315 Section 17.1.4]
	 */
	if (state == DHCP6S_SOLICIT &&
	    (ifp->send_flags & DHCIFF_RAPID_COMMIT) &&
	    !optinfo->rapidcommit) {
		dprintf2(LOG_INFO, FNAME, "no rapid commit");
		return (-1);
	}

	/*
	 * The client MAY choose to report any status code or message from the
	 * status code option in the Reply message.
	 * [RFC3315 Section 18.1.8]
	 */
	for (lv = TAILQ_FIRST(&optinfo->stcode_list); lv;
	     lv = TAILQ_NEXT(lv, link)) {
		state_code = lv->val_num16; /* Foxconn Bernie added start, 2014/11/27 */
		dprintf2(LOG_INFO, FNAME, "status code: %s",
		    dhcp6_stcodestr(lv->val_num16));
	}
	switch (state_code) {
	case DH6OPT_STCODE_UNSPECFAIL:
	case DH6OPT_STCODE_USEMULTICAST:
	case DH6OPT_STCODE_NOTONLINK:   /* pling added 01/05/2016, CE Router logo */
		dprintf2(LOG_INFO, FNAME, "status code: %s",
					dhcp6_stcodestr(state_code));
		/* retransmit the message with multicast address */
		return -1;
	default:
		break;
	}
	/* Foxconn Bernie added end, 2014/11/27 */

	if (!TAILQ_EMPTY(&optinfo->dns_list)) {
		struct dhcp6_listval *d;
		int i = 0;

		for (d = TAILQ_FIRST(&optinfo->dns_list); d;
		     d = TAILQ_NEXT(d, link), i++) {
			info_printf("nameserver[%d] %s",
			    i, in6addr2str(&d->val_addr6, 0));
		}
	}

	if (!TAILQ_EMPTY(&optinfo->dnsname_list)) {
		struct dhcp6_listval *d;
		int i = 0;

		for (d = TAILQ_FIRST(&optinfo->dnsname_list); d;
		     d = TAILQ_NEXT(d, link), i++) {
			info_printf("Domain search list[%d] %s",
			    i, d->val_vbuf.dv_buf);
		}
	}

	if (!TAILQ_EMPTY(&optinfo->ntp_list)) {
		struct dhcp6_listval *d;
		int i = 0;

		for (d = TAILQ_FIRST(&optinfo->ntp_list); d;
		     d = TAILQ_NEXT(d, link), i++) {
			info_printf("NTP server[%d] %s",
			    i, d->val_vbuf.dv_buf);
		}
	}

	if (!TAILQ_EMPTY(&optinfo->sip_list)) {
		struct dhcp6_listval *d;
		int i = 0;

		for (d = TAILQ_FIRST(&optinfo->sip_list); d;
		     d = TAILQ_NEXT(d, link), i++) {
			info_printf("SIP server address[%d] %s",
			    i, in6addr2str(&d->val_addr6, 0));
		}
	}

	if (!TAILQ_EMPTY(&optinfo->sipname_list)) {
		struct dhcp6_listval *d;
		int i = 0;

		for (d = TAILQ_FIRST(&optinfo->sipname_list); d;
		     d = TAILQ_NEXT(d, link), i++) {
			info_printf("SIP domain name[%d] %s",
			    i, d->val_vbuf.dv_buf);
		}
	}

	if (!TAILQ_EMPTY(&optinfo->vendoropt_list)) {
		struct dhcp6_listval *d;
		int i = 0;

		for (d = TAILQ_FIRST(&optinfo->vendoropt_list); d;
		     d = TAILQ_NEXT(d, link), i++) {
			info_printf("Vendor option IANA : %u", d->val_num32);
		}
	}

	                //printf("peter: GOT REPLY timeout=%d\n", ev->timeouts);

/* Foxconn Bernie added start, 2014/11/27 */
    if(state == DHCP6S_DECLINE){
            /* send REQUEST message to server with none decline address */
		ev->timeouts = 0;
		ev->state = DHCP6S_AFTER_DECLINE;

		client6_send(ev);

		dhcp6_set_timeoparam(ev);
		dhcp6_reset_timer(ev);
		return 0;
	}
/* Foxconn Bernie added end, 2014/11/27 */

	/*
	 * Set refresh timer for configuration information specified in
	 * information-request.  If the timer value is specified by the server
	 * in an information refresh time option, use it; use the protocol
	 * default otherwise.
	 */
	if (state == DHCP6S_INFOREQ) {
		int64_t refreshtime = DHCP6_IRT_DEFAULT;

		if (optinfo->refreshtime != DH6OPT_REFRESHTIME_UNDEF)
			refreshtime = optinfo->refreshtime;

		ifp->timer = dhcp6_add_timer(client6_expire_refreshtime, ifp);
		if (ifp->timer == NULL) {
			dprintf2(LOG_WARNING, FNAME,
			    "failed to add timer for refresh time");
		} else {
			struct timeval tv;

			tv.tv_sec = (long)refreshtime;
			tv.tv_usec = 0;

			if (tv.tv_sec < 0) {
				/*
				 * XXX: tv_sec can overflow for an
				 * unsigned 32bit value.
				 */
				dprintf2(LOG_WARNING, FNAME,
				    "refresh time is too large: %lu",
				    (u_int32_t)refreshtime);
				tv.tv_sec = 0x7fffffff;	/* XXX */
			}

			dhcp6_set_timer(&tv, ifp->timer);
		}
	} else if (optinfo->refreshtime != DH6OPT_REFRESHTIME_UNDEF) {
		/*
		 * draft-ietf-dhc-lifetime-02 clarifies that refresh time
		 * is only used for information-request and reply exchanges.
		 */
		dprintf2(LOG_INFO, FNAME,
		    "unexpected information refresh time option (ignored)");
	}

/* Foxconn Bernie added start, 2015/01/06 */
	if(state == DHCP6S_RENEW){
		switch (state_code) {
			case DH6OPT_STCODE_SUCCESS:
				/* WNDR4500 TD#156: Send signal to radvd to refresh
				 * the prefix lifetime */
					dprintf2(LOG_INFO, FNAME, "status code: %s",
				dhcp6_stcodestr(state_code));
				printf("\n[%s][%d]: Send signal to radvd to refresh the prefix lifetime.\n", __FUNCTION__, __LINE__);
				system("killall -SIGUSR1 radvd");
				break;
			default:
				break;
		}
	}
/* Foxconn Bernie added start, 2015/01/06 */

/* Foxconn Bernie added start, 2014/11/27 */
	for (evd = TAILQ_FIRST(&ev->data_list); evd;evd = TAILQ_NEXT(evd, link)) {
		switch (evd->type) {
		case DHCP6_EVDATA_IANA:
			req_na = 1;
			break;
		case DHCP6_EVDATA_IAPD:
			req_pd = 1;
			break;
		}
	}
/* Foxconn Bernie added end, 2014/11/27 */
	/* update stateful configuration information */

	if (state != DHCP6S_RELEASE) {
/* Foxconn Bernie added start, 2014/11/27 @ Check IA_NA/IA_PD is empty or not. */
		if (req_na) {
			for (ibv = TAILQ_FIRST(&optinfo->iana_list); ibv; ibv = TAILQ_NEXT(ibv, link))
			{
				if ((iac = find_iaconf(&ifp->iaconf_list, IATYPE_NA,
				    ibv->val_ia.iaid)) != NULL)
				{
				    ia_na_isempty = 0;
				    break;
				}
			}
		}
        /* Foxconn Bernie modified start, 2016/01/05 @ ce logo*/

        if (req_pd)
        {
            for (ibv = TAILQ_FIRST(&optinfo->iapd_list); ibv; ibv = TAILQ_NEXT(ibv, link))
            {
                if ((iac = find_iaconf(&ifp->iaconf_list, IATYPE_PD,
                    ibv->val_ia.iaid)) != NULL)
                {
                    ia_pd_isempty = 0;
                    break;
                }
            }

            if(ia_pd_isempty == 0)
            {
                sibv = TAILQ_FIRST(&ibv->sublist);
                if(sibv == NULL)
                {
                    ia_pd_isempty = 1;
                    //printf("%s,%d,ia_pd_isempty=%d, sibv = null\n",__func__,__LINE__,ia_pd_isempty);
                    goto NEXTSTEP;
                }

                //printf("%s,%d,ia_pd_isempty=%d, check iapd addr.., ibv->sublist len=%d \n",__func__,__LINE__,ia_pd_isempty, sizeof(&ibv->sublist));
                for (sibv = TAILQ_FIRST(&ibv->sublist); sibv;
                 sibv = TAILQ_NEXT(sibv, link))
                {
                    //printf("%s,%d,ia_pd_isempty=%d,sibv->type=%d \n",__func__,__LINE__,ia_pd_isempty,sibv->type);
                    switch (sibv->type)
                    {
                        case DHCP6_LISTVAL_PREFIX6:
                        {
                            //printf("%s,%d, IAPD prefix %s \n",__func__,__LINE__,in6addr2str(&sibv->val_prefix6.addr, 0));
                            struct in6_addr tmpaddr;
                            memset(&tmpaddr, 0, sizeof(tmpaddr));
                            if(memcmp(&tmpaddr, &sibv->val_prefix6.addr, sizeof(tmpaddr)) == 0)
                            {
                                ia_pd_isempty = 1;
                                printf("%s,%d,ia_pd_isempty=%d, iapd addr is zero. \n",__func__,__LINE__,ia_pd_isempty);
                                goto NEXTSTEP;
                            }
                        }
                        break;
                        default:
                        break;

                    }
                }
            }
            //printf("%s,%d,ia_pd_isempty=%d \n",__func__,__LINE__,ia_pd_isempty);
        }
NEXTSTEP:
        /* Foxconn Bernie modified end, 2016/01/05 @ ce logo*/

		if (state == DHCP6S_RENEW || state == DHCP6S_REBIND) {
			if ((req_na && ia_na_isempty && !req_pd) || (req_pd && ia_pd_isempty && !req_na))
			{
                //printf("%s,%d,state is renew or rebind, return to retransmission. \n",__func__,__LINE__);
                if(state == DHCP6S_RENEW)
                {
                    return (-1);    /* pling added 01/05/2016, CE router logo */
                }
                else if((state == DHCP6S_REBIND) && (req_pd && ia_pd_isempty && !req_na) )
                {
                    //printf("%s,%d, create a new xid.\n",__func__,__LINE__);
					ev->timeouts = 0;

					client6_send(ev);
					dhcp6_set_timeoparam(ev);
					dhcp6_reset_timer(ev);
					return 0;
                }else
                    return (-1);
			}
		}
/* Foxconn Bernie added end, 2014/11/27 */

		update_ia(IATYPE_PD, &optinfo->iapd_list, ifp,
		    &optinfo->serverID, ev->authparam);
		update_ia(IATYPE_NA, &optinfo->iana_list, ifp,
		    &optinfo->serverID, ev->authparam);
/* Foxconn Bernie added start, 2014/11/27 @ Update na and pd info. Check dad status.*/
		if (get_na_fail == 1 || get_pd_fail == 1) {
			(void)client6_start(ifp);

		}

	/*Bernie add: Wait three second to detect duplicate address.
	   If There are two IANA, one is duplicate, another is not. It would remove all IANA.
	*/

		if (get_na_fail == 0 && req_na == 1 && ev->state != DHCP6S_RENEW) {
			sleep(5);

			if (dad_parse(&optinfo->iana_list, ev) == 1) {

				for (iac = TAILQ_FIRST(&ifp->iaconf_list); iac;
					iac = TAILQ_NEXT(iac, link)) {
					for (ia = TAILQ_FIRST(&iac->iadata); ia; ia = ia_next) {
						ia_next = TAILQ_NEXT(ia, link);

					TAILQ_REMOVE(&iac->iadata, ia, link);

					duidfree(&ia->serverid);

					if (ia->timer)
						dhcp6_remove_timer(&ia->timer);

					if (ia->evdata) {
						TAILQ_REMOVE(&ia->evdata->event->data_list, ia->evdata, link);
						if (ia->evdata->destructor)
							ia->evdata->destructor(ia->evdata);
						free(ia->evdata);
						ia->evdata = NULL;
					}

					if (ia->ctl && ia->ctl->cleanup)
						(*ia->ctl->cleanup)(ia->ctl);

					if (ia->authparam != NULL)
						free(ia->authparam);

						free(ia);
					}
				}

				unlink("/tmp/iana");

				update_ia(IATYPE_PD, &optinfo->iapd_list, ifp,
		    			&optinfo->serverID, ev->authparam);
				ev->timeouts = 0;
				ev->state = DHCP6S_DECLINE;
				client6_send(ev);
				dhcp6_set_timeoparam(ev);
				dhcp6_reset_timer(ev);
				return 0;
			}
		}
	}
	/* If got NA or PD success, reset retrans time*/

        if ( ((req_na == 1 && get_na_fail == 0) || (req_pd == 1 && get_pd_fail == 0))
		&& ev->state != DHCP6S_RENEW && ev->state != DHCP6S_REBIND) {
                retrans_time = 1000;
	}

	/* Note the option for reconfig message */
	if (state != DHCP6S_RELEASE && state != DHCP6S_INFOREQ) {
		note_reconfig_option(optinfo);
	}
/* Foxconn Bernie added end, 2014/11/27 */

/* Foxconn Bernie added start, 2014/11/27 @ Feedback to acos iana and pd.*/
	printf("\n[%s][%d]:oldprefix=%s,state=%d recy reply iapd prefix=%s\n", __FUNCTION__, __LINE__,oldprefix,state,iapdInfo.prefix);
	/* Foxconn Bernie added start 01/13/2015 */
	if((strlen(ianaInfo.iaaddr)>0 )&&(ianaInfo.plen<=0 || ianaInfo.plen>128)&& (ianaInfo.pltime <= ianaInfo.vltime)&& ianaInfo.isinvalid_t1t2==0)
	{
		printf("\nIana len=%d, it's invalid. Reset iana length = 64.\n",ianaInfo.plen);
		ianaInfo.plen=64;
	}
	/* Foxconn Bernie added end 01/13/2015 */

	if(strcmp(iapdInfo.prefix, oldprefix) != 0)
	{
		printf("\n[%s][%d]: Get new prefix, set dhcp6cup ..\n", __FUNCTION__, __LINE__);
	    char command[256], command2[256];
	    memset(command, 0, sizeof(command));
		if(strlen(ianaInfo.iaaddr)>0)
		{
		 sprintf(command, "dhcp6c_up %s %s %d ",
	                        dhcp6_interface,
	                        ianaInfo.iaaddr,
	                        ianaInfo.plen);

		}

		if(strlen(iapdInfo.prefix)>0)
		{
	        sprintf(command2, " %s %d &",
	                iapdInfo.prefix,
			        iapdInfo.plen);

	        if (!strlen(command))
	            sprintf(command, "dhcp6c_up %s ", dhcp6_interface);

	        strcat(command, command2);
	    }
		printf("\n[%s][%d],feedback to acos iana and pd.command=%s,noaddr_haspd=%d\n", __FUNCTION__, __LINE__,command,noaddr_haspd);
		if (strlen(command) && noaddr_haspd ==0)
			system(command);
		/* Foxconn Bernie added start, 2015/01/16 @ ce logo*/

		if(noaddr_haspd==1 && strlen(ianaInfo.iaaddr)==0 && strlen(iapdInfo.prefix)>0 && iapdInfo.vltimePD != 0)
		{
			char tmpcommand[256];
			memset(tmpcommand, 0, sizeof(tmpcommand));
			sprintf(tmpcommand, "dhcp6c_up %s %s %d %d",
							   dhcp6_interface,
							   iapdInfo.prefix,
							   iapdInfo.plen,
							   1);
			printf("\n[%s][%d]: OutsideStatcode include noaddrsavail, But it has iapd. Feedback to acos (autoconfig)...,tmpcommand=%s.\n", __FUNCTION__, __LINE__,tmpcommand);
			if (strlen(tmpcommand))
				system(tmpcommand);

			noaddr_haspd =0;

		}
		/* Foxconn Bernie added end, 2015/01/16 @ ce logo*/


	}else
	{
		/* Foxconn Bernie modified start, 2015/01/16 @ ce logo*/
		printf("\n[%s][%d]: The same prefix, do not feedback .. noaddr_haspd=%d\n", __FUNCTION__, __LINE__,noaddr_haspd);
		char tmpcommand[256] = {0}, tmpcommand2[256] = {0};
		memset(tmpcommand, 0, sizeof(tmpcommand));
		if(iana_option && strlen(ianaInfo.iaaddr)>0 && ianaInfo.vltime != 0 && (ianaInfo.pltime <= ianaInfo.vltime)) /* Foxconn Bernie modified , 2015/01/15@ ce logo, A client discards any addresses for which the preferred lifetime is
  		 greater than the valid lifetime. */
		{
			sprintf(tmpcommand, "dhcp6c_up %s %s %d ",
							   dhcp6_interface,
							   ianaInfo.iaaddr,
							   ianaInfo.plen);

            /* Foxconn Neil added, 2020/04/17, code added to make sure dhcp6c_up can run with all needed parameters  */
		    if(strlen(iapdInfo.prefix)>0)
		    {
	            sprintf(tmpcommand2, " %s %d &",
	                iapdInfo.prefix,
			        iapdInfo.plen);

	            if (!strlen(tmpcommand))
	                sprintf(tmpcommand, "dhcp6c_up %s ", dhcp6_interface);

	            strcat(tmpcommand, tmpcommand2);
	        }
            /* Foxconn Neil end, 2020/04/17, code added to make sure dhcp6c_up can run with all needed parameters  */            
			printf("\n[%s][%d]: Only Get iana,iana pltime=%lu vltime=%lu feedback to acos (autoconfig)...,tmpcommand=%s.\n", __FUNCTION__, __LINE__,ianaInfo.pltime,ianaInfo.vltime,tmpcommand);
			if (strlen(tmpcommand))
				system(tmpcommand);

            iana_option = 0;

		}
		else
		{
			printf("\n[%s][%d]: iana pltime=%lu vltime=%lu, preferred lifetime is greater than the valid lifetime or T1 greater than the T2 or no ianaAddr. Do nothing.\n", __FUNCTION__, __LINE__,ianaInfo.pltime,ianaInfo.vltime);

		}
	}
	noaddr_haspd =0;
	ianaInfo.isinvalid_t1t2=0;
	/* Foxconn Bernie modified end, 2015/01/16 @ ce logo*/

  	if(strlen(iapdInfo.prefix)>0)
	{
		strcpy(oldprefix,iapdInfo.prefix);
		printf("\n[%s][%d]: If old prefix is null, copy pd prefix to old prefix. oldprefix=%s\n", __FUNCTION__, __LINE__,oldprefix);
	}

	if (!TAILQ_EMPTY(&optinfo->vendoropt_list)) {
		struct dhcp6_listval *d;
		int i = 0;

		for (d = TAILQ_FIRST(&optinfo->vendoropt_list); d;
		     d = TAILQ_NEXT(d, link), i++) {
			info_printf("Vendor option IANA : %u", d->val_num32);
		}
	}

	/*
	 * Call the configuration script, if specified, to handle various
	 * configuration parameters.
	 */

	if (ifp->scriptpath != NULL && strlen(ifp->scriptpath) != 0) {
		dprintf2(LOG_DEBUG, FNAME, "executes %s", ifp->scriptpath);
		client6_script(ifp->scriptpath, state, optinfo);
	}
/* Foxconn Bernie added end, 2014/11/27 */
	dhcp6_remove_event(ev);

	if (state == DHCP6S_RELEASE) {
		/*
		 * When the client receives a valid Reply message in response
		 * to a Release message, the client considers the Release event
		 * completed, regardless of the Status Code option(s) returned
		 * by the server.
		 * [RFC3315 Section 18.1.8]
		 */
		unlink("/tmp/iana"); /* Foxconn Bernie added, 2014/11/27 */
		check_exit();
	}

	dprintf2(LOG_DEBUG, FNAME, "got an expected reply, sleeping.");

	if (infreq_mode) {
		exit_ok = 1;
		free_resources(NULL);
		unlink(pid_file);
		check_exit();
	}
	return (0);
}

static struct dhcp6_event *
find_event_withid(ifp, xid)
	struct dhcp6_if *ifp;
	u_int32_t xid;
{
	struct dhcp6_event *ev;

	for (ev = TAILQ_FIRST(&ifp->event_list); ev;
	     ev = TAILQ_NEXT(ev, link)) {
		if (ev->xid == xid)
			return (ev);
	}

	return (NULL);
}

static int
process_auth(authparam, dh6, len, optinfo)
	struct authparam *authparam;
	struct dhcp6 *dh6;
	ssize_t len;
	struct dhcp6_optinfo *optinfo;
{
	struct keyinfo *key = NULL;
	int authenticated = 0;
	u_int8_t msgtype = dh6->dh6_msgtype;

	switch (optinfo->authproto) {
	case DHCP6_AUTHPROTO_UNDEF:
		/* server did not provide authentication option */
		break;
	case DHCP6_AUTHPROTO_DELAYED:
		if ((optinfo->authflags & DHCP6OPT_AUTHFLAG_NOINFO)) {
			dprintf2(LOG_INFO, FNAME, "server did not include "
			    "authentication information");
			break;
		}

		if (optinfo->authalgorithm != DHCP6_AUTHALG_HMACMD5) {
			dprintf2(LOG_INFO, FNAME, "unknown authentication "
			    "algorithm (%d)", optinfo->authalgorithm);
			break;
		}

		if (optinfo->authrdm != DHCP6_AUTHRDM_MONOCOUNTER) {
			dprintf2(LOG_INFO, FNAME,"unknown RDM (%d)",
			    optinfo->authrdm);
			break;
		}

		/*
		 * Replay protection.  If we do not know the previous RD value,
		 * we accept the message anyway (XXX).
		 */
		if ((authparam->flags & AUTHPARAM_FLAGS_NOPREVRD)) {
			dprintf2(LOG_WARNING, FNAME, "previous RD value is "
			    "unknown (accept it)");
		} else {
			if (dhcp6_auth_replaycheck(optinfo->authrdm,
			    authparam->prevrd, optinfo->authrd)) {
				dprintf2(LOG_INFO, FNAME,
				    "possible replay attack detected");
				break;
			}
		}

		/* identify the secret key */
		if ((key = authparam->key) != NULL) {
			/*
			 * If we already know a key, its identification should
			 * match that contained in the received option.
			 * (from Section 21.4.5.1 of RFC3315)
			 */
			if (optinfo->delayedauth_keyid != key->keyid ||
			    optinfo->delayedauth_realmlen != key->realmlen ||
			    memcmp(optinfo->delayedauth_realmval, key->realm,
			    key->realmlen) != 0) {
				dprintf2(LOG_INFO, FNAME,
				    "authentication key mismatch");
				break;
			}
		} else {
			key = find_key(optinfo->delayedauth_realmval,
			    optinfo->delayedauth_realmlen,
			    optinfo->delayedauth_keyid);
			if (key == NULL) {
				dprintf2(LOG_INFO, FNAME, "failed to find key "
				    "provided by the server (ID: %x)",
				    optinfo->delayedauth_keyid);
				break;
			} else {
				dprintf2(LOG_DEBUG, FNAME, "found key for "
				    "authentication: %s", key->name);
			}
			authparam->key = key;
		}

		/* check for the key lifetime */
		if (dhcp6_validate_key(key)) {
			dprintf2(LOG_INFO, FNAME, "key %s has expired",
			    key->name);
			break;
		}

		/* validate MAC */
		if (dhcp6_verify_mac((char *)dh6, len, optinfo->authproto,
		    optinfo->authalgorithm,
		    optinfo->delayedauth_offset + sizeof(*dh6), key) == 0) {
			dprintf2(LOG_DEBUG, FNAME, "message authentication "
			    "validated");
			authenticated = 1;
		} else {
			dprintf2(LOG_INFO, FNAME, "invalid message "
			    "authentication");
		}

		break;

/* Foxconn Bernie added start, 2014/11/27 */
	case DHCP6_AUTHPROTO_RECONFIG:
		if ((optinfo->authflags & DHCP6OPT_AUTHFLAG_NOINFO)) {
			dprintf2(LOG_INFO, FNAME, "server did not include "
			        "authentication information");
			break;
		}

		if (optinfo->authalgorithm != DHCP6_AUTHALG_HMACMD5) {
			dprintf2(LOG_INFO, FNAME, "unknown authentication "
			        "algorithm (%d)", optinfo->authalgorithm);
			break;
		}

		if (optinfo->authrdm != DHCP6_AUTHRDM_MONOCOUNTER) {
			dprintf2(LOG_INFO, FNAME,"unknown RDM (%d)",
			        optinfo->authrdm);
			break;
		}
		switch(optinfo->reconfigauth_type){
			case DH6OPT_AUTH_RECONFIG_KEY:
				/* Reconfigure Key value (used in Reply message). */
				if (msgtype != DH6_REPLY) {
					dprintf2(LOG_INFO, FNAME, "recv the wrong authentication type"
					        "about DH6OPT_AUTH_RECONFIG_KEY");
					return -1;
				}
				reconfig_key = malloc(sizeof(struct keyinfo));
				reconfig_key->secretlen = sizeof(optinfo->reconfigauth_val);
				reconfig_key->secret = malloc(sizeof(optinfo->reconfigauth_val));
				memcpy(reconfig_key->secret, optinfo->reconfigauth_val, reconfig_key->secretlen);
				return 0;
			case DH6OPT_AUTH_RECONFIG_HMACMD5:
				/* HMAC-MD5 digest of the message (used in Reconfigure  message). */
				if (msgtype != DH6_RECONFIGURE) {
					dprintf2(LOG_INFO, FNAME, "recv the wrong authentication type"
					        "about DH6OPT_AUTH_RECONFIG_HMACMD5");
					return -1;
				}
				/* validate MAC */
				if (dhcp6_verify_mac((char *)dh6, len, optinfo->authproto,
				    optinfo->authalgorithm,
				    optinfo->reconfigauth_offset + sizeof(*dh6), reconfig_key) == 0) {
					dprintf2(LOG_DEBUG, FNAME, "message authentication validated");
					return 0;
				} else {
					dprintf2(LOG_INFO, FNAME, "invalid message authentication");
					return 0; /* Foxconn Bernie modified, 2016/01/19 @ CHT-TL bug: Make sure reconfigure message can run on any server. */
				}
				break;
			default:
				dprintf2(LOG_INFO, FNAME, "server sent unsupported "
				        "authentication type: %d", optinfo->reconfigauth_type);
				break;
		}

		break;
/* Foxconn Bernie added end, 2014/11/27 */

	default:
		dprintf2(LOG_INFO, FNAME, "server sent unsupported "
		    "authentication protocol (%d)", optinfo->authproto);
		break;
	}

	if (authenticated == 0) {
		if (authparam->authproto != DHCP6_AUTHPROTO_UNDEF) {
			dprintf2(LOG_INFO, FNAME, "message not authenticated "
			    "while authentication required");

			/*
			 * Right now, we simply discard unauthenticated
			 * messages.
			 */
			return (-1);
		}
	} else {
		/* if authenticated, update the "previous" RD value */
		authparam->prevrd = optinfo->authrd;
		authparam->flags &= ~AUTHPARAM_FLAGS_NOPREVRD;
	}

	return (0);
}

static int
set_auth(ev, optinfo)
	struct dhcp6_event *ev;
	struct dhcp6_optinfo *optinfo;
{
	struct authparam *authparam = ev->authparam;

	if (authparam == NULL)
		return (0);

	optinfo->authproto = authparam->authproto;
	optinfo->authalgorithm = authparam->authalgorithm;
	optinfo->authrdm = authparam->authrdm;

	switch (authparam->authproto) {
	case DHCP6_AUTHPROTO_UNDEF: /* we simply do not need authentication */
		return (0);
	case DHCP6_AUTHPROTO_DELAYED:
		if (ev->state == DHCP6S_INFOREQ) {
			/*
			 * In the current implementation, delayed
			 * authentication for Information-request and Reply
			 * exchanges doesn't work.  Specification is also
			 * unclear on this usage.
			 */
			dprintf2(LOG_WARNING, FNAME, "delayed authentication "
			    "cannot be used for Information-request yet");
			return (-1);
		}

		if (ev->state == DHCP6S_SOLICIT) {
			optinfo->authflags |= DHCP6OPT_AUTHFLAG_NOINFO;
			return (0); /* no auth information is needed */
		}

		if (authparam->key == NULL) {
			dprintf2(LOG_INFO, FNAME,
			    "no authentication key for %s",
			    dhcp6_event_statestr(ev));
			return (-1);
		}

		if (dhcp6_validate_key(authparam->key)) {
			dprintf2(LOG_INFO, FNAME, "key %s is invalid",
			    authparam->key->name);
			return (-1);
		}

		if (get_rdvalue(optinfo->authrdm, &optinfo->authrd,
		    sizeof(optinfo->authrd))) {
			dprintf2(LOG_ERR, FNAME, "failed to get a replay "
			    "detection value");
			return (-1);
		}

		optinfo->delayedauth_keyid = authparam->key->keyid;
		optinfo->delayedauth_realmlen = authparam->key->realmlen;
		optinfo->delayedauth_realmval =
		    malloc(optinfo->delayedauth_realmlen);
		if (optinfo->delayedauth_realmval == NULL) {
			dprintf2(LOG_ERR, FNAME, "failed to allocate memory "
			    "for authentication realm");
			return (-1);
		}
		memcpy(optinfo->delayedauth_realmval, authparam->key->realm,
		    optinfo->delayedauth_realmlen);

		break;

/* Foxconn Bernie added start, 2014/11/27 */
	case DHCP6_AUTHPROTO_RECONFIG:
		/* set replay detection */
		if (get_rdvalue(optinfo->authrdm, &optinfo->authrd,
		                sizeof(optinfo->authrd)))
		{
			dprintf2(LOG_ERR, FNAME, "failed to get a replay detection value");
			return (-1);
		}
		optinfo->reconfigauth_type = DH6OPT_AUTH_RECONFIG_HMACMD5;
		break;
/* Foxconn Bernie added end, 2014/11/27 */

	default:
		dprintf2(LOG_ERR, FNAME, "unsupported authentication protocol "
		    "%d", authparam->authproto);
		return (-1);
	}

	return (0);
}

static int
set_vendorclass(optinfo)
	struct dhcp6_optinfo *optinfo;
{
	struct dhcp6_list *vl = NULL;
	struct dhcp6_list *vendor_sub_opt = NULL;
	struct vendorclass *config_vlist;
	struct dhcp6_listval *lv;

	u_int32_t vendor_iana;

	vl = &optinfo->vendorclass_list;

	for(config_vlist = vendorclass_list; config_vlist != NULL; config_vlist = config_vlist->next) {
		struct dhcp6_vbuf subopt;
		char subopt_valstr[256];

		/* Init sub-option list*/
		vendor_sub_opt = NULL;
		memset(subopt_valstr,0,sizeof(subopt_valstr));
		if ((vendor_sub_opt = malloc(sizeof(*vendor_sub_opt))) == NULL)
			return -1;
		TAILQ_INIT(vendor_sub_opt);

		/* Add vendor-class-data to sub option */
		sprintf(subopt_valstr,"%s",config_vlist->vendor_class_data);
		subopt.dv_len = strlen(subopt_valstr);
		subopt.dv_buf = subopt_valstr;

		if (dhcp6_add_listval(vendor_sub_opt, DHCP6_LISTVAL_VENDORCLASS_DATA,
			&subopt, NULL) == NULL) {
			dprintf2(LOG_ERR, FNAME, "failed to add a vendor-class-data %s ",subopt_valstr);
			return -1;
		}

		vendor_iana = config_vlist->iana;
		/* Add vendor-class to option config_vlist */
		if (dhcp6_add_listval(vl, DHCP6_LISTVAL_VENDORCLASS,
			&vendor_iana, vendor_sub_opt) == NULL) {
			dprintf2(LOG_ERR, FNAME, "failed to add vendor-class %d ",vendor_iana);
			return -1;
		}
	}
	return (0);
}

static int
set_vendor(optinfo)
	struct dhcp6_optinfo *optinfo;
{
	struct dhcp6_list *vl = NULL;
	struct dhcp6_list *vendor_BBF_sub_opt = NULL;
	struct dhcp6_list *vendor_CHTR_sub_opt = NULL;
	struct vendorinfo *config_vlist;
	struct dhcp6_listval *lv;

	int send_subopt_bbf = 0;
	int send_subopt_chtr = 0;

	struct dhcp6_vbuf subopt;
	u_int32_t vendor_iana;

	vl = &optinfo->vendoropt_list;

	for(config_vlist = vendor_list; config_vlist != NULL; config_vlist = config_vlist->next){
		struct dhcp6_vbuf subopt;
		char subopt_valstr[256];

		vendor_BBF_sub_opt = NULL;
		vendor_CHTR_sub_opt = NULL;
		memset(subopt_valstr,0,sizeof(subopt_valstr));

		if ((vendor_BBF_sub_opt = malloc(sizeof(*vendor_BBF_sub_opt))) == NULL)
			return -1;
		TAILQ_INIT(vendor_BBF_sub_opt);

		if ((vendor_CHTR_sub_opt = malloc(sizeof(*vendor_CHTR_sub_opt))) == NULL)
			return -1;
		TAILQ_INIT(vendor_CHTR_sub_opt);

		/* Add OUI to sub option*/
		if((vendor_list->oui != NULL) && (strlen(vendor_list->oui) > 0)) {
			sprintf(subopt_valstr,"%s",vendor_list->oui);
			subopt.dv_len = strlen(subopt_valstr);
			subopt.dv_buf = subopt_valstr;

			if (dhcp6_add_listval(vendor_BBF_sub_opt, DHCP6_LISTVAL_VENDOR_OUI, &subopt, NULL) == NULL) {
				dprintf2(LOG_ERR, FNAME, "failed to add a vendor sub opt %s ",subopt_valstr);
				return -1;
			}
			send_subopt_bbf = 1;
		}

		/* Add SN to sub option*/
		if((vendor_list->sn != NULL) && (strlen(vendor_list->sn) > 0)) {
			sprintf(subopt_valstr,"%s",vendor_list->sn);
			subopt.dv_len = strlen(subopt_valstr);
			subopt.dv_buf = subopt_valstr;

			if (dhcp6_add_listval(vendor_BBF_sub_opt, DHCP6_LISTVAL_VENDOR_SN, &subopt, NULL) == NULL) {
				dprintf2(LOG_ERR, FNAME, "failed to add a vendor sub opt %s ",subopt_valstr);
				return -1;
			}
			send_subopt_bbf = 1;
		}

		/* Add Product Class to sub option*/
		if((vendor_list->product_class != NULL) && (strlen(vendor_list->product_class) > 0)) {
			sprintf(subopt_valstr,"%s",vendor_list->product_class);
			subopt.dv_len = strlen(subopt_valstr);
			subopt.dv_buf = subopt_valstr;

			if (dhcp6_add_listval(vendor_BBF_sub_opt, DHCP6_LISTVAL_VENDOR_PD, &subopt, NULL) == NULL) {
				dprintf2(LOG_ERR, FNAME, "failed to add a vendor sub opt %s ",subopt_valstr);
				return -1;
			}
			send_subopt_bbf = 1;
		}

		/* Add Option Request Option to sub option*/
		if((vendor_list->oro != NULL) && (strlen(vendor_list->oro) > 0)) {
			sprintf(subopt_valstr,"%s",vendor_list->oro);
			subopt.dv_len = strlen(subopt_valstr);
			subopt.dv_buf = subopt_valstr;

			if (dhcp6_add_listval(vendor_CHTR_sub_opt, DHCP6_LISTVAL_VENDOR_CHTR_ORO, &subopt, NULL) == NULL) {
				dprintf2(LOG_ERR, FNAME, "failed to add a vendor sub opt %s ",subopt_valstr);
				return -1;
			}
			send_subopt_chtr = 1;
		}

		/* Add Device Type to sub option*/
		if((vendor_list->cl_devicetype != NULL) && (strlen(vendor_list->cl_devicetype) > 0)) {
			sprintf(subopt_valstr,"%s",vendor_list->cl_devicetype);
			subopt.dv_len = strlen(subopt_valstr);
			subopt.dv_buf = subopt_valstr;

			if (dhcp6_add_listval(vendor_CHTR_sub_opt, DHCP6_LISTVAL_VENDOR_CHTR_DEVTYPE, &subopt, NULL) == NULL) {
				dprintf2(LOG_ERR, FNAME, "failed to add a vendor sub opt %s ",subopt_valstr);
				return -1;
			}
			send_subopt_chtr = 1;
		}

		/* Add eCM/eSafe Colon to sub option*/
		if((vendor_list->cl_ecmesafe != NULL) && (strlen(vendor_list->cl_ecmesafe) > 0)) {
			sprintf(subopt_valstr,"%s",vendor_list->cl_ecmesafe);
			subopt.dv_len = strlen(subopt_valstr);
			subopt.dv_buf = subopt_valstr;

			if (dhcp6_add_listval(vendor_CHTR_sub_opt, DHCP6_LISTVAL_VENDOR_CHTR_ECMESAFE, &subopt, NULL) == NULL) {
				dprintf2(LOG_ERR, FNAME, "failed to add a vendor sub opt %s ",subopt_valstr);
				return -1;
			}
			send_subopt_chtr = 1;
		}

		/* Add Device Serial Number to sub option*/
		if((vendor_list->dev_sn != NULL) && (strlen(vendor_list->dev_sn) > 0)) {
			sprintf(subopt_valstr,"%s",vendor_list->dev_sn);
			subopt.dv_len = strlen(subopt_valstr);
			subopt.dv_buf = subopt_valstr;

			if (dhcp6_add_listval(vendor_CHTR_sub_opt, DHCP6_LISTVAL_VENDOR_CHTR_DEVSN, &subopt, NULL) == NULL) {
				dprintf2(LOG_ERR, FNAME, "failed to add a vendor sub opt %s ",subopt_valstr);
				return -1;
			}
			send_subopt_chtr = 1;
		}

		/* Add Hardware Version Number to sub option*/
		if((vendor_list->dev_hwver != NULL) && (strlen(vendor_list->dev_hwver) > 0)) {
			sprintf(subopt_valstr,"%s",vendor_list->dev_hwver);
			subopt.dv_len = strlen(subopt_valstr);
			subopt.dv_buf = subopt_valstr;

			if (dhcp6_add_listval(vendor_CHTR_sub_opt, DHCP6_LISTVAL_VENDOR_CHTR_DEVHWVER, &subopt, NULL) == NULL) {
				dprintf2(LOG_ERR, FNAME, "failed to add a vendor sub opt %s ",subopt_valstr);
				return -1;
			}
			send_subopt_chtr = 1;
		}

		/* Add Software Version Number to sub option*/
		if((vendor_list->dev_swver != NULL) && (strlen(vendor_list->dev_swver) > 0)) {
			sprintf(subopt_valstr,"%s",vendor_list->dev_swver);
			subopt.dv_len = strlen(subopt_valstr);
			subopt.dv_buf = subopt_valstr;

			if (dhcp6_add_listval(vendor_CHTR_sub_opt, DHCP6_LISTVAL_VENDOR_CHTR_DEVSWVER, &subopt, NULL) == NULL) {
				dprintf2(LOG_ERR, FNAME, "failed to add a vendor sub opt %s ",subopt_valstr);
				return -1;
			}
			send_subopt_chtr = 1;
		}

		/* Add Boot ROM version to sub option*/
		if((vendor_list->dev_blver != NULL) && (strlen(vendor_list->dev_blver) > 0)) {
			sprintf(subopt_valstr,"%s",vendor_list->dev_blver);
			subopt.dv_len = strlen(subopt_valstr);
			subopt.dv_buf = subopt_valstr;

			if (dhcp6_add_listval(vendor_CHTR_sub_opt, DHCP6_LISTVAL_VENDOR_CHTR_DEVBLVER, &subopt, NULL) == NULL) {
				dprintf2(LOG_ERR, FNAME, "failed to add a vendor sub opt %s ",subopt_valstr);
				return -1;
			}
			send_subopt_chtr = 1;
		}

		/* Add Vendor OUI to sub option*/
		if((vendor_list->dev_vendoroui != NULL) && (strlen(vendor_list->dev_vendoroui) > 0)) {
			sprintf(subopt_valstr,"%s",vendor_list->dev_vendoroui);
			subopt.dv_len = strlen(subopt_valstr);
			subopt.dv_buf = subopt_valstr;

			if (dhcp6_add_listval(vendor_CHTR_sub_opt, DHCP6_LISTVAL_VENDOR_CHTR_DEVVENDEROUI, &subopt, NULL) == NULL) {
				dprintf2(LOG_ERR, FNAME, "failed to add a vendor sub opt %s ",subopt_valstr);
				return -1;
			}
			send_subopt_chtr = 1;
		}

		/* Add Model Number to sub option*/
		if((vendor_list->dev_modelid != NULL) && (strlen(vendor_list->dev_modelid) > 0)) {
			sprintf(subopt_valstr,"%s",vendor_list->dev_modelid);
			subopt.dv_len = strlen(subopt_valstr);
			subopt.dv_buf = subopt_valstr;

			if (dhcp6_add_listval(vendor_CHTR_sub_opt, DHCP6_LISTVAL_VENDOR_CHTR_DEVMODELID, &subopt, NULL) == NULL) {
				dprintf2(LOG_ERR, FNAME, "failed to add a vendor sub opt %s ",subopt_valstr);
				return -1;
			}
			send_subopt_chtr = 1;
		}

		/* Add Vendor Identifier to sub option*/
		if((vendor_list->dev_vendorid != NULL) && (strlen(vendor_list->dev_vendorid) > 0)) {
			sprintf(subopt_valstr,"%s",vendor_list->dev_vendorid);
			subopt.dv_len = strlen(subopt_valstr);
			subopt.dv_buf = subopt_valstr;

			if (dhcp6_add_listval(vendor_CHTR_sub_opt, DHCP6_LISTVAL_VENDOR_CHTR_DEVVENDERID, &subopt, NULL) == NULL) {
				dprintf2(LOG_ERR, FNAME, "failed to add a vendor sub opt %s ",subopt_valstr);
				return -1;
			}
			send_subopt_chtr = 1;
		}

		/* Add sub option to vendor option list*/
		if(send_subopt_bbf) {
			vendor_iana = VENDOR_IANA_ID_BROADBAND_FORUM;
			if (dhcp6_add_listval(vl, DHCP6_LISTVAL_VENDOR,
				&vendor_iana, vendor_BBF_sub_opt) == NULL) {
				dprintf2(LOG_ERR, FNAME, "failed to add a %d ",vendor_iana);
				return -1;
			}
		}

		if(send_subopt_chtr) {
			vendor_iana = VENDOR_IANA_ID_CHARTER;
			if (dhcp6_add_listval(vl, DHCP6_LISTVAL_VENDOR,
				&vendor_iana, vendor_CHTR_sub_opt) == NULL) {
				dprintf2(LOG_ERR, FNAME, "failed to add a %d ",vendor_iana);
				return -1;
			}
		}
	}

	return (0);
}

static void
info_printf(const char *fmt, ...)
{
	va_list ap;
	char logbuf[LINE_MAX];

	va_start(ap, fmt);
	vsnprintf(logbuf, sizeof(logbuf), fmt, ap);

	dprintf2(LOG_DEBUG, FNAME, "%s", logbuf);
	if (infreq_mode)
		printf("%s\n", logbuf);

	return;
}
