/* dhcpd.c
 *
 * udhcp Server
 * Copyright (C) 1999 Matthew Ramsay <matthewr@moreton.com.au>
 *			Chris Trew <ctrew@moreton.com.au>
 *
 * Rewrite by Russ Dill <Russ.Dill@asu.edu> July 2001
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <time.h>
#include <sys/time.h>

#include "debug.h"
#include "dhcpd.h"
#include "arpping.h"
#include "socket.h"
#include "options.h"
#include "files.h"
#include "leases.h"
#include "packet.h"
#include "serverpacket.h"
#include "pidfile.h"


/* globals */
struct dhcpOfferedAddr *leases;
struct server_config_t server_config;
static int signal_pipe[2];

/* Exit and cleanup */
static void exit_server(int retval)
{
	pidfile_delete(server_config.pidfile);
	CLOSE_LOG();
	exit(retval);
}


/* Signal handler */
static void signal_handler(int sig)
{
	if (send(signal_pipe[1], &sig, sizeof(sig), MSG_DONTWAIT) < 0) {
		LOG(LOG_ERR, "Could not send signal: %s", 
			strerror(errno));
	}
}

static void fing_format_dhcp_option_string(struct dhcpMessage* packet, char* out_str, unsigned int len)
{
    int i, length, option55Req = 0;
    unsigned char *optionptr;
    int over = 0, done = 0, curr = OPTION_FIELD;

    optionptr = packet->options;
    i = 0;
    /* foxconn modified start, zacker, 09/18/2009, @big_size_pkt */
    //length = 308;
    /* Foxconn modified start pling 05/11/2012 */
    /* Fix a DHCP server crash issue:
     * DHCP server packet was not modified to receive large size packet.
     * So we only check options for LEN_OPTIONS long.
     */
    if (packet->op == BOOTREQUEST)
        length = LEN_OPTIONS;
    else
        length = LEN_OPTIONS + LEN_PADDING;
    /* Foxconn modified end pling 05/11/2012 */
    /* foxconn modified end, zacker, 09/18/2009, @big_size_pkt */
    while (!done) {
        if (i >= length) {
            LOG(LOG_WARNING, "bogus packet, option fields too long.");
            return;
        }
#if 0
        if (optionptr[i + OPT_CODE] == code) {
            if (i + 1 + optionptr[i + OPT_LEN] >= length) {
                LOG(LOG_WARNING, "bogus packet, option fields too long.");
                return NULL;
            }
            return optionptr + i + 2;
        }	
#endif

        switch (optionptr[i + OPT_CODE]) {
            case DHCP_PADDING:
                i++;
                break;
                /* Foxconn Harry added start 2020/04/20, Following the Fing team's request, we just need to collect the option 55 parameters */
            case DHCP_PARAM_REQ:
                for (option55Req = 1; option55Req <= optionptr[i + OPT_LEN]; option55Req++) {        
                    if(strlen(out_str)+5>len)
                        return;
                    else
                    {
                        char buf[16];
                        if(strlen(out_str)>0)
                            strcat(out_str, ",");
                        snprintf(buf, sizeof(buf), "%d", optionptr[i + OPT_LEN + option55Req]);
                        strcat(out_str, buf);
                    }
                }
                i += optionptr[OPT_LEN + i] + 2;
                break; 
                /* Foxconn Harry added start 2020/04/20, Following the Fing team's request, we just need to collect the option 55 parameters */
            case DHCP_OPTION_OVER:
                if (i + 1 + optionptr[i + OPT_LEN] >= length) {
                    LOG(LOG_WARNING, "bogus packet, option fields too long.");
                    return;
                }
                over = optionptr[i + 3];
                i += optionptr[OPT_LEN] + 2;
                break;
            case DHCP_END:
                if (curr == OPTION_FIELD && over & FILE_FIELD) {
                    optionptr = packet->file;
                    i = 0;
                    length = 128;
                    curr = FILE_FIELD;
                } else if (curr == FILE_FIELD && over & SNAME_FIELD) {
                    optionptr = packet->sname;
                    i = 0;
                    length = 64;
                    curr = SNAME_FIELD;
                } else done = 1;
                break;
            default:
                i += optionptr[OPT_LEN + i] + 2;
        }
    }
    return;
}

static void fing_write_client_info2file(struct dhcpMessage* packet, struct dhcpOfferedAddr *lease)
{
    FILE *fp;
    char path[64];
    unsigned char mac[6];
    char dhcp_option_str[128];
    struct in_addr client_ip;
    //char client_ip_str[32];
    char buf[512];
    unsigned char* hostname = NULL;
    unsigned char* vendor = NULL;
    char vendor_buf[256] = {0};
    char hostname_buf[256] = {0};

    memcpy(mac, &lease->chaddr[0], 6);
    sprintf(path,"/tmp/dhcp_info_for_fing/%.2X:%.2X:%.2X:%.2X:%.2X:%.2X",
            mac[0], mac[1], mac[2],
            mac[3], mac[4], mac[5]);
    if (!(fp = fopen(path, "w")))
    {
        DEBUG(LOG_WARNING,"cant no open %s",path);
    }
    if(fp)
    {
        client_ip.s_addr = lease->yiaddr;
        snprintf(buf, sizeof(buf)-1, "ip %s\n", inet_ntoa(client_ip));
        fputs(buf, fp);
        memset(dhcp_option_str, 0, sizeof(dhcp_option_str));
        fing_format_dhcp_option_string(packet, dhcp_option_str, sizeof(dhcp_option_str));
        snprintf(buf, sizeof(buf)-1, "params %s\n", dhcp_option_str);
        fputs(buf, fp);
        //fwrite(dhcp_option_str, sizeof(dhcp_option_str), 1, fp);
        vendor = get_option(packet, DHCP_VENDOR);
        if(vendor)
        {
            strncpy(vendor_buf, (char*)vendor, vendor[-1]);
            snprintf(buf, sizeof(buf)-1, "vendor %s\n", vendor_buf);
            fputs(buf, fp);
        }
        hostname = get_option(packet, DHCP_HOST_NAME);
        if(hostname)
        {
            strncpy(hostname_buf, (char*)hostname, hostname[-1]);
            snprintf(buf, sizeof(buf)-1, "hostname %s\n", hostname_buf);
            fputs(buf, fp);
        }
        fclose(fp);
    } 

}


#ifdef COMBINED_BINARY	
int udhcpd_main(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{	
	fd_set rfds;
	struct timeval tv;
	int server_socket = -1;
	int bytes, retval;
	struct dhcpMessage packet;
	unsigned char *state;
	unsigned char *server_id, *requested, *hostname , *client_id, *FQDN;
	u_int32_t server_id_align, requested_align, client_id_align, client_id_type;
	unsigned char mac[6];
	unsigned long timeout_end;
	struct option_set *option;
	struct dhcpOfferedAddr *lease;
	int pid_fd;
	int max_sock;
	int sig;
	unsigned cmd[64];
	
	OPEN_LOG("udhcpd");
	LOG(LOG_INFO, "udhcp server (v%s) started", VERSION);

    sprintf(cmd,"mkdir /tmp/dhcp_device_release");
    system(cmd);
    mkdir("/tmp/dhcp_info_for_fing", 0777);

	memset(&server_config, 0, sizeof(struct server_config_t));
	
	if (argc < 2)
		read_config(DHCPD_CONF_FILE);
	else read_config(argv[1]);
    
    /*get reserved ip from configuration file*/ 
    if(strstr(argv[1], "udhcpd2"))
        num_of_reservedIP = 0;  /* We don't have Reserved IP for Guest Network  */
    else
        num_of_reservedIP = getReservedAddr(resrvMacAddr, resrvIpAddr);    

	pid_fd = pidfile_acquire(server_config.pidfile);
	pidfile_write_release(pid_fd);

	if ((option = find_option(server_config.options, DHCP_LEASE_TIME))) {
		memcpy(&server_config.lease, option->data + 2, 4);
		server_config.lease = ntohl(server_config.lease);
	}
	else server_config.lease = LEASE_TIME;
	
	leases = malloc(sizeof(struct dhcpOfferedAddr) * server_config.max_leases);
	memset(leases, 0, sizeof(struct dhcpOfferedAddr) * server_config.max_leases);
	read_leases(server_config.lease_file);

	if (read_interface(server_config.interface, &server_config.ifindex,
			   &server_config.server, server_config.arp) < 0)
		exit_server(1);

#ifndef DEBUGGING
	pid_fd = pidfile_acquire(server_config.pidfile); /* hold lock during fork. */
	if (daemon(0, 0) == -1) {
		perror("fork");
		exit_server(1);
	}
	pidfile_write_release(pid_fd);
#endif


	socketpair(AF_UNIX, SOCK_STREAM, 0, signal_pipe);
	signal(SIGUSR1, signal_handler);
	signal(SIGTERM, signal_handler);

	timeout_end = time(0) + server_config.auto_time;
	while(1) { /* loop until universe collapses */

		if (server_socket < 0)
			if ((server_socket = listen_socket(INADDR_ANY, SERVER_PORT, server_config.interface)) < 0) {
				LOG(LOG_ERR, "FATAL: couldn't create server socket, %s", strerror(errno));
				exit_server(0);
			}			

		FD_ZERO(&rfds);
		FD_SET(server_socket, &rfds);
		FD_SET(signal_pipe[0], &rfds);
		if (server_config.auto_time) {
			tv.tv_sec = timeout_end - time(0);
			tv.tv_usec = 0;
		}
		if (!server_config.auto_time || tv.tv_sec > 0) {
			max_sock = server_socket > signal_pipe[0] ? server_socket : signal_pipe[0];
			retval = select(max_sock + 1, &rfds, NULL, NULL, 
					server_config.auto_time ? &tv : NULL);
		} else retval = 0; /* If we already timed out, fall through */

		if (retval == 0) {
			write_leases();
			timeout_end = time(0) + server_config.auto_time;
			continue;
		} else if (retval < 0 && errno != EINTR) {
			DEBUG(LOG_INFO, "error on select");
			continue;
		}
		
		if (FD_ISSET(signal_pipe[0], &rfds)) {
			if (read(signal_pipe[0], &sig, sizeof(sig)) < 0)
				continue; /* probably just EINTR */
			switch (sig) {
			case SIGUSR1:
				LOG(LOG_INFO, "Received a SIGUSR1");
				write_leases();
				/* why not just reset the timeout, eh */
				timeout_end = time(0) + server_config.auto_time;
				continue;
			case SIGTERM:
				LOG(LOG_INFO, "Received a SIGTERM");
				exit_server(0);
			}
		}

		if ((bytes = get_packet(&packet, server_socket)) < 0) { /* this waits for a packet - idle */
			if (bytes == -1 && errno != EINTR) {
				DEBUG(LOG_INFO, "error on read, %s, reopening socket", strerror(errno));
				close(server_socket);
				server_socket = -1;
			}
			continue;
		}

		if ((state = get_option(&packet, DHCP_MESSAGE_TYPE)) == NULL) {
			DEBUG(LOG_ERR, "couldn't get option from packet, ignoring");
			continue;
		}
        
		/* ADDME: look for a static lease */
		lease = find_lease_by_chaddr(packet.chaddr);
		switch (state[0]) {
		case DHCPDISCOVER:
			DEBUG(LOG_INFO,"received DISCOVER");
			
			if (sendOffer(&packet) < 0) {
				LOG(LOG_ERR, "send OFFER failed");
			}
			break;			
 		case DHCPREQUEST:
        {
 		    /* foxcon added start by EricHuang, 03/01/2007 */
 		    unsigned char mac[6];
 		    u_int32_t r_addr;
 		    memcpy(mac, packet.chaddr, 6);
 		    /* foxcon added end by EricHuang, 03/01/2007 */
 		    
			DEBUG(LOG_INFO, "received REQUEST");

			requested = get_option(&packet, DHCP_REQUESTED_IP);
			server_id = get_option(&packet, DHCP_SERVER_ID);
			hostname = get_option(&packet, DHCP_HOST_NAME);
            client_id = get_option(&packet, DHCP_CLIENT_ID);
            FQDN = get_option(&packet, 0x51);

			if (requested) memcpy(&requested_align, requested, 4);
			/* Foxconn added start pling 08/03/2011 */
			/* Should clear this var, otherwise it keeps old value
			 * (from previous packet) and cause reserved IP client
			 * to get NAK.
			 */
			else
				requested_align = 0;
			/* Foxconn added end pling 08/03/2011 */
			if (server_id) memcpy(&server_id_align, server_id, 4);

            if (client_id) 
            {
                memcpy(&client_id_type,client_id,1);
                memcpy(mac, client_id+1, 6);
            }
            /* foxcon added start by EricHuang, 03/01/2007 */
            r_addr = find_reserved_ip(mac);
            if (r_addr) {
                if (requested_align)
                {
                    if ( requested_align!=htonl(r_addr))
                    {
                        sendNAK(&packet);
                        DEBUG(LOG_INFO, "prepare to send a reserved ip (0x%x, 0x%x)\n", requested_align, htonl(r_addr));
                        break;
                    }
                }
            }
            /* foxcon added end by EricHuang, 03/01/2007 */

			if (lease) { /*ADDME: or static lease */
				if (server_id) {
					/* SELECTING State */
					DEBUG(LOG_INFO, "server_id = %08x", ntohl(server_id_align));
					if (server_id_align == server_config.server && requested && 
					    requested_align == lease->yiaddr) {
						sendACK(&packet, lease->yiaddr);
					}
				} else {
					if (requested) {
						/* INIT-REBOOT State */
						if (lease->yiaddr == requested_align)
							sendACK(&packet, lease->yiaddr);
						else sendNAK(&packet);
					} else {
						/* RENEWING or REBINDING State */
						if (lease->yiaddr == packet.ciaddr)
							sendACK(&packet, lease->yiaddr);
						else {
							/* don't know what to do!!!! */
							sendNAK(&packet);
						}
					}						
				}
				if (hostname) {
					bytes = hostname[-1];
					if (bytes >= (int) sizeof(lease->hostname))
						bytes = sizeof(lease->hostname) - 1;
					strncpy(lease->hostname, hostname, bytes);
					lease->hostname[bytes] = '\0';
                    DEBUG(LOG_INFO,"rewrite leass table, hostname: %s\n", lease->hostname);
                    /* foxconn wklin removed, 05/07/2007 */
                    /* write_leases(); */ /*Rewrite lease table into file.*/
				} else
					lease->hostname[0] = '\0';
                /* foxconn wklin added, 05/07/2007 */
                FILE *fp;
                unsigned char path[64];
                memcpy(mac, &lease->chaddr[0], 6);
                sprintf(path,"/tmp/dhcp_device_release/%.2X:%.2X:%.2X:%.2X:%.2X:%.2X",
                        mac[0], mac[1], mac[2],
                        mac[3], mac[4], mac[5]);
                if (!(fp = fopen(path, "w")))
                {
                    DEBUG(LOG_WARNING,"cant no open %s",path);
                }
                if(fp)
                {
                    fwrite(lease->chaddr, 16, 1, fp);
                    fclose(fp);
                }
                fing_write_client_info2file(&packet, lease);
                write_leases(); /*Rewrite lease table into file.*/
			
			/* what to do if we have no record of the client */
			} else if (server_id) {
				/* SELECTING State */

			} else if (requested) {
				/* INIT-REBOOT State */
				if ((lease = find_lease_by_yiaddr(requested_align))) {
					if (lease_expired(lease)) {
						/* probably best if we drop this lease */
#if 0						
						memset(lease->chaddr, 0, 16);
                        write_leases(); /*Rewrite lease table into file.*/
#endif /* Modify for HOME IOT test IP change issue*/                    
					/* make some contention for this address */
					} else sendNAK(&packet);
				} else if (requested_align < server_config.start || 
					   requested_align > server_config.end) {
					sendNAK(&packet);
				} else {
					sendNAK(&packet);
				}

			} else if (packet.ciaddr) {
				/* RENEWING or REBINDING State */
				sendNAK(&packet);
			}
			break;
		}
		case DHCPDECLINE:
			DEBUG(LOG_INFO,"received DECLINE");
			if (lease) {
				memset(lease->chaddr, 0, 16);
				lease->expires = time(0) + server_config.decline_time;
                write_leases(); /*Rewrite lease table into file.*/
			}			
			break;
		case DHCPRELEASE:
			DEBUG(LOG_INFO,"received RELEASE");
			if (lease) 
            {    
                lease->expires = time(0);
                write_leases(); /*Rewrite lease table into file.*/
            }
			break;
		case DHCPINFORM:
			DEBUG(LOG_INFO,"received INFORM");
			send_inform(&packet);
			break;	
		default:
			LOG(LOG_WARNING, "unsupported DHCP message (%02x) -- ignoring", state[0]);
		}
	}

	return 0;
}

