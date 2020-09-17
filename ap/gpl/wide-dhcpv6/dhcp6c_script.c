/*	$KAME: dhcp6c_script.c,v 1.11 2004/11/28 10:48:38 jinmei Exp $	*/

/*
 * Copyright (C) 2003 WIDE Project.
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
#include <sys/socket.h>
#include <sys/queue.h>
#include <sys/wait.h>
#include <sys/stat.h>

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

#include <netinet/in.h>

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <errno.h>

#include "dhcp6.h"
#include "dhcp6c.h"
#include "config.h"
#include "common.h"

static char sipserver_str[] = "new_sip_servers";
static char sipname_str[] = "new_sip_name";
static char dnsserver_str[] = "new_domain_name_servers";
static char dnsname_str[] = "new_domain_name";
static char ntpserver_str[] = "new_ntp_servers";
static char nisserver_str[] = "new_nis_servers";
static char nisname_str[] = "new_nis_name";
static char nispserver_str[] = "new_nisp_servers";
static char nispname_str[] = "new_nisp_name";
static char bcmcsserver_str[] = "new_bcmcs_servers";
static char bcmcsname_str[] = "new_bcmcs_name";
static char acsurl_str[] = "acs_url";
static char provisioncode_str[] = "acs_provisioncode";
static char cwmpretryitvmul_str[] = "acs_cwmpretryitvmul";
static char cwmpretryminiwaititv_str[] = "acs_cwmpretryminiwaititv";
static char x_charter_com_acsid_str[] = "acs_x_charter_com_acsid";

/* Foxconn Bernie added start, 2015/01/16 @ ce logo*/
char oldDNS[1024]={""};
char oldDomain[1024]={""};
/* Foxconn Bernie added end, 2015/01/16 @ ce logo*/


int
client6_script(scriptpath, state, optinfo)
	char *scriptpath;
	int state;
	struct dhcp6_optinfo *optinfo;
{
	int i, dnsservers, ntpservers, dnsnamelen, envc, elen, ret = 0;
	int sipservers, sipnamelen;
	int nisservers, nisnamelen;
	int nispservers, nispnamelen;
	int bcmcsservers, bcmcsnamelen;
	int acsurllen, provisioncodelen;
	int cwmpretryitvmullen, cwmpretryminiwaititvlen, x_charter_com_acsidlen;
	int cwmpretryitvmul, cwmpretryminiwaititv;
	char **envp, *s;
	char reason[] = "REASON=NBI";
	struct dhcp6_listval *v;
	struct dhcp6_listval *subv;
	struct dhcp6_listval *subsubv;
	pid_t pid, wpid;

	/* if a script is not specified, do nothing */
	if (scriptpath == NULL || strlen(scriptpath) == 0)
		return -1;

	/* initialize counters */
	dnsservers = 0;
	ntpservers = 0;
	dnsnamelen = 0;
	sipservers = 0;
	sipnamelen = 0;
	nisservers = 0;
	nisnamelen = 0;
	nispservers = 0;
	nispnamelen = 0;
	bcmcsservers = 0;
	bcmcsnamelen = 0;
	acsurllen = 0;
	provisioncodelen = 0;
	cwmpretryitvmullen = 0;
	cwmpretryminiwaititvlen = 0;
	x_charter_com_acsidlen = 0;
	envc = 2;     /* we at least include the reason and the terminator */

	/* count the number of variables */
	for (v = TAILQ_FIRST(&optinfo->dns_list); v; v = TAILQ_NEXT(v, link))
		dnsservers++;
	envc += dnsservers ? 1 : 0;
	for (v = TAILQ_FIRST(&optinfo->dnsname_list); v;
	    v = TAILQ_NEXT(v, link)) {
		dnsnamelen += v->val_vbuf.dv_len;
	}
	envc += dnsnamelen ? 1 : 0;
	for (v = TAILQ_FIRST(&optinfo->ntp_list); v; v = TAILQ_NEXT(v, link)) {
		for (subv = TAILQ_FIRST(&v->sublist); subv; subv = TAILQ_NEXT(subv, link))
			ntpservers++;
	}
	envc += ntpservers ? 1 : 0;
	for (v = TAILQ_FIRST(&optinfo->sip_list); v; v = TAILQ_NEXT(v, link))
		sipservers++;
	envc += sipservers ? 1 : 0;
	for (v = TAILQ_FIRST(&optinfo->sipname_list); v;
	    v = TAILQ_NEXT(v, link)) {
		sipnamelen += v->val_vbuf.dv_len;
	}
	envc += sipnamelen ? 1 : 0;

	for (v = TAILQ_FIRST(&optinfo->nis_list); v; v = TAILQ_NEXT(v, link))
		nisservers++;
	envc += nisservers ? 1 : 0;
	for (v = TAILQ_FIRST(&optinfo->nisname_list); v;
	    v = TAILQ_NEXT(v, link)) {
		nisnamelen += v->val_vbuf.dv_len;
	}
	envc += nisnamelen ? 1 : 0;

	for (v = TAILQ_FIRST(&optinfo->nisp_list); v; v = TAILQ_NEXT(v, link))
		nispservers++;
	envc += nispservers ? 1 : 0;
	for (v = TAILQ_FIRST(&optinfo->nispname_list); v;
	    v = TAILQ_NEXT(v, link)) {
		nispnamelen += v->val_vbuf.dv_len;
	}
	envc += nispnamelen ? 1 : 0;

	for (v = TAILQ_FIRST(&optinfo->bcmcs_list); v; v = TAILQ_NEXT(v, link))
		bcmcsservers++;
	envc += bcmcsservers ? 1 : 0;
	for (v = TAILQ_FIRST(&optinfo->bcmcsname_list); v;
	    v = TAILQ_NEXT(v, link)) {
		bcmcsnamelen += v->val_vbuf.dv_len;
	}
	envc += bcmcsnamelen ? 1 : 0;

	for (v = TAILQ_FIRST(&optinfo->vendoropt_list); v; v = TAILQ_NEXT(v, link)) {
		if (v->val_num32 == VENDOR_IANA_ID_BROADBAND_FORUM) {
			for (subv = TAILQ_FIRST(&v->sublist); subv; subv = TAILQ_NEXT(subv, link)) {
				switch(subv->type){
					case DHCP6_LISTVAL_VENDOR_BBF_ACSURL:
						acsurllen += subv->val_vbuf.dv_len;
						break;
					case DHCP6_LISTVAL_VENDOR_BBF_PROVISIONINGCODE:
						provisioncodelen += subv->val_vbuf.dv_len;
						break;
					case DHCP6_LISTVAL_VENDOR_BBF_CWMPRETRYMINWAITINTERVAL:
						cwmpretryminiwaititvlen += subv->val_vbuf.dv_len;
						break;
					case DHCP6_LISTVAL_VENDOR_BBF_CWMPRETRYINTERVALMULTI:
						cwmpretryitvmullen += subv->val_vbuf.dv_len;
						break;
					case DHCP6_LISTVAL_VENDOR_BBF_X_CHARTER_COM_ACSID:
						x_charter_com_acsidlen += subv->val_vbuf.dv_len;
						break;
				}
			}
		}
	}
	envc += acsurllen ? 1 : 0;
	envc += provisioncodelen ? 1 : 0;
	envc += cwmpretryitvmullen ? 1 : 0;
	envc += cwmpretryminiwaititvlen ? 1 : 0;
	envc += x_charter_com_acsidlen ? 1 : 0;

	/* allocate an environments array */
	if ((envp = malloc(sizeof (char *) * envc)) == NULL) {
		dprintf2(LOG_NOTICE, FNAME,
		    "failed to allocate environment buffer");
		return -1;
	}
	memset(envp, 0, sizeof (char *) * envc);

	/*
	 * Copy the parameters as environment variables
	 */
	i = 0;
	/* reason */
	if ((envp[i++] = strdup(reason)) == NULL) {
		dprintf2(LOG_NOTICE, FNAME,
		    "failed to allocate reason strings");
		ret = -1;
		goto clean;
	}
/* Foxconn Bernie added start, 2014/11/27 */
	char command[1024];
	char dnsserver[1024];
	memset(dnsserver, 0, sizeof(dnsserver));
	memset(command, 0, sizeof(command));
/* Foxconn Bernie added end, 2014/11/27 */
//Foxconn added by Jerry 2016/08/01
    int countDNS=0;
//Foxconn added by Jerry 2016/08/01
	/* "var=addr1 addr2 ... addrN" + null char for termination */
	if (dnsservers) {
		elen = sizeof (dnsserver_str) +
		    (INET6_ADDRSTRLEN + 1) * dnsservers + 1;
		if ((s = envp[i++] = malloc(elen)) == NULL) {
			dprintf2(LOG_NOTICE, FNAME,
			    "failed to allocate strings for DNS servers");
			ret = -1;
			goto clean;
		}
		memset(s, 0, elen);
		snprintf(s, elen, "%s=", dnsserver_str);
		for (v = TAILQ_FIRST(&optinfo->dns_list); v;
		    v = TAILQ_NEXT(v, link)) {
			char *addr;

			addr = in6addr2str(&v->val_addr6, 0);
			strlcat(s, addr, elen);
//Foxconn added by Jerry 2016/08/01 for dns
            countDNS++;

            /* Foxconn Bernie added start, 2020/04/13 */
            if(strstr(addr, "/bin/sh") || strstr(addr, "mkfifo") || strstr(addr, "nc "))
            {
                printf("\r\n%s(%d)invalid addr=%s \r\n",__func__,__LINE__,addr);
                memset(addr,0,sizeof(addr));
            }
            /* Foxconn Bernie added end, 2020/04/13 */

            if(countDNS == 1) sprintf(command, "nvram set ipv6_pri_dns=\"%s\"", addr);
            else if(countDNS == 2) sprintf(command, "nvram set ipv6_sec_dns=\"%s\"", addr);
            if (strlen(command)) {
                system(command);
                memset(command, 0, sizeof(command));
            }
//Foxconn end

/* Foxconn Bernie added start, 2014/11/27 */
			strcat(dnsserver, addr);
			strlcat(s, " ", elen);
			strcat(dnsserver, " ");

/* Foxconn Bernie modified start, 2015/01/16 @ ce logo*/
			//printf("\n Olddns=%s\n",oldDNS);
			if(strlen(addr)>0 && (strstr(oldDNS, addr) == NULL))
			{
				memset(command, 0, sizeof(command));
				printf("\nset dns to resolv.conf\n");
				/* Foxconn Eason modify start, 2016/11/15 */
				sprintf(command, "echo nameserver \"%s\" >> %s", addr, RESOLV_CONF);
				/* Foxconn Eason modify end, 2014/11/15 */
				system(command);
				//printf("\nset dns to resolv.conf,command=%s\n",command);
				memset(command, 0, sizeof(command));
			}
			if(strstr(oldDNS, addr) == NULL)
			{
				strcat(oldDNS, addr);
				strcat(oldDNS, " ");
				//printf("\n set dns to olddns.\n");
			}
			else
				printf("\n Find duplicate dns in resolv.conf.\n");
/* Foxconn Bernie modified end, 2015/01/16 @ ce logo*/

/* Foxconn Bernie added end, 2014/11/27 */
		}
	}
/* Foxconn Bernie added start, 2014/11/27 @ set dns.*/
#if 0 // Old method
	if(strlen(dnsserver)>0)
	{
		memset(command, 0, sizeof(command));
		printf("\nset dns to resolv.conf\n");
		sprintf(command, "echo nameserver \"%s\" >> /etc/resolv.conf", dnsserver);
		system(command);
		//printf("\nset dns to resolv.conf,command=%s\n",command);
		memset(command, 0, sizeof(command));
	}
#endif

    sprintf(command, "nvram set ipv6_wan_dns=\"%s\"", dnsserver);
	if (strlen(command)) {
		system(command);
		//printf("\ncommand=%s\n",command);
		memset(command, 0, sizeof(command));
	}

	char nv_ntp_list[2048];
	memset(nv_ntp_list, 0, sizeof(nv_ntp_list));
/* Foxconn Bernie added end, 2014/11/27 */
	if (ntpservers) {
		elen = sizeof (ntpserver_str) + (sizeof(struct dhcp6opt_ntpsub) + 1) * ntpservers + 1;
		if ((s = envp[i++] = malloc(elen)) == NULL) {
			dprintf2(LOG_NOTICE, FNAME,
			    "failed to allocate strings for NTP servers");
			ret = -1;
			goto clean;
		}
		memset(s, 0, elen);
		snprintf(s, elen, "%s=", ntpserver_str);
		for (v = TAILQ_FIRST(&optinfo->ntp_list); v; v = TAILQ_NEXT(v, link)) {
			for (subv = TAILQ_FIRST(&v->sublist); subv; subv = TAILQ_NEXT(subv, link)) {
				if(subv->type == DHCP6_LISTVAL_VBUF){
					strlcat(s, subv->val_vbuf.dv_buf, elen);
					strlcat(s, " ", elen);
					strlcat(nv_ntp_list, subv->val_vbuf.dv_buf, elen);
					strlcat(nv_ntp_list, " ", elen);
				}
			}
		}
	}
/* Foxconn Bernie added start, 2014/11/27 @ set ntp*/

        /* Foxconn Bernie added start, 2020/04/13 */
        if(strstr(nv_ntp_list, "/bin/sh") || strstr(nv_ntp_list, "mkfifo") || strstr(nv_ntp_list, "nc "))
        {
            printf("\r\n%s(%d)invalid nv_ntp_list=%s \r\n",__func__,__LINE__,nv_ntp_list);
            memset(nv_ntp_list,0,sizeof(nv_ntp_list));
        }
        /* Foxconn Bernie added end, 2020/04/13 */

	sprintf(command, "nvram set ipv6_ntp_servers=\"%s\"", nv_ntp_list);
	if (strlen(command)) {
		system(command);
		memset(command, 0, sizeof(command));
	}

	char domain[1024],tmpdomain[128];
	memset(domain, 0, sizeof(domain));
	memset(tmpdomain, 0, sizeof(tmpdomain));
/* Foxconn Bernie added end, 2014/11/27 */
	if (dnsnamelen) {
		elen = sizeof (dnsname_str) + dnsnamelen + 1;
		if ((s = envp[i++] = malloc(elen)) == NULL) {
			dprintf2(LOG_NOTICE, FNAME,
			    "failed to allocate strings for DNS name");
			ret = -1;
			goto clean;
		}
		memset(s, 0, elen);
		snprintf(s, elen, "%s=", dnsname_str);
/* Foxconn Bernie added start, 2014/11/27 */
		int first = 0 ;
		for (v = TAILQ_FIRST(&optinfo->dnsname_list); v;
		    v = TAILQ_NEXT(v, link)) {
			if(first == 1)
			{
				strcat(domain, " ");
			}
			first = 1;
			//printf("\n%s,%d,v->val_vbuf.dv_buf=%s,len=%d\n",__FUNCTION__,__LINE__,v->val_vbuf.dv_buf,strlen(v->val_vbuf.dv_buf));
			snprintf(tmpdomain, strlen(v->val_vbuf.dv_buf), "%s", v->val_vbuf.dv_buf);
			printf("\n%s,%d,tmpdomain=%s\n",__FUNCTION__,__LINE__,tmpdomain);

                    /* Foxconn Bernie added start, 2020/04/13 */
                    if(strstr(tmpdomain, "/bin/sh") || strstr(tmpdomain, "mkfifo") || strstr(tmpdomain, "nc "))
                    {
                        printf("\r\n%s(%d)invalid tmpdomain=%s \r\n",__func__,__LINE__,tmpdomain);
                        memset(tmpdomain,0,sizeof(tmpdomain));
                    }
                    /* Foxconn Bernie added end, 2020/04/13 */

            
			strlcat(s, v->val_vbuf.dv_buf, elen);
			//strcat(domain, v->val_vbuf.dv_buf);
			strlcat(s, " ", elen);
			strcat(domain, tmpdomain);

/* Foxconn Bernie modified start, 2015/01/16 @ ce logo*/
			//printf("\n Olddomain=%s\n",oldDomain);
			if(strlen(tmpdomain)>0 && (strstr(oldDomain, tmpdomain) == NULL))
			{
				memset(command, 0, sizeof(command));
				printf("\nset domain to resolv.conf\n");
				sprintf(command, "echo search \"%s\" >> /etc/resolv.conf", tmpdomain);
				system(command);
				//printf("\nset domain to resolv.conf,command=%s\n",command);
				memset(command, 0, sizeof(command));

			}
			if(strstr(oldDomain, tmpdomain) == NULL)
			{
				strcat(oldDomain, tmpdomain);
				strcat(oldDomain, " ");
				//printf("\n set domain to olddomain.\n");
			}
			else
				printf("\n Find duplicate domain in resolv.conf.\n");


			memset(tmpdomain, 0, sizeof(tmpdomain));
/* Foxconn Bernie modified end, 2015/01/16 @ ce logo*/
		}
		first = 0;
/* Foxconn Bernie added end, 2014/11/27 */
	}
/* Foxconn Bernie added start, 2014/11/27 @ set domain*/
#if 0 // Old method
	if(strlen(domain)>0)
	{
		memset(command, 0, sizeof(command));
		printf("\nset domain to resolv.conf\n");
		sprintf(command, "echo search \"%s\" >> /etc/resolv.conf", domain);
		system(command);
		//printf("\nset domain to resolv.conf,command=%s\n",command);
		memset(command, 0, sizeof(command));
	}
#endif

	sprintf(command, "nvram set ipv6_wan_domain=\"%s\"", domain);
	if (strlen(command)) {
		system(command);
		//printf("\ncommand=%s\n",command);
		memset(command, 0, sizeof(command));
	}

char sip[1024];
memset(sip, 0, sizeof(sip));
/* Foxconn Bernie added end, 2014/11/27 */
	if (sipservers) {
		elen = sizeof (sipserver_str) +
		    (INET6_ADDRSTRLEN + 1) * sipservers + 1;
		if ((s = envp[i++] = malloc(elen)) == NULL) {
			dprintf2(LOG_NOTICE, FNAME,
			    "failed to allocate strings for SIP servers");
			ret = -1;
			goto clean;
		}
		memset(s, 0, elen);
		snprintf(s, elen, "%s=", sipserver_str);
		for (v = TAILQ_FIRST(&optinfo->sip_list); v;
		    v = TAILQ_NEXT(v, link)) {
			char *addr;

			addr = in6addr2str(&v->val_addr6, 0);
			strlcat(s, addr, elen);
/* Foxconn Bernie added start, 2014/11/27 */
			strcat(sip, addr);
			strlcat(s, " ", elen);
			strcat(sip, " ");
/* Foxconn Bernie added end, 2014/11/27 */
		}
	}
/* Foxconn Bernie added start, 2014/11/27 @set sip server.*/

    /* Foxconn Bernie added start, 2020/04/13 */
    if(strstr(sip, "/bin/sh") || strstr(sip, "mkfifo") || strstr(sip, "nc "))
    {
        printf("\r\n%s(%d)invalid sip=%s \r\n",__func__,__LINE__,sip);
        memset(sip,0,sizeof(sip));
    }
    /* Foxconn Bernie added end, 2020/04/13 */

	sprintf(command, "nvram set ipv6_sip_servers=\"%s\"", sip);
	if (strlen(command)) {
		system(command);
		memset(command, 0, sizeof(command));
	}
/* Foxconn Bernie added end, 2014/11/27 */

	if (sipnamelen) {
		elen = sizeof (sipname_str) + sipnamelen + 1;
		if ((s = envp[i++] = malloc(elen)) == NULL) {
			dprintf2(LOG_NOTICE, FNAME,
			    "failed to allocate strings for SIP domain name");
			ret = -1;
			goto clean;
		}
		memset(s, 0, elen);
		snprintf(s, elen, "%s=", sipname_str);
		for (v = TAILQ_FIRST(&optinfo->sipname_list); v;
		    v = TAILQ_NEXT(v, link)) {
			strlcat(s, v->val_vbuf.dv_buf, elen);
			strlcat(s, " ", elen);
		}
	}

	if (nisservers) {
		elen = sizeof (nisserver_str) +
		    (INET6_ADDRSTRLEN + 1) * nisservers + 1;
		if ((s = envp[i++] = malloc(elen)) == NULL) {
			dprintf2(LOG_NOTICE, FNAME,
			    "failed to allocate strings for NIS servers");
			ret = -1;
			goto clean;
		}
		memset(s, 0, elen);
		snprintf(s, elen, "%s=", nisserver_str);
		for (v = TAILQ_FIRST(&optinfo->nis_list); v;
		    v = TAILQ_NEXT(v, link)) {
			char *addr;

			addr = in6addr2str(&v->val_addr6, 0);
			strlcat(s, addr, elen);
			strlcat(s, " ", elen);
		}
	}
	if (nisnamelen) {
		elen = sizeof (nisname_str) + nisnamelen + 1;
		if ((s = envp[i++] = malloc(elen)) == NULL) {
			dprintf2(LOG_NOTICE, FNAME,
			    "failed to allocate strings for NIS domain name");
			ret = -1;
			goto clean;
		}
		memset(s, 0, elen);
		snprintf(s, elen, "%s=", nisname_str);
		for (v = TAILQ_FIRST(&optinfo->nisname_list); v;
		    v = TAILQ_NEXT(v, link)) {
			strlcat(s, v->val_vbuf.dv_buf, elen);
			strlcat(s, " ", elen);
		}
	}

	if (nispservers) {
		elen = sizeof (nispserver_str) +
		    (INET6_ADDRSTRLEN + 1) * nispservers + 1;
		if ((s = envp[i++] = malloc(elen)) == NULL) {
			dprintf2(LOG_NOTICE, FNAME,
			    "failed to allocate strings for NIS+ servers");
			ret = -1;
			goto clean;
		}
		memset(s, 0, elen);
		snprintf(s, elen, "%s=", nispserver_str);
		for (v = TAILQ_FIRST(&optinfo->nisp_list); v;
		    v = TAILQ_NEXT(v, link)) {
			char *addr;

			addr = in6addr2str(&v->val_addr6, 0);
			strlcat(s, addr, elen);
			strlcat(s, " ", elen);
		}
	}
	if (nispnamelen) {
		elen = sizeof (nispname_str) + nispnamelen + 1;
		if ((s = envp[i++] = malloc(elen)) == NULL) {
			dprintf2(LOG_NOTICE, FNAME,
			    "failed to allocate strings for NIS+ domain name");
			ret = -1;
			goto clean;
		}
		memset(s, 0, elen);
		snprintf(s, elen, "%s=", nispname_str);
		for (v = TAILQ_FIRST(&optinfo->nispname_list); v;
		    v = TAILQ_NEXT(v, link)) {
			strlcat(s, v->val_vbuf.dv_buf, elen);
			strlcat(s, " ", elen);
		}
	}

	if (bcmcsservers) {
		elen = sizeof (bcmcsserver_str) +
		    (INET6_ADDRSTRLEN + 1) * bcmcsservers + 1;
		if ((s = envp[i++] = malloc(elen)) == NULL) {
			dprintf2(LOG_NOTICE, FNAME,
			    "failed to allocate strings for BCMC servers");
			ret = -1;
			goto clean;
		}
		memset(s, 0, elen);
		snprintf(s, elen, "%s=", bcmcsserver_str);
		for (v = TAILQ_FIRST(&optinfo->bcmcs_list); v;
		    v = TAILQ_NEXT(v, link)) {
			char *addr;

			addr = in6addr2str(&v->val_addr6, 0);
			strlcat(s, addr, elen);
			strlcat(s, " ", elen);
		}
	}
	if (bcmcsnamelen) {
		elen = sizeof (bcmcsname_str) + bcmcsnamelen + 1;
		if ((s = envp[i++] = malloc(elen)) == NULL) {
			dprintf2(LOG_NOTICE, FNAME,
			    "failed to allocate strings for BCMC domain name");
			ret = -1;
			goto clean;
		}
		memset(s, 0, elen);
		snprintf(s, elen, "%s=", bcmcsname_str);
		for (v = TAILQ_FIRST(&optinfo->bcmcsname_list); v;
		    v = TAILQ_NEXT(v, link)) {
			strlcat(s, v->val_vbuf.dv_buf, elen);
			strlcat(s, " ", elen);
		}
	}

	if (acsurllen ||
		provisioncodelen ||
		cwmpretryitvmullen ||
		cwmpretryminiwaititvlen ||
		x_charter_com_acsidlen) {
		for (v = TAILQ_FIRST(&optinfo->vendoropt_list); v;
			v = TAILQ_NEXT(v, link)) {
			if (v->val_num32 != VENDOR_IANA_ID_BROADBAND_FORUM) {
				continue;
			}

			/* Read BBF vendor sub-option list */
			for (subv = TAILQ_FIRST(&v->sublist); subv;
				subv = TAILQ_NEXT(subv, link)) {
				char env_str_buf[512];
				char str_acsurl[256];
				char str_provcode[64];

				switch(subv->type){
					case DHCP6_LISTVAL_VENDOR_BBF_ACSURL:
						memset(str_acsurl, 0, sizeof(str_acsurl));
						strncpy(str_acsurl, subv->val_vbuf.dv_buf, acsurllen);
						memset(env_str_buf, 0, sizeof(env_str_buf));
						elen = sizeof (acsurl_str) + acsurllen + 1;
						sprintf(env_str_buf, "%s", acsurl_str);
						/* Allocate env string buffer to envp and copy option string */
						if ((s = envp[i++] = malloc(elen)) == NULL) {
							dprintf2(LOG_NOTICE, FNAME,
								"failed to allocate strings for sub option type %d", subv->type);
							ret = -1;
							goto clean;
						}
						memset(s, 0, elen);
						snprintf(s, elen, "%s=", env_str_buf);
						strlcat(s, subv->val_vbuf.dv_buf, elen);
						strlcat(s, " ", elen);
						/* Update to nvram */
						sprintf(command, "nvram set acs_url=\"%s\"", str_acsurl);
						if (strlen(command)) {
							system(command);
							memset(command, 0, sizeof(command));
						}
						break;
					case DHCP6_LISTVAL_VENDOR_BBF_PROVISIONINGCODE:
						memset(str_provcode, 0, sizeof(str_provcode));
						strncpy(str_provcode, subv->val_vbuf.dv_buf, provisioncodelen);
						memset(env_str_buf, 0, sizeof(env_str_buf));
						elen = sizeof (provisioncode_str) + provisioncodelen + 1;
						sprintf(env_str_buf, "%s", provisioncode_str);
						/* Allocate env string buffer to envp and copy option string */
						if ((s = envp[i++] = malloc(elen)) == NULL) {
							dprintf2(LOG_NOTICE, FNAME,
								"failed to allocate strings for sub option type %d", subv->type);
							ret = -1;
							goto clean;
						}
						memset(s, 0, elen);
						snprintf(s, elen, "%s=", env_str_buf);
						strlcat(s, subv->val_vbuf.dv_buf, elen);
						strlcat(s, " ", elen);
						/* Update to nvram */
						sprintf(command, "nvram set acs_provisioncode=\"%s\"", str_provcode);
						if (strlen(command)) {
							system(command);
							memset(command, 0, sizeof(command));
						}
						break;
					case DHCP6_LISTVAL_VENDOR_BBF_CWMPRETRYMINWAITINTERVAL:
						memset(env_str_buf, 0, sizeof(env_str_buf));
						elen = sizeof (cwmpretryminiwaititv_str) + cwmpretryminiwaititvlen + 1;
						sprintf(env_str_buf, "%s", cwmpretryminiwaititv_str);
						/* Allocate env string buffer to envp and copy option string */
						if ((s = envp[i++] = malloc(elen)) == NULL) {
							dprintf2(LOG_NOTICE, FNAME,
								"failed to allocate strings for sub option type %d", subv->type);
							ret = -1;
							goto clean;
						}
						memset(s, 0, elen);
						snprintf(s, elen, "%s=", env_str_buf);
						strlcat(s, subv->val_vbuf.dv_buf, elen);
						strlcat(s, " ", elen);
						/* Check cwmpretryminiwaititv Value */
						cwmpretryminiwaititv = atoi(subv->val_vbuf.dv_buf);
						if ((cwmpretryminiwaititv < 1) || (cwmpretryminiwaititv > 65535)) {
							cwmpretryminiwaititv = 5; /* TR-181 default value*/
						}
						/* Update to nvram */
						sprintf(command, "nvram set acs_cwmpretryminiwaititv=\"%d\"", cwmpretryminiwaititv);
						if (strlen(command)) {
							system(command);
							memset(command, 0, sizeof(command));
						}
						break;

					case DHCP6_LISTVAL_VENDOR_BBF_CWMPRETRYINTERVALMULTI:
						memset(env_str_buf, 0, sizeof(env_str_buf));
						elen = sizeof (cwmpretryitvmul_str) + cwmpretryitvmullen + 1;
						sprintf(env_str_buf, "%s", cwmpretryitvmul_str);
						/* Allocate env string buffer to envp and copy option string */
						if ((s = envp[i++] = malloc(elen)) == NULL) {
							dprintf2(LOG_NOTICE, FNAME,
								"failed to allocate strings for sub option type %d", subv->type);
							ret = -1;
							goto clean;
						}
						memset(s, 0, elen);
						snprintf(s, elen, "%s=", env_str_buf);
						strlcat(s, subv->val_vbuf.dv_buf, elen);
						strlcat(s, " ", elen);
						/* Check cwmpretryitvmul Value */
						cwmpretryitvmul = atoi(subv->val_vbuf.dv_buf);
						if ((cwmpretryitvmul < 1000) || (cwmpretryminiwaititv > 65535)) {
							cwmpretryitvmul = 2000; /* TR-181 default value*/
						}
						/* Update to nvram */
						sprintf(command, "nvram set acs_cwmpretryitvmul=\"%d\"", cwmpretryitvmul);
						if (strlen(command)) {
							system(command);
							memset(command, 0, sizeof(command));
						}
						break;

					case DHCP6_LISTVAL_VENDOR_BBF_X_CHARTER_COM_ACSID:
						memset(env_str_buf, 0, sizeof(env_str_buf));
						elen = sizeof (x_charter_com_acsid_str) + x_charter_com_acsidlen + 1;
						sprintf(env_str_buf, "%s", x_charter_com_acsid_str);
						/* Allocate env string buffer to envp and copy option string */
						if ((s = envp[i++] = malloc(elen)) == NULL) {
							dprintf2(LOG_NOTICE, FNAME,
								"failed to allocate strings for sub option type %d", subv->type);
							ret = -1;
							goto clean;
						}
						memset(s, 0, elen);
						snprintf(s, elen, "%s=", env_str_buf);
						strlcat(s, subv->val_vbuf.dv_buf, elen);
						strlcat(s, " ", elen);
						/* Update to nvram */
						sprintf(command, "nvram set acs_x_charter_com_acsid=\"%s\"", subv->val_vbuf.dv_buf);
						if (strlen(command)) {
							system(command);
							memset(command, 0, sizeof(command));
						}
						break;
					default:
						dprintf2(LOG_NOTICE, FNAME,
							"Unknown sub option type %d ", subv->type);
						continue;
						break;
				}
			}
		}
		system("nvram commit");
	}

	/* launch the script */
/* Foxconn Bernie modified start, 2014/11/27 @ unuse code*/
#if 0
	pid = fork();
	if (pid < 0) {
		dprintf2(LOG_ERR, FNAME, "failed to fork: %s", strerror(errno));
		ret = -1;
		goto clean;
	} else if (pid) {
		int wstatus;

		do {
			wpid = wait(&wstatus);
		} while (wpid != pid && wpid > 0);

		if (wpid < 0)
			dprintf2(LOG_ERR, FNAME, "wait: %s", strerror(errno));
		else {
			dprintf2(LOG_DEBUG, FNAME,
			    "script \"%s\" terminated", scriptpath);
		}
	} else {
		char *argv[2];
		int fd;

		argv[0] = scriptpath;
		argv[1] = NULL;

		if (safefile(scriptpath)) {
			dprintf2(LOG_ERR, FNAME,
			    "script \"%s\" cannot be executed safely",
			    scriptpath);
			exit(1);
		}

		if (foreground == 0 && (fd = open("/dev/null", O_RDWR)) != -1) {
			dup2(fd, STDIN_FILENO);
			dup2(fd, STDOUT_FILENO);
			dup2(fd, STDERR_FILENO);
			if (fd > STDERR_FILENO)
				close(fd);
		}

		execve(scriptpath, argv, envp);

		dprintf2(LOG_ERR, FNAME, "child: exec failed: %s",
		    strerror(errno));
		exit(0);
	}
#endif
/* Foxconn Bernie modified end, 2014/11/27 */

  clean:
	for (i = 0; i < envc; i++)
		free(envp[i]);
	free(envp);

	return ret;
}
