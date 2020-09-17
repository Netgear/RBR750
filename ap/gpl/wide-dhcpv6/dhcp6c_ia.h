/*	$KAME: dhcp6c_ia.h,v 1.6 2004/06/10 07:28:29 jinmei Exp $	*/

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
/* Foxconn Bernie added start, 2014/11/27 @ copy form dhcp6c_ia.c*/
extern int get_na_fail;
extern int get_pd_fail;
typedef enum {IAS_ACTIVE, IAS_RENEW, IAS_REBIND} iastate_t;

struct ia {
        TAILQ_ENTRY(ia) link;

        /* back pointer to configuration */
        struct ia_conf *conf;

        /* common parameters of IA */
        u_int32_t t1;           /* duration for renewal */
        u_int32_t t2;           /* duration for rebind  */

        /* internal parameters for renewal/rebinding */
        iastate_t state;
        struct dhcp6_timer *timer;
        struct dhcp6_eventdata *evdata;

        /* DHCP related parameters */
        struct dhcp6_if *ifp;   /* DHCP interface */
        struct duid serverid;   /* the server ID that provided this IA */

        /* control information shared with each particular config routine */
        struct iactl *ctl;

        /* authentication parameters for transaction with servers on this IA */
        struct authparam *authparam;
};
/* Foxconn Bernie added end, 2014/11/27 */

struct iactl {
	struct ia *iactl_ia;	/* back pointer to IA */

	/* callback function called when something may happen on the IA */
	void (*callback) __P((struct ia *));

	/* common methods: */
	int (*isvalid) __P((struct iactl *));
	u_int32_t (*duration) __P((struct iactl *));
	int (*renew_data) __P((struct iactl *, struct dhcp6_ia *,
	    struct dhcp6_eventdata **, struct dhcp6_eventdata *));
	int (*rebind_data) __P((struct iactl *, struct dhcp6_ia *,
	    struct dhcp6_eventdata **, struct dhcp6_eventdata *));
	int (*release_data) __P((struct iactl *, struct dhcp6_ia *,
	    struct dhcp6_eventdata **, struct dhcp6_eventdata *));
	int (*reestablish_data) __P((struct iactl *, struct dhcp6_ia *,
	    struct dhcp6_eventdata **, struct dhcp6_eventdata *));
	void (*cleanup) __P((struct iactl *));
};
/* Foxconn Bernie added start, 2014/11/27 @ copy from prefixconf.c*/
TAILQ_HEAD(siteprefix_list, siteprefix);

struct siteprefix {
	TAILQ_ENTRY (siteprefix) link;

	struct dhcp6_prefix prefix;
	time_t updatetime;
	struct dhcp6_timer *timer;
	struct iactl_pd *ctl;
	TAILQ_HEAD(, dhcp6_ifprefix) ifprefix_list; /* interface prefixes */
};

struct dhcp6_ifprefix {
	TAILQ_ENTRY(dhcp6_ifprefix) plink;

	/* interface configuration */
	struct prefix_ifconf *ifconf;

	/* interface prefix parameters */
	struct sockaddr_in6 paddr;
	int plen;

	/* address assigned on the interface based on the prefix */
	struct sockaddr_in6 ifaddr;
};

struct iactl_pd {
	struct iactl common;
	struct pifc_list *pifc_head;
	struct siteprefix_list siteprefix_head;
};
#define iacpd_ia common.iactl_ia
#define iacpd_callback common.callback
#define iacpd_isvalid common.isvalid
#define iacpd_duration common.duration
#define iacpd_renew_data common.renew_data
#define iacpd_rebind_data common.rebind_data
#define iacpd_reestablish_data common.reestablish_data
#define iacpd_release_data common.release_data
#define iacpd_cleanup common.cleanup

void remove_siteprefix __P((struct siteprefix *));
/* Foxconn Bernie added end, 2014/11/27 */

extern void update_ia __P((iatype_t, struct dhcp6_list *,
    struct dhcp6_if *, struct duid *, struct authparam *));
extern void release_all_ia __P((struct dhcp6_if *));
extern void remove_ia __P((struct ia *));
extern int release_ia __P((struct ia *));
extern char * iastr __P((iatype_t ));