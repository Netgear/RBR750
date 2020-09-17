/*	$KAME: prefixconf.c,v 1.33 2005/09/16 11:30:15 suz Exp $	*/

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

#include "dhcp6.h"
#include "config.h"
#include "common.h"
#include "timer.h"
#include "dhcp6c_ia.h"
#include "prefixconf.h"

#include <stdlib.h>
#include <time.h>
#include <arpa/inet.h>


static struct siteprefix *find_siteprefix __P((struct siteprefix_list *,
    struct dhcp6_prefix *, int));
static int isvalid __P((struct iactl *));
static u_int32_t duration __P((struct iactl *));
static void cleanup __P((struct iactl *));
static int renew_prefix __P((struct iactl *, struct dhcp6_ia *,
    struct dhcp6_eventdata **, struct dhcp6_eventdata *));
static void renew_data_free __P((struct dhcp6_eventdata *));

static struct dhcp6_timer *siteprefix_timo __P((void *));

static int add_ifprefix __P((struct siteprefix *,
    struct dhcp6_prefix *, struct prefix_ifconf *));

extern struct dhcp6_timer *client6_timo __P((void *));
static int pd_ifaddrconf __P((ifaddrconf_cmd_t, struct dhcp6_ifprefix *ifpfx));

/* Foxconn Bernie added start, 2014/11/27 */
#define DHCP_PD	"/tmp/dhcp_pd"
extern struct iana_information ianaInfo;
extern struct iapd_information iapdInfo;

void copy_mac_to_eui64(char *mac, char *ifid)
{
	ifid[8] = mac[0];
	ifid[8] ^= 0x02; /* reverse the u/l bit*/
	ifid[9] = mac[1];
	ifid[10] = mac[2];
	ifid[11] = 0xff;
	ifid[12] = 0xfe;
	ifid[13] = mac[3];
	ifid[14] = mac[4];
	ifid[15] = mac[5];
}
/* Foxconn Bernie added end, 2014/11/27 */

int
update_prefix(ia, pinfo, pifc, dhcpifp, ctlp, callback)
	struct ia *ia;
	struct dhcp6_prefix *pinfo;
	struct pifc_list *pifc;
	struct dhcp6_if *dhcpifp;
	struct iactl **ctlp;
	void (*callback)__P((struct ia *));
{
	struct iactl_pd *iac_pd = (struct iactl_pd *)*ctlp;
	struct siteprefix *sp;
	struct prefix_ifconf *pif;
	int spcreate = 0;
	struct timeval timo;

	/*
	 * A client discards any addresses for which the preferred
         * lifetime is greater than the valid lifetime.
	 * [RFC3315 22.6] 
	 */
	if (pinfo->vltime != DHCP6_DURATION_INFINITE &&
	    (pinfo->pltime == DHCP6_DURATION_INFINITE ||
	    pinfo->pltime > pinfo->vltime)) {
		dprintf(LOG_INFO, FNAME, "invalid prefix %s/%d: "
		    "pltime (%lu) is larger than vltime (%lu)",
		    in6addr2str(&pinfo->addr, 0), pinfo->plen,
		    pinfo->pltime, pinfo->vltime);
		return (-1);
	}

	if (iac_pd == NULL) {
		if ((iac_pd = malloc(sizeof(*iac_pd))) == NULL) {
			dprintf(LOG_NOTICE, FNAME, "memory allocation failed");
			return (-1);
		}
		memset(iac_pd, 0, sizeof(*iac_pd));
		iac_pd->iacpd_ia = ia;
		iac_pd->iacpd_callback = callback;
		iac_pd->iacpd_isvalid = isvalid;
		iac_pd->iacpd_duration = duration;
		iac_pd->iacpd_cleanup = cleanup;
		iac_pd->iacpd_renew_data =
		    iac_pd->iacpd_rebind_data =
		    iac_pd->iacpd_release_data =
		    iac_pd->iacpd_reestablish_data = renew_prefix;

		iac_pd->pifc_head = pifc;
		TAILQ_INIT(&iac_pd->siteprefix_head);
		*ctlp = (struct iactl *)iac_pd;
	}

	/* search for the given prefix, and make a new one if it fails */
	if ((sp = find_siteprefix(&iac_pd->siteprefix_head, pinfo, 1)) == NULL) {
		if ((sp = malloc(sizeof(*sp))) == NULL) {
			dprintf(LOG_NOTICE, FNAME, "memory allocation failed");
			return (-1);
		}
		memset(sp, 0, sizeof(*sp));
		sp->prefix.addr = pinfo->addr;
		sp->prefix.plen = pinfo->plen;
		sp->ctl = iac_pd;
		TAILQ_INIT(&sp->ifprefix_list);

		TAILQ_INSERT_TAIL(&iac_pd->siteprefix_head, sp, link);

		spcreate = 1;
	}

	/* update the timestamp of update */
	sp->updatetime = time(NULL);

	/* update the prefix according to pinfo */
	sp->prefix.pltime = pinfo->pltime;
	sp->prefix.vltime = pinfo->vltime;
	dprintf(LOG_DEBUG, FNAME, "%s a prefix %s/%d pltime=%lu, vltime=%lu",
	    spcreate ? "create" : "update",
	    in6addr2str(&pinfo->addr, 0), pinfo->plen,
	    pinfo->pltime, pinfo->vltime);

	/* update prefix interfaces if necessary */
	if (sp->prefix.vltime != 0 && spcreate) {
		for (pif = TAILQ_FIRST(iac_pd->pifc_head); pif;
		    pif = TAILQ_NEXT(pif, link)) {
			/*
			 * The requesting router MUST NOT assign any delegated
			 * prefixes or subnets from the delegated prefix(es) to
			 * the link through which it received the DHCP message
			 * from the delegating router.
			 * [RFC3633 Section 12.1]
			 */
			if (strcmp(pif->ifname, dhcpifp->ifname) == 0) {
				dprintf(LOG_INFO, FNAME,
				    "skip %s as a prefix interface",
				    dhcpifp->ifname);
				continue;
			}

			add_ifprefix(sp, pinfo, pif);
		}
	}

	/*
	 * If the new vltime is 0, this prefix immediately expires.
	 * Otherwise, set up or update the associated timer.
	 */
	switch (sp->prefix.vltime) {
	case 0:
		remove_siteprefix(sp);
		break;
	case DHCP6_DURATION_INFINITE:
		if (sp->timer)
			dhcp6_remove_timer(&sp->timer);
		break;
	default:
		if (sp->timer == NULL) {
			sp->timer = dhcp6_add_timer(siteprefix_timo, sp);
			if (sp->timer == NULL) {
				dprintf(LOG_NOTICE, FNAME,
				    "failed to add prefix timer");
				remove_siteprefix(sp); /* XXX */
				return (-1);
			}
		}
		/* update the timer */
		timo.tv_sec = sp->prefix.vltime;
		timo.tv_usec = 0;

		dhcp6_set_timer(&timo, sp->timer);
		break;
	}

	return (0);
}

static struct siteprefix *
find_siteprefix(head, prefix, match_plen)
	struct siteprefix_list *head;
	struct dhcp6_prefix *prefix;
	int match_plen;
{
	struct siteprefix *sp;

	for (sp = TAILQ_FIRST(head); sp; sp = TAILQ_NEXT(sp, link)) {
		if (!IN6_ARE_ADDR_EQUAL(&sp->prefix.addr, &prefix->addr))
			continue;
		if (match_plen == 0 || sp->prefix.plen == prefix->plen)
			return (sp);
	}

	return (NULL);
}
/* Foxconn Bernie modified start, 2014/11/27 */
extern char dhcp6_interface[16];

void
remove_siteprefix(sp)
	struct siteprefix *sp;
{
	struct dhcp6_ifprefix *ip;
	char cmd[128] = {};
	FILE *fp_dhcppd;
	char buf[200];
	char str_ipv6[200];
	int plen;

	dprintf(LOG_DEBUG, FNAME, "remove a site prefix %s/%d",
	    in6addr2str(&sp->prefix.addr, 0), sp->prefix.plen);

	inet_ntop(AF_INET6, &sp->prefix.addr, str_ipv6, sizeof(str_ipv6));
	plen = sp->prefix.plen;
/* Foxconn Bernie added end, 2014/11/27 */	

	if (sp->timer)
		dhcp6_remove_timer(&sp->timer);

	/* remove all interface prefixes */
	while ((ip = TAILQ_FIRST(&sp->ifprefix_list)) != NULL) {
		TAILQ_REMOVE(&sp->ifprefix_list, ip, plink);
		pd_ifaddrconf(IFADDRCONF_REMOVE, ip);
		free(ip);
	}

/* Foxconn Bernie added start, 2014/11/27 */
	if ( (fp_dhcppd = fopen(DHCP_PD, "r")) != NULL )
	{
		if (fgets(buf, sizeof(buf), fp_dhcppd)) {
			buf[strlen(buf)-1] = '\0';
			if ( strcmp(buf, str_ipv6) != 0 )
				goto exit_if;
		} else
			goto exit_if;
		
		if (fgets(buf, sizeof(buf), fp_dhcppd)) {
			buf[strlen(buf)-1] = '\0';
			if ( atoi(buf) != plen )
				goto exit_if;
		} else
			goto exit_if;

		sprintf(cmd, "ip -6 route add %s/%d dev br0", str_ipv6, plen);
		system(cmd);

		/* Foxconn Bernie added start, 2014/12/26 */
		char tempcommand[1024];
		memset(tempcommand, 0, sizeof(tempcommand));
		sprintf(tempcommand, "nvram set ipv6_code5_oldpd=\"%s/%d\"", str_ipv6, plen);
		if (strlen(tempcommand)) {
			//printf("\ntempcommand=%s\n",tempcommand);
			system(tempcommand);
			memset(tempcommand, 0, sizeof(tempcommand));
		}
		/* Foxconn Bernie added end, 2014/12/26 */

		/* Remove PD, stop LAN */
		unlink(DHCP_PD);
		char command[256];
		sprintf(command, "dhcp6c_down %s", dhcp6_interface);
		printf("\nRemove PD, stop LAN, command=%s\n",command);
		system(command);		

		
exit_if:
		fclose(fp_dhcppd);
	}
/* Foxconn Bernie added end, 2014/11/27 */	
	TAILQ_REMOVE(&sp->ctl->siteprefix_head, sp, link);
	free(sp);
}

static int
isvalid(iac)
	struct iactl *iac;
{
	struct iactl_pd *iac_pd = (struct iactl_pd *)iac;

	if (TAILQ_EMPTY(&iac_pd->siteprefix_head))
		return (0);	/* this IA is invalid */
	return (1);
}

static u_int32_t
duration(iac)
	struct iactl *iac;
{
	struct iactl_pd *iac_pd = (struct iactl_pd *)iac;
	struct siteprefix *sp;
	u_int32_t base = DHCP6_DURATION_INFINITE, pltime, passed;
	time_t now;

	/* Determine the smallest period until pltime expires. */
	now = time(NULL);
	for (sp = TAILQ_FIRST(&iac_pd->siteprefix_head); sp;
	    sp = TAILQ_NEXT(sp, link)) {
		passed = now > sp->updatetime ?
		    (u_int32_t)(now - sp->updatetime) : 0;
		pltime = sp->prefix.pltime > passed ?
		    sp->prefix.pltime - passed : 0;

		if (base == DHCP6_DURATION_INFINITE || pltime < base)
			base = pltime;
	}

	return (base);
}

static void
cleanup(iac)
	struct iactl *iac;
{
	struct iactl_pd *iac_pd = (struct iactl_pd *)iac;
	struct siteprefix *sp;

	while ((sp = TAILQ_FIRST(&iac_pd->siteprefix_head)) != NULL) {
		TAILQ_REMOVE(&iac_pd->siteprefix_head, sp, link);
		remove_siteprefix(sp);
	}

	free(iac);
}

static int
renew_prefix(iac, iaparam, evdp, evd)
	struct iactl *iac;
	struct dhcp6_ia *iaparam;
	struct dhcp6_eventdata **evdp, *evd;
{
	struct iactl_pd *iac_pd = (struct iactl_pd *)iac;
	struct siteprefix *sp;
	struct dhcp6_list *ial = NULL, pl;

	TAILQ_INIT(&pl);
	for (sp = TAILQ_FIRST(&iac_pd->siteprefix_head); sp;
	    sp = TAILQ_NEXT(sp, link)) {
		if (dhcp6_add_listval(&pl, DHCP6_LISTVAL_PREFIX6,
		    &sp->prefix, NULL) == NULL)
			goto fail;
	}

	if ((ial = malloc(sizeof(*ial))) == NULL)
		goto fail;
	TAILQ_INIT(ial);
	if (dhcp6_add_listval(ial, DHCP6_LISTVAL_IAPD, iaparam, &pl) == NULL)
		goto fail;
	dhcp6_clear_list(&pl);

	evd->type = DHCP6_EVDATA_IAPD;
	evd->data = (void *)ial;
	evd->privdata = (void *)evdp;
	evd->destructor = renew_data_free;

	return (0);

  fail:
	dhcp6_clear_list(&pl);
	if (ial)
		free(ial);
	return (-1);
}

static void
renew_data_free(evd)
	struct dhcp6_eventdata *evd;
{
	struct dhcp6_list *ial;

	if (evd->type != DHCP6_EVDATA_IAPD) {
		dprintf(LOG_ERR, FNAME, "assumption failure");
		exit(1);
	}

	if (evd->privdata)
		*(struct dhcp6_eventdata **)evd->privdata = NULL;
	ial = (struct dhcp6_list *)evd->data;
	dhcp6_clear_list(ial);
	free(ial);
}

static struct dhcp6_timer *
siteprefix_timo(arg)
	void *arg;
{
	struct siteprefix *sp = (struct siteprefix *)arg;
	struct ia *ia;
	void (*callback)__P((struct ia *));

	dprintf(LOG_DEBUG, FNAME, "prefix timeout for %s/%d",
	    in6addr2str(&sp->prefix.addr, 0), sp->prefix.plen);

	ia = sp->ctl->iacpd_ia;
	callback = sp->ctl->iacpd_callback;

	if (sp->timer)
		dhcp6_remove_timer(&sp->timer);

	remove_siteprefix(sp);

	(*callback)(ia);

	return (NULL);
}

static int
add_ifprefix(siteprefix, prefix, pconf)
	struct siteprefix *siteprefix;
	struct dhcp6_prefix *prefix;
	struct prefix_ifconf *pconf;
{
	struct dhcp6_ifprefix *ifpfx = NULL;
	struct in6_addr *a;
	u_long sla_id;
	char *sp;
	int b, i;

	if ((ifpfx = malloc(sizeof(*ifpfx))) == NULL) {
		dprintf(LOG_NOTICE, FNAME,
		    "failed to allocate memory for ifprefix");
		return (-1);
	}
	memset(ifpfx, 0, sizeof(*ifpfx));

	ifpfx->ifconf = pconf;

	ifpfx->paddr.sin6_family = AF_INET6;
#ifdef HAVE_SA_LEN
	ifpfx->paddr.sin6_len = sizeof(struct sockaddr_in6);
#endif
	ifpfx->paddr.sin6_addr = prefix->addr;
/* Foxconn Bernie added start, 2014/11/27 */	
	/*
		Description: generate dhcp_pd file.
	*/
	char str_ipv6[50];
	char str_buf[200];
	struct in6_addr *p_in6;
	char str_duid[260];
	size_t duid_len;
	char *duid_id;
	int tmp_len=0;
	u_int32_t iaid;
	FILE *fp_dhcppd;
	u_int32_t t1;
	u_int32_t t2;
	
	p_in6 = &(prefix->addr);
	inet_ntop(AF_INET6, p_in6->s6_addr, str_ipv6, sizeof(str_ipv6));
	
	sprintf(str_buf, "ip -6 route flush root 2000::/3 dev lo");
	system(str_buf);
	
	// if length of DHCP-PD < 64, add a NULL Route to loopback
	if (prefix->plen < 64) {
		sprintf(str_buf,"ip -6 route add %s/%d dev lo", str_ipv6, prefix->plen);
		system(str_buf);
	}
	// added server duid in dhcp_pd
	iaid = siteprefix->ctl->iacpd_ia->conf->iaid;
	t1 = siteprefix->ctl->iacpd_ia->t1;
	t2 = siteprefix->ctl->iacpd_ia->t2;
	duid_len = siteprefix->ctl->iacpd_ia->serverid.duid_len;
	duid_id  = siteprefix->ctl->iacpd_ia->serverid.duid_id;
	str_duid[0] = '\0';
	if (duid_len>128) // if duid_len>128 , only get 128 bytes
		duid_len = 128;
	for (i=0,tmp_len=0; i<duid_len; i++)
		tmp_len += sprintf(str_duid+tmp_len, "%02x", (unsigned char)duid_id[i]);
	str_duid[tmp_len]='\0';

	if((fp_dhcppd=fopen(DHCP_PD,"w"))!=NULL){
		fprintf(fp_dhcppd,
			"%s\n"  // dhcp_pd: prefix
			"%d\n"  // dhcp_pd: prefix_len
			"%u\n"  // dhcp_pd: iaid
			"%u\n"  // dhcp_pd: T1
			"%u\n"  // dhcp_pd: T2
			"%u\n"  // dhcp_pd: prefer lifetime
			"%u\n", // dhcp_pd: valid lifetime
			str_ipv6, prefix->plen, iaid, t1, t2, prefix->pltime, prefix->vltime);
		fclose(fp_dhcppd);
	}
	if (prefix->plen > 64)   prefix->plen = 64;
	if (prefix->plen <= 64)  pconf->sla_len = 64-prefix->plen;
/* Foxconn Bernie added end, 2014/11/27 */
	ifpfx->plen = prefix->plen + pconf->sla_len;
	/*
	 * XXX: our current implementation assumes ifid len is a multiple of 8
	 */
	if ((pconf->ifid_len % 8) != 0) {
		dprintf(LOG_ERR, FNAME,
		    "assumption failure on the length of interface ID");
		goto bad;
	}
	if (ifpfx->plen + pconf->ifid_len < 0 ||
	    ifpfx->plen + pconf->ifid_len > 128) {
		dprintf(LOG_INFO, FNAME,
			"invalid prefix length %d + %d + %d",
			prefix->plen, pconf->sla_len, pconf->ifid_len);
		goto bad;
	}

	/* copy prefix and SLA ID */
	a = &ifpfx->paddr.sin6_addr;
	b = prefix->plen;
	for (i = 0, b = prefix->plen; b > 0; b -= 8, i++)
		a->s6_addr[i] = prefix->addr.s6_addr[i];
	/* Foxconn Bernie modified start, 2014/11/27 @ If sla_len=0 don't copy sla_id*/
	if (pconf->sla_len > 0) {
	sla_id = htonl(pconf->sla_id);
	sp = ((char *)&sla_id + 3);
	i = (128 - pconf->ifid_len) / 8;
	for (b = pconf->sla_len; b > 7; b -= 8, sp--)
		a->s6_addr[--i] = *sp;
	if (b)
		a->s6_addr[--i] |= *sp;
	}
	/* Foxconn Bernie modified end, 2014/11/27 */
	/* configure the corresponding address */
	ifpfx->ifaddr = ifpfx->paddr;
/* Foxconn Bernie added start, 2014/11/27 @ send this infomation to acos and grenate dhcp pd file*/
/* origin code:
	for (i = 15; i >= pconf->ifid_len / 8; i--)
		ifpfx->ifaddr.sin6_addr.s6_addr[i] = pconf->ifid[i];
	if (pd_ifaddrconf(IFADDRCONF_ADD, ifpfx))
		goto bad;
*/		
		
	char mac[6];
	get_macaddr(pconf->ifname, mac);
	copy_mac_to_eui64(mac, ifpfx->ifaddr.sin6_addr.s6_addr);		

	{
		char str_ipv62[50];
		char iapd_prefix[64]={0};
		char command[128]={0};
		sprintf(str_ipv62,"%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x",ifpfx->ifaddr.sin6_addr.s6_addr[0],ifpfx->ifaddr.sin6_addr.s6_addr[1],ifpfx->ifaddr.sin6_addr.s6_addr[2],ifpfx->ifaddr.sin6_addr.s6_addr[3],ifpfx->ifaddr.sin6_addr.s6_addr[4],ifpfx->ifaddr.sin6_addr.s6_addr[5],ifpfx->ifaddr.sin6_addr.s6_addr[6],ifpfx->ifaddr.sin6_addr.s6_addr[7],ifpfx->ifaddr.sin6_addr.s6_addr[8],ifpfx->ifaddr.sin6_addr.s6_addr[9],ifpfx->ifaddr.sin6_addr.s6_addr[10],ifpfx->ifaddr.sin6_addr.s6_addr[11],ifpfx->ifaddr.sin6_addr.s6_addr[12],ifpfx->ifaddr.sin6_addr.s6_addr[13],ifpfx->ifaddr.sin6_addr.s6_addr[14],ifpfx->ifaddr.sin6_addr.s6_addr[15]);
		FILE *fp=NULL;
		fp = fopen ( "tmp/iapdAcos" , "w+" );
		if(fp)
		{
			    sprintf(iapd_prefix,"%s",str_ipv6);
			    sprintf(command,"nvram set ipv6_pd_prefix=\"%s\"",iapd_prefix);
			    system(command);
				fprintf(fp,"v6str=%s prefix=%s plen=%d iaid=%u dlen=%d sduid=%s ptime=%u vtime=%u", str_ipv62,str_ipv6,prefix->plen,iaid,duid_len, str_duid,prefix->pltime, prefix->vltime);
			  fclose(fp);
		}

		memset(&iapdInfo,0,sizeof(iapdInfo));
		if(str_ipv62 != NULL)
		{
			strcpy(iapdInfo.v6str, str_ipv62);
		}
		if(str_ipv6 != NULL)
		{
			strcpy(iapdInfo.prefix, str_ipv6);
		}
		iapdInfo.plen = prefix->plen;
		iapdInfo.iaid = iaid;
		iapdInfo.duid_len = duid_len;
		if(str_duid != NULL)
		{
			strcpy(iapdInfo.duid, str_duid);
		}		
		iapdInfo.pltimePD= prefix->pltime;
		iapdInfo.vltimePD= prefix->vltime;		
		//printf("\nSave IAPD info,v6str=%s prefix=%s plen=%d iaid=%u dlen=%d sduid=%s ptime=%u vtime=%u\n", iapdInfo.v6str,iapdInfo.prefix,iapdInfo.plen,iapdInfo.iaid,iapdInfo.duid_len,iapdInfo.duid, iapdInfo.pltimePD,iapdInfo.vltimePD);
		char iapd_valid_lifetime_cmd_buf[1024];
		char iapd_preferred_lifetime_cmd_buf[1024];
		sprintf(iapd_valid_lifetime_cmd_buf,
				"nvram set RA_AdvValidLifetime_from_IAPD=%u",
				iapdInfo.vltimePD);
		sprintf(iapd_preferred_lifetime_cmd_buf,
				"nvram set RA_AdvPreferredLifetime_from_IAPD=%u",
				iapdInfo.pltimePD);	
		system(iapd_valid_lifetime_cmd_buf);
		system(iapd_preferred_lifetime_cmd_buf);
		system("nvram set RA_use_dynamic_lifetime=1");		

	}
/* Foxconn Bernie added end, 2014/11/27 */	
	/* TODO: send a control message for other processes */

	TAILQ_INSERT_TAIL(&siteprefix->ifprefix_list, ifpfx, plink);

	return (0);

  bad:
	if (ifpfx)
		free(ifpfx);
	return (-1);
}

#ifndef ND6_INFINITE_LIFETIME
#define ND6_INFINITE_LIFETIME 0xffffffff
#endif

static int
pd_ifaddrconf(cmd, ifpfx)
	ifaddrconf_cmd_t cmd;
	struct dhcp6_ifprefix *ifpfx;
{
	struct prefix_ifconf *pconf;

	pconf = ifpfx->ifconf;
	return (ifaddrconf(cmd, pconf->ifname, &ifpfx->ifaddr, ifpfx->plen, 
	    ND6_INFINITE_LIFETIME, ND6_INFINITE_LIFETIME));
}
