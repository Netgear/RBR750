include ../../config.mk
include ../../config.in

IPOBJ=ip.o ipaddress.o iproute.o iprule.o \
    rtm_map.o iptunnel.o ipneigh.o iplink.o \
    ipmaddr.o ipmonitor.o ipmroute.o ipprefix.o \
    ipxfrm.o xfrm_state.o xfrm_policy.o xfrm_monitor.o

RTMONOBJ=rtmon.o

ALLOBJ=$(IPOBJ) $(RTMONOBJ)
SCRIPTS=ifcfg rtpr routel routef
# Foxonn modified start pling 09/08/2011, code shrink
#TARGETS=ip rtmon
TARGETS=ip
# Foxonn modfiied end pling 09/08/2011, code shrink

all: $(TARGETS) $(SCRIPTS)

ip: $(IPOBJ) $(LIBNETLINK) $(LIBUTIL)

rtmon: $(RTMONOBJ) $(LIBNETLINK)

install: all
	install -m 0755 $(TARGETS) $(DESTDIR)$(SBINDIR)
	$(STRIP) $(DESTDIR)$(SBINDIR)/ip
# Foxonn removed start pling 09/08/2011, code shrink
#	$(STRIP) $(DESTDIR)$(SBINDIR)/rtmon
#	install -m 0755 $(SCRIPTS) $(DESTDIR)$(SBINDIR)
# Foxonn removed end pling 09/08/2011, code shrink

clean:
	rm -f $(ALLOBJ) $(TARGETS)

