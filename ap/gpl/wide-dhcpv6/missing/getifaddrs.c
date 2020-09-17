/*
 * Copyright (c) 2006 WIDE Project. All rights reserved.
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

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <sys/ioctl.h>
//#include <linux/in.h>
#include <linux/in6.h>
//#include <linux/ipv6.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>

//#include <sys/sockio.h>
#include <sys/socket.h>
#include <net/if.h>

#include "ifaddrs.h"

/* Foxconn Bernie added start, 2014/11/27 @ fix compiler error.*/
static int
//get_lifreq(int fd, struct lifreq **ifr_ret)
get_ifreq(int fd, struct ifreq **ifr_ret)
{
	//struct lifnum lifn;
	//struct lifconf lifc;
	//struct lifreq *lifrp;
	int ifn;
	struct ifconf ifc;
	struct ifreq *ifrp;

	//lifn.lifn_family = AF_UNSPEC;
	//lifn.lifn_flags = 0;
	//if (ioctl(fd, SIOCGLIFNUM, &lifn) == -1)
#ifdef SIOCGIFNUM
	if (ioctl(fd, SIOCGIFNUM, &ifn) == -1)
		//lifn.lifn_count = 16;
		ifn = 16;
	else
		//lifn.lifn_count += 16;
		ifn += 16;
#else
		//ifn = 16;
		ifn = 32;
#endif

	for (;;) {
		//lifc.lifc_len = lifn.lifn_count * sizeof (*lifrp);
		ifc.ifc_len = ifn * sizeof (*ifrp);
		//lifrp = malloc(lifc.lifc_len);
		ifrp = malloc(ifc.ifc_len);
		if (ifrp == NULL)
			return (-1);

		//lifc.lifc_family = AF_UNSPEC;
		//lifc.lifc_flags = 0;
		//lifc.lifc_buf = (char *)lifrp;
		ifc.ifc_buf = (char *)ifrp;

		//if (ioctl(fd, SIOCGLIFCONF, &lifc) == -1) {
		if (ioctl(fd, SIOCGIFCONF, &ifc) == -1) {
			free(ifrp);
			if (errno == EINVAL) {
				//lifn.lifn_count <<= 1;
				ifn <<= 1;
				continue;
			}
			(void) close(fd);
			return (-1);
		}
		//if (lifc.lifc_len < (lifn.lifn_count - 1) * sizeof (*lifrp))
		if (ifc.ifc_len < (ifn - 1) * sizeof (*ifrp))
			break;
		//free(ifrp);
		//lifn.lifn_count <<= 1;
		ifn <<= 1;
	}
//	(void) close(fd);

	*ifr_ret = ifrp;
	//return (lifc.lifc_len / sizeof (*lifrp));
	return (ifc.ifc_len / sizeof (*ifrp));
}

static size_t
//nbytes(const struct lifreq *lifrp, int nlif, size_t socklen)
nbytes(const struct ifreq *ifrp, int lif, size_t socklen)
{
	size_t len = 0;
	size_t slen;

	//while (nlif > 0) {
	while (lif > 0) {
		//slen = strlen(lifrp->lifr_name) + 1;
		slen = strlen(ifrp->ifr_name) + 1;
		len += sizeof (struct ifaddrs) + ((slen + 3) & ~3);
		len += 3 * socklen;
		//lifrp++;
		ifrp++;
		//nlif--;
		lif--;
	}
	return (len);
}

static struct sockaddr *
//addrcpy(struct sockaddr_storage *addr, char **bufp)
addrcpy(struct sockaddr *addr, char **bufp)
{
	char *buf = *bufp;
	size_t len;
	//len = ((addr->ss_family == AF_INET) ? sizeof (struct sockaddr_in) :
	//    sizeof (struct sockaddr_in6));
	//len =  sizeof(struct sockaddr_in6);
	len =  sizeof(struct sockaddr);
	//len =  sizeof(struct sockaddr_ll);
	(void) memcpy(buf, addr, len);
	*bufp = buf + len;
	return ((struct sockaddr *)buf);
}

static int
//populate(struct ifaddrs *ifa, int fd, struct lifreq *lifrp, int nlif, int af,
populate(struct ifaddrs *ifa, int fd, struct ifreq *ifrp, int lif, int af,
    char **bufp)
{
	char *buf = *bufp;
	size_t slen;

	//while (nlif > 0) {
	while (lif > 0) {
		if (!strcmp(ifrp->ifr_name, "lo")) //skip lo interface
			goto next;
		//ifa->ifa_next = (nlif > 1) ? ifa + 1 : NULL;
		ifa->ifa_next = (lif > 1) ? ifa + 1 : NULL;
		//(void) strcpy(ifa->ifa_name = buf, lifrp->lifr_name);
		(void) strcpy(ifa->ifa_name = buf, ifrp->ifr_name);
		//slen = strlen(lifrp->lifr_name) + 1;
		slen = strlen(ifrp->ifr_name) + 1;
		buf += (slen + 3) & ~3;
		//if (ioctl(fd, SIOCGLIFFLAGS, lifrp) == -1)
		if (ioctl(fd, SIOCGIFFLAGS, ifrp) == -1)
			ifa->ifa_flags = 0;
		else
			//ifa->ifa_flags = lifrp->lifr_flags;
			ifa->ifa_flags = ifrp->ifr_flags;
		//if (ioctl(fd, SIOCGLIFADDR, lifrp) == -1)
		if (ioctl(fd, SIOCGIFADDR, ifrp) == -1)
			ifa->ifa_addr = NULL;
		else
			//ifa->ifa_addr = addrcpy(&lifrp->lifr_addr, &buf);
			ifa->ifa_addr = addrcpy(&ifrp->ifr_addr, &buf);
		//if (ioctl(fd, SIOCGLIFNETMASK, lifrp) == -1)
		if (ioctl(fd, SIOCGIFNETMASK, ifrp) == -1)
			ifa->ifa_netmask = NULL;
		else
			//ifa->ifa_netmask = addrcpy(&lifrp->lifr_addr, &buf);
			ifa->ifa_netmask = addrcpy(&ifrp->ifr_addr, &buf);
		if (ifa->ifa_flags & IFF_POINTOPOINT) {
			//if (ioctl(fd, SIOCGLIFDSTADDR, lifrp) == -1)
			if (ioctl(fd, SIOCGIFDSTADDR, ifrp) == -1)
				ifa->ifa_dstaddr = NULL;
			else
				ifa->ifa_dstaddr =
				    //addrcpy(&lifrp->lifr_dstaddr, &buf);
				    addrcpy(&ifrp->ifr_dstaddr, &buf);
		} else if (ifa->ifa_flags & IFF_BROADCAST) {
			//if (ioctl(fd, SIOCGLIFBRDADDR, lifrp) == -1)
			if (ioctl(fd, SIOCGIFBRDADDR, ifrp) == -1)
				ifa->ifa_broadaddr = NULL;
			else
				ifa->ifa_broadaddr =
				    //addrcpy(&lifrp->lifr_broadaddr, &buf);
				    addrcpy(&ifrp->ifr_broadaddr, &buf);
		} else {
			ifa->ifa_dstaddr = NULL;
		}

		ifa++;
		//nlif--;
next:
		lif--;
		//lifrp++;
		ifrp++;
	}
	*bufp = buf;
	return (0);
}

int
getifaddrs(struct ifaddrs **ifap)
{
	//int fd4, fd6;
	int fd6;
	//int nif4, nif6 = 0;
	int nif6 = 0;
	//struct lifreq *ifr4 = NULL;
	//struct lifreq *ifr6 = NULL;
	//struct ifreq *ifr4 = NULL;
	struct ifreq *ifr6 = NULL;
	struct ifaddrs *ifa = NULL;
	char *buf;

	//if ((fd4 = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	//	return (-1);
	//if ((fd6 = socket(AF_INET6, SOCK_DGRAM, 0)) == -1 &&
	if ((fd6 = socket(AF_INET, SOCK_DGRAM, 0)) == -1 &&
	//if ((fd6 = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW)) == -1 &&
	    errno != EAFNOSUPPORT) {
	//	(void) close(fd4);
		return (-1);
	}

	//if ((nif4 = get_lifreq(fd4, &ifr4)) == -1 ||
	//    (fd6 != -1 && (nif6 = get_lifreq(fd6, &ifr6)) == -1))
	//	goto failure;
	if ((fd6 != -1 && (nif6 = get_ifreq(fd6, &ifr6)) == -1))
		goto failure;

	//if (nif4 == 0 && nif6 == 0) {
	if (nif6 == 0) {
		*ifap = NULL;
		return (0);
	}

	//ifa = malloc(nbytes(ifr4, nif4, sizeof (struct sockaddr_in)) +
	//    nbytes(ifr6, nif6, sizeof (struct sockaddr_in6)));
	//ifa = malloc(nbytes(ifr6, nif6, sizeof (struct sockaddr_in6)));
	ifa = malloc(nbytes(ifr6, nif6, sizeof (struct sockaddr)));
	if (ifa == NULL)
		goto failure;

	//buf = (char *)(ifa + nif4 + nif6);
	buf = (char *)(ifa + nif6);

	//if (populate(ifa, fd4, ifr4, nif4, AF_INET, &buf) == -1)
	//	goto failure;
	//if (nif4 > 0 && nif6 > 0)
	//	ifa[nif4 - 1].ifa_next = ifa + nif4;
	//if (populate(ifa + nif4, fd6, ifr6, nif6, AF_INET6, &buf) == -1)
	//if (populate(ifa, fd6, ifr6, nif6, AF_INET6, &buf) == -1)
	if (populate(ifa, fd6, ifr6, nif6, AF_INET, &buf) == -1)
	//if (populate(ifa, fd6, ifr6, nif6, AF_PACKET, &buf) == -1)
		goto failure;
	*ifap = ifa;
	if (fd6 != -1)
		(void) close(fd6);
	if (ifr6)
		free(ifr6);
	
	return (0);

failure:
	if (ifa)
		free(ifa);
	//(void) close(fd4);
	if (fd6 != -1)
		(void) close(fd6);
	//free(ifr4);
	if (ifr6)
		free(ifr6);
	return (-1);
}

void
freeifaddrs(struct ifaddrs *ifa)
{
	if (ifa)
		free(ifa);
}
/* Foxconn Bernie added end, 2014/11/27 */
/* Foxconn Bernie added start, 2014/11/27 */
int
find_ipv6_addr(const char * ifname,
               char * dst, int n)
{
	FILE *f;
	char devname[IFNAMSIZ];
	char addr6p[8][5]={0};
	int has_ip=0, plen, scope, dad_status, if_idx;
	char buf[64];
	int r = 0;
	int gua = 0;

	if (!dst)
		return -1;

	if (!ifname || ifname[0] == '\0')
		return -1;
	f = fopen("/proc/net/if_inet6", "r");
	if (f == NULL) {
		fprintf(stderr, "Cannot open /proc/net/if_inet6 file for interface %s.\n", ifname);
		return -1;
	} else {
		while (gua == 0 &&
			fscanf(f, "%4s%4s%4s%4s%4s%4s%4s%4s %02x %02x %02x %02x %20s\n",
				addr6p[0], addr6p[1], addr6p[2], addr6p[3], addr6p[4],
				addr6p[5], addr6p[6], addr6p[7], &if_idx, &plen, &scope,
				&dad_status, devname) != EOF) {
			if (!strcmp(devname, ifname)) {
				switch (scope) {
					case 0:
						if(gua == 0) {
							sprintf(buf, "%s:%s:%s:%s:%s:%s:%s:%s",
								addr6p[0], addr6p[1], addr6p[2], addr6p[3],
								addr6p[4], addr6p[5], addr6p[6], addr6p[7]);
							gua = 1;
							if(addr6p[0][0] == 'f' && (addr6p[0][1] == 'd' || addr6p[0][1] == 'c'))
								gua = 0;
						}
						has_ip = 1;
						break;
					case 32:
						if(has_ip == 0)
							sprintf(buf, "%s:%s:%s:%s:%s:%s:%s:%s",
								addr6p[0], addr6p[1], addr6p[2], addr6p[3],
								addr6p[4], addr6p[5], addr6p[6], addr6p[7]);
						has_ip = 1;
						break;
					default:
						has_ip = 0;
						sprintf(buf, "::");
						break;
					}
			}
		}
		if (has_ip == 0) {
			fprintf(stderr, "No IPv6 address found for interface %s.\n", ifname);
			return -1;
		}
		fclose(f);
		snprintf(dst, n, "%s", buf);
		r = 1;
	}
	return r;
}
/* Foxconn Bernie added end, 2014/11/27 */
