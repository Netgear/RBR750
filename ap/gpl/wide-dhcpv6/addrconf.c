/*	$KAME: addrconf.c,v 1.8 2005/09/16 11:30:13 suz Exp $	*/

/*
 * Copyright (C) 2002 WIDE Project.
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
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/queue.h>
#include <sys/ioctl.h>

#include <net/if.h>
#ifdef __FreeBSD__
#include <net/if_var.h>
#endif

#include <netinet/in.h>

#ifdef __KAME__
#include <netinet6/in6_var.h>
#include <netinet6/nd6.h>
#endif

#include <errno.h>
#include <syslog.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "dhcp6.h"
#include "config.h"
#include "common.h"
#include "timer.h"
#include "dhcp6c_ia.h"
#include "prefixconf.h"


TAILQ_HEAD(statefuladdr_list, statefuladdr);
struct iactl_na {
	struct iactl common;
	struct statefuladdr_list statefuladdr_head;
};
#define iacna_ia common.iactl_ia
#define iacna_callback common.callback
#define iacna_isvalid common.isvalid
#define iacna_duration common.duration
#define iacna_renew_data common.renew_data
#define iacna_rebind_data common.rebind_data
#define iacna_reestablish_data common.reestablish_data
#define iacna_release_data common.release_data
#define iacna_cleanup common.cleanup

struct statefuladdr {
	TAILQ_ENTRY (statefuladdr) link;

	struct dhcp6_statefuladdr addr;
	time_t updatetime;
	struct dhcp6_timer *timer;
	struct iactl_na *ctl;
	struct dhcp6_if *dhcpif;
};

/* Foxconn Bernie added start, 2014/11/27 */
extern struct iana_information ianaInfo;
extern struct iapd_information iapdInfo;
/* Foxconn Bernie added end, 2014/11/27 */

static struct statefuladdr *find_addr __P((struct statefuladdr_list *,
    struct dhcp6_statefuladdr *));
static int remove_addr __P((struct statefuladdr *));
static int isvalid_addr __P((struct iactl *));
static u_int32_t duration_addr __P((struct iactl *));
static void cleanup_addr __P((struct iactl *));
static int renew_addr __P((struct iactl *, struct dhcp6_ia *,
    struct dhcp6_eventdata **, struct dhcp6_eventdata *));
static void na_renew_data_free __P((struct dhcp6_eventdata *));

static struct dhcp6_timer *addr_timo __P((void *));

static int na_ifaddrconf __P((ifaddrconf_cmd_t, struct statefuladdr *, int plen)); /* Foxconn Bernie added modified, 2014/11/27 */

extern struct dhcp6_timer *client6_timo __P((void *));


int
update_address(ia, addr, plen, dhcpifp, ctlp, callback) /* Foxconn Bernie added modified, 2014/11/27 */
	struct ia *ia;
	struct dhcp6_statefuladdr *addr;
	int plen; /* Foxconn Bernie added, 2014/11/27 */
	struct dhcp6_if *dhcpifp;
	struct iactl **ctlp;
	void (*callback)__P((struct ia *));
{
	struct iactl_na *iac_na = (struct iactl_na *)*ctlp;
	struct statefuladdr *sa;
	int sacreate = 0;
	struct timeval timo;

	/*
	 * A client discards any addresses for which the preferred
         * lifetime is greater than the valid lifetime.
	 * [RFC3315 22.6] 
	 */
	if (addr->vltime != DHCP6_DURATION_INFINITE &&
	    (addr->pltime == DHCP6_DURATION_INFINITE ||
	    addr->pltime > addr->vltime)) {
		dprintf2(LOG_INFO, FNAME, "invalid address %s: "
		    "pltime (%lu) is larger than vltime (%lu)",
		    in6addr2str(&addr->addr, 0),
		    addr->pltime, addr->vltime);
		return (-1);
	}

	if (iac_na == NULL) {
		if ((iac_na = malloc(sizeof(*iac_na))) == NULL) {
			dprintf2(LOG_NOTICE, FNAME, "memory allocation failed");
			return (-1);
		}
		memset(iac_na, 0, sizeof(*iac_na));
		iac_na->iacna_ia = ia;
		iac_na->iacna_callback = callback;
		iac_na->iacna_isvalid = isvalid_addr;
		iac_na->iacna_duration = duration_addr;
		iac_na->iacna_cleanup = cleanup_addr;
		iac_na->iacna_renew_data =
		    iac_na->iacna_rebind_data =
		    iac_na->iacna_release_data =
		    iac_na->iacna_reestablish_data = renew_addr;

		TAILQ_INIT(&iac_na->statefuladdr_head);
		*ctlp = (struct iactl *)iac_na;
	}

	/* search for the given address, and make a new one if it fails */
	if ((sa = find_addr(&iac_na->statefuladdr_head, addr)) == NULL) {
		if ((sa = malloc(sizeof(*sa))) == NULL) {
			dprintf2(LOG_NOTICE, FNAME, "memory allocation failed");
			return (-1);
		}
		memset(sa, 0, sizeof(*sa));
		sa->addr.addr = addr->addr;
		sa->ctl = iac_na;
		TAILQ_INSERT_TAIL(&iac_na->statefuladdr_head, sa, link);
		sacreate = 1;
	}

	/* update the timestamp of update */
	sa->updatetime = time(NULL);

	/* update the prefix according to addr */
	sa->addr.pltime = addr->pltime;
	sa->addr.vltime = addr->vltime;
/* Foxconn Bernie modified start, 2014/11/27 */	
	sa->addr.plen = plen; 
	sa->dhcpif = dhcpifp;
	dprintf2(LOG_DEBUG, FNAME, "%s an address %s pltime=%lu, vltime=%lu, plen=%d",
	    sacreate ? "create" : "update",
	    in6addr2str(&addr->addr, 0), addr->pltime, addr->vltime,plen);

	if (sa->addr.vltime != 0
#ifdef __linux__
/*
 In linux: Can not add the same ipv6 address again, 
           call na_ifaddrconf will be failed.
           So call na_ifaddrconf only at first time.
*/
		&& sacreate
#endif
	   )
		if (na_ifaddrconf(IFADDRCONF_ADD, sa, plen) < 0)
			return (-1);
/* Foxconn Bernie modified end, 2014/11/27 */

	/*
	 * If the new vltime is 0, this address immediately expires.
	 * Otherwise, set up or update the associated timer.
	 */
	switch (sa->addr.vltime) {
	case 0:
		if (remove_addr(sa) < 0)
			return (-1);
		break;
	case DHCP6_DURATION_INFINITE:
		if (sa->timer)
			dhcp6_remove_timer(&sa->timer);
		break;
	default:
		if (sa->timer == NULL) {
			sa->timer = dhcp6_add_timer(addr_timo, sa);
			if (sa->timer == NULL) {
				dprintf2(LOG_NOTICE, FNAME,
				    "failed to add stateful addr timer");
				remove_addr(sa); /* XXX */
				return (-1);
			}
		}
		/* update the timer */
		timo.tv_sec = sa->addr.vltime;
		timo.tv_usec = 0;

		dhcp6_set_timer(&timo, sa->timer);
		break;
	}

	return (0);
}

static struct statefuladdr *
find_addr(head, addr)
	struct statefuladdr_list *head;
	struct dhcp6_statefuladdr *addr;
{
	struct statefuladdr *sa;

	for (sa = TAILQ_FIRST(head); sa; sa = TAILQ_NEXT(sa, link)) {
		if (!IN6_ARE_ADDR_EQUAL(&sa->addr.addr, &addr->addr))
			continue;
		return (sa);
	}

	return (NULL);
}

static int
remove_addr(sa)
	struct statefuladdr *sa;
{
	int ret;

	dprintf2(LOG_DEBUG, FNAME, "remove an address %s",
	    in6addr2str(&sa->addr.addr, 0));

	if (sa->timer)
		dhcp6_remove_timer(&sa->timer);

	TAILQ_REMOVE(&sa->ctl->statefuladdr_head, sa, link);
	ret = na_ifaddrconf(IFADDRCONF_REMOVE, sa, sa->addr.plen); /* Foxconn Bernie added, 2014/11/27 */

	free(sa);

	return (ret);
}

static int
isvalid_addr(iac)
	struct iactl *iac;
{
	struct iactl_na *iac_na = (struct iactl_na *)iac;

	if (TAILQ_EMPTY(&iac_na->statefuladdr_head))
		return (0);	/* this IA is invalid */
	return (1);
}

static u_int32_t
duration_addr(iac)
	struct iactl *iac;
{
	struct iactl_na *iac_na = (struct iactl_na *)iac;
	struct statefuladdr *sa;
	u_int32_t base = DHCP6_DURATION_INFINITE, pltime, passed;
	time_t now;

	/* Determine the smallest period until pltime expires. */
	now = time(NULL);
	for (sa = TAILQ_FIRST(&iac_na->statefuladdr_head); sa;
	    sa = TAILQ_NEXT(sa, link)) {
		passed = now > sa->updatetime ?
		    (u_int32_t)(now - sa->updatetime) : 0;
		pltime = sa->addr.pltime > passed ?
		    sa->addr.pltime - passed : 0;

		if (base == DHCP6_DURATION_INFINITE || pltime < base)
			base = pltime;
	}

	return (base);
}

static void
cleanup_addr(iac)
	struct iactl *iac;
{
	struct iactl_na *iac_na = (struct iactl_na *)iac;
	struct statefuladdr *sa;

	while ((sa = TAILQ_FIRST(&iac_na->statefuladdr_head)) != NULL) {
		TAILQ_REMOVE(&iac_na->statefuladdr_head, sa, link);
		remove_addr(sa);
	}

	free(iac);
}

static int
renew_addr(iac, iaparam, evdp, evd)
	struct iactl *iac;
	struct dhcp6_ia *iaparam;
	struct dhcp6_eventdata **evdp, *evd;
{
	struct iactl_na *iac_na = (struct iactl_na *)iac;
	struct statefuladdr *sa;
	struct dhcp6_list *ial = NULL, pl;

	TAILQ_INIT(&pl);
	for (sa = TAILQ_FIRST(&iac_na->statefuladdr_head); sa;
	    sa = TAILQ_NEXT(sa, link)) {
		if (dhcp6_add_listval(&pl, DHCP6_LISTVAL_STATEFULADDR6,
		    &sa->addr, NULL) == NULL)
			goto fail;
	}

	if ((ial = malloc(sizeof(*ial))) == NULL)
		goto fail;
	TAILQ_INIT(ial);
	if (dhcp6_add_listval(ial, DHCP6_LISTVAL_IANA, iaparam, &pl) == NULL)
		goto fail;
	dhcp6_clear_list(&pl);

	evd->type = DHCP6_EVDATA_IANA;
	evd->data = (void *)ial;
	evd->privdata = (void *)evdp;
	evd->destructor = na_renew_data_free;

	return (0);

  fail:
	dhcp6_clear_list(&pl);
	if (ial)
		free(ial);
	return (-1);
}

static void
na_renew_data_free(evd)
	struct dhcp6_eventdata *evd;
{
	struct dhcp6_list *ial;

	if (evd->type != DHCP6_EVDATA_IANA) {
		dprintf2(LOG_ERR, FNAME, "assumption failure");
		exit(1);
	}

	if (evd->privdata)
		*(struct dhcp6_eventdata **)evd->privdata = NULL;
	ial = (struct dhcp6_list *)evd->data;
	dhcp6_clear_list(ial);
	free(ial);
}

static struct dhcp6_timer *
addr_timo(arg)
	void *arg;
{
	struct statefuladdr *sa = (struct statefuladdr *)arg;
	struct ia *ia;
	void (*callback)__P((struct ia *));

	dprintf2(LOG_DEBUG, FNAME, "address timeout for %s",
	    in6addr2str(&sa->addr.addr, 0));

	ia = sa->ctl->iacna_ia;
	callback = sa->ctl->iacna_callback;

	if (sa->timer)
		dhcp6_remove_timer(&sa->timer);

	remove_addr(sa);

	unlink("/tmp/iana"); /* Foxconn Bernie added, 2014/11/27 */

	(*callback)(ia);

	return (NULL);
}

/* Foxconn Bernie modified start, 2014/11/27 */
static int
na_ifaddrconf(cmd, sa, plen) 
	ifaddrconf_cmd_t cmd;
	struct statefuladdr *sa;
	int plen;
{
	struct dhcp6_statefuladdr *addr;
	struct sockaddr_in6 sin6;
	FILE *fp_iana = NULL;
	u_int32_t iaid;
	u_int32_t t1;
	u_int32_t t2;

	addr = &sa->addr;
	memset(&sin6, 0, sizeof(sin6));
	sin6.sin6_family = AF_INET6;
#ifdef HAVE_SA_LEN
	sin6.sin6_len = sizeof(sin6);
#endif
	sin6.sin6_addr = addr->addr;
//Write to File,Bernie
	ianaInfo.plen = plen;
	//printf("\n%s,%d,iaaddr=%s pltime=%lu vltime=%lu, plen=%d\n",__FUNCTION__,__LINE__,ianaInfo.iaaddr,ianaInfo.pltime,ianaInfo.vltime,ianaInfo.plen);
	iaid = sa->ctl->iacna_ia->conf->iaid;
	t1 = sa->ctl->iacna_ia->t1;
	t2 = sa->ctl->iacna_ia->t2;
        if(cmd == IFADDRCONF_ADD && (fp_iana = fopen("/tmp/iana","w")) != NULL){
                fprintf(fp_iana,
		        "%s\n"
                        "%d\n"
                        "%u\n"
                        "%u\n"
                        "%u\n"
			"%u\n"
			"%u\n"
		,addr2str((struct sockaddr *)&sin6), plen, iaid, t1, t2, addr->pltime, addr->vltime);
		fclose(fp_iana);
	}

		return (ifaddrconf(cmd, sa->dhcpif->ifname, &sin6, plen,
	    		addr->pltime, addr->vltime));
}
/* Foxconn Bernie modified end, 2014/11/27 */
