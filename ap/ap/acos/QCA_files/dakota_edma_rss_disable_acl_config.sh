# Disable EDMA RSS 
devmem 0x0c080894 32 0x0

# Set int_pri ro Rx ring mapping: 0 -> 0; 1 -> 2; 2 -> 4; 3 -> 6; 4 -> 2; 5 -> 4; 6 -> 6; 7 -> 0
#     0100 0000 0110 0010 0110 0100 0010 0000
devmem 0x0c080820 32 0x40626420

ssdk_sh acl list create 0 0

# IPv4 TCP/UDP
ssdk_sh acl rule add 0 0   1 ip4 n n n n n n n n n n n n n n n y 0.0.0.0 0.0.0.1 y 0.0.0.0 0.0.0.1            n n n n y mask 0x0000 0x0001   y mask 0x0000 0x0001 n n n n n y     n n n n n n n y 0      n n 0 0 n n n n n n n n n n n n n
ssdk_sh acl rule add 0 1   1 ip4 n n n n n n n n n n n n n n n y 0.0.0.0 0.0.0.1 y 0.0.0.0 0.0.0.1            n n n n y mask 0x0000 0x0001   y mask 0x0001 0x0001 n n n n n y     n n n n n n n y 1      n n 0 0 n n n n n n n n n n n n n
ssdk_sh acl rule add 0 2   1 ip4 n n n n n n n n n n n n n n n y 0.0.0.0 0.0.0.1 y 0.0.0.0 0.0.0.1            n n n n y mask 0x0001 0x0001   y mask 0x0000 0x0001 n n n n n y     n n n n n n n y 2      n n 0 0 n n n n n n n n n n n n n
ssdk_sh acl rule add 0 3   1 ip4 n n n n n n n n n n n n n n n y 0.0.0.0 0.0.0.1 y 0.0.0.0 0.0.0.1            n n n n y mask 0x0001 0x0001   y mask 0x0001 0x0001 n n n n n y     n n n n n n n y 3      n n 0 0 n n n n n n n n n n n n n
ssdk_sh acl rule add 0 4   1 ip4 n n n n n n n n n n n n n n n y 0.0.0.0 0.0.0.1 y 0.0.0.1 0.0.0.1            n n n n y mask 0x0000 0x0001   y mask 0x0000 0x0001 n n n n n y     n n n n n n n y 4      n n 0 0 n n n n n n n n n n n n n
ssdk_sh acl rule add 0 5   1 ip4 n n n n n n n n n n n n n n n y 0.0.0.0 0.0.0.1 y 0.0.0.1 0.0.0.1            n n n n y mask 0x0000 0x0001   y mask 0x0001 0x0001 n n n n n y     n n n n n n n y 5      n n 0 0 n n n n n n n n n n n n n
ssdk_sh acl rule add 0 6   1 ip4 n n n n n n n n n n n n n n n y 0.0.0.0 0.0.0.1 y 0.0.0.1 0.0.0.1            n n n n y mask 0x0001 0x0001   y mask 0x0000 0x0001 n n n n n y     n n n n n n n y 6      n n 0 0 n n n n n n n n n n n n n
ssdk_sh acl rule add 0 7   1 ip4 n n n n n n n n n n n n n n n y 0.0.0.0 0.0.0.1 y 0.0.0.1 0.0.0.1            n n n n y mask 0x0001 0x0001   y mask 0x0001 0x0001 n n n n n y     n n n n n n n y 7      n n 0 0 n n n n n n n n n n n n n 
ssdk_sh acl rule add 0 8   1 ip4 n n n n n n n n n n n n n n n y 0.0.0.1 0.0.0.1 y 0.0.0.0 0.0.0.1            n n n n y mask 0x0000 0x0001   y mask 0x0000 0x0001 n n n n n y     n n n n n n n y 7      n n 0 0 n n n n n n n n n n n n n
ssdk_sh acl rule add 0 9   1 ip4 n n n n n n n n n n n n n n n y 0.0.0.1 0.0.0.1 y 0.0.0.0 0.0.0.1            n n n n y mask 0x0000 0x0001   y mask 0x0001 0x0001 n n n n n y     n n n n n n n y 6      n n 0 0 n n n n n n n n n n n n n
ssdk_sh acl rule add 0 10  1 ip4 n n n n n n n n n n n n n n n y 0.0.0.1 0.0.0.1 y 0.0.0.0 0.0.0.1            n n n n y mask 0x0001 0x0001   y mask 0x0000 0x0001 n n n n n y     n n n n n n n y 5      n n 0 0 n n n n n n n n n n n n n
ssdk_sh acl rule add 0 11  1 ip4 n n n n n n n n n n n n n n n y 0.0.0.1 0.0.0.1 y 0.0.0.0 0.0.0.1            n n n n y mask 0x0001 0x0001   y mask 0x0001 0x0001 n n n n n y     n n n n n n n y 4      n n 0 0 n n n n n n n n n n n n n
ssdk_sh acl rule add 0 12  1 ip4 n n n n n n n n n n n n n n n y 0.0.0.1 0.0.0.1 y 0.0.0.1 0.0.0.1            n n n n y mask 0x0000 0x0001   y mask 0x0000 0x0001 n n n n n y     n n n n n n n y 3      n n 0 0 n n n n n n n n n n n n n
ssdk_sh acl rule add 0 13  1 ip4 n n n n n n n n n n n n n n n y 0.0.0.1 0.0.0.1 y 0.0.0.1 0.0.0.1            n n n n y mask 0x0000 0x0001   y mask 0x0001 0x0001 n n n n n y     n n n n n n n y 2      n n 0 0 n n n n n n n n n n n n n
ssdk_sh acl rule add 0 14  1 ip4 n n n n n n n n n n n n n n n y 0.0.0.1 0.0.0.1 y 0.0.0.1 0.0.0.1            n n n n y mask 0x0001 0x0001   y mask 0x0000 0x0001 n n n n n y     n n n n n n n y 1      n n 0 0 n n n n n n n n n n n n n
ssdk_sh acl rule add 0 15  1 ip4 n n n n n n n n n n n n n n n y 0.0.0.1 0.0.0.1 y 0.0.0.1 0.0.0.1            n n n n y mask 0x0001 0x0001   y mask 0x0001 0x0001 n n n n n y     n n n n n n n y 0      n n 0 0 n n n n n n n n n n n n n 

# IPv6 TCP/UDP

ssdk_sh acl rule add 0 16 1 ip6 n n n n n n n n n n n n n n n y 0000::0000 0000::0001 y 0000::0000 0000::0001 n n n n y mask 0x0000 0x0001   y mask 0x0001 0x0001 n n n n n y     n n n n n n n y 1      n n 0 0 n n n n n n n n n n n n n
ssdk_sh acl rule add 0 17 1 ip6 n n n n n n n n n n n n n n n y 0000::0000 0000::0001 y 0000::0000 0000::0001 n n n n y mask 0x0001 0x0001   y mask 0x0000 0x0001 n n n n n y     n n n n n n n y 2      n n 0 0 n n n n n n n n n n n n n
ssdk_sh acl rule add 0 18 1 ip6 n n n n n n n n n n n n n n n y 0000::0000 0000::0001 y 0000::0000 0000::0001 n n n n y mask 0x0001 0x0001   y mask 0x0001 0x0001 n n n n n y     n n n n n n n y 3      n n 0 0 n n n n n n n n n n n n n
ssdk_sh acl rule add 0 19 1 ip6 n n n n n n n n n n n n n n n y 0000::0000 0000::0001 y 0000::0001 0000::0001 n n n n y mask 0x0000 0x0001   y mask 0x0000 0x0001 n n n n n y     n n n n n n n y 4      n n 0 0 n n n n n n n n n n n n n
ssdk_sh acl rule add 0 20 1 ip6 n n n n n n n n n n n n n n n y 0000::0000 0000::0001 y 0000::0000 0000::0001 n n n n y mask 0x0000 0x0001   y mask 0x0000 0x0001 n n n n n y     n n n n n n n y 0      n n 0 0 n n n n n n n n n n n n n
ssdk_sh acl rule add 0 21 1 ip6 n n n n n n n n n n n n n n n y 0000::0000 0000::0001 y 0000::0001 0000::0001 n n n n y mask 0x0000 0x0001   y mask 0x0001 0x0001 n n n n n y     n n n n n n n y 5      n n 0 0 n n n n n n n n n n n n n
ssdk_sh acl rule add 0 22 1 ip6 n n n n n n n n n n n n n n n y 0000::0000 0000::0001 y 0000::0001 0000::0001 n n n n y mask 0x0001 0x0001   y mask 0x0000 0x0001 n n n n n y     n n n n n n n y 6      n n 0 0 n n n n n n n n n n n n n
ssdk_sh acl rule add 0 23 1 ip6 n n n n n n n n n n n n n n n y 0000::0000 0000::0001 y 0000::0001 0000::0001 n n n n y mask 0x0001 0x0001   y mask 0x0001 0x0001 n n n n n y     n n n n n n n y 7      n n 0 0 n n n n n n n n n n n n n
ssdk_sh acl rule add 0 24 1 ip6 n n n n n n n n n n n n n n n y 0000::0001 0000::0001 y 0000::0000 0000::0001 n n n n y mask 0x0000 0x0001   y mask 0x0000 0x0001 n n n n n y     n n n n n n n y 7      n n 0 0 n n n n n n n n n n n n n
ssdk_sh acl rule add 0 25 1 ip6 n n n n n n n n n n n n n n n y 0000::0001 0000::0001 y 0000::0000 0000::0001 n n n n y mask 0x0000 0x0001   y mask 0x0001 0x0001 n n n n n y     n n n n n n n y 6      n n 0 0 n n n n n n n n n n n n n
ssdk_sh acl rule add 0 26 1 ip6 n n n n n n n n n n n n n n n y 0000::0001 0000::0001 y 0000::0000 0000::0001 n n n n y mask 0x0001 0x0001   y mask 0x0000 0x0001 n n n n n y     n n n n n n n y 5      n n 0 0 n n n n n n n n n n n n n
ssdk_sh acl rule add 0 27 1 ip6 n n n n n n n n n n n n n n n y 0000::0001 0000::0001 y 0000::0000 0000::0001 n n n n y mask 0x0001 0x0001   y mask 0x0001 0x0001 n n n n n y     n n n n n n n y 4      n n 0 0 n n n n n n n n n n n n n
ssdk_sh acl rule add 0 28 1 ip6 n n n n n n n n n n n n n n n y 0000::0001 0000::0001 y 0000::0001 0000::0001 n n n n y mask 0x0000 0x0001   y mask 0x0000 0x0001 n n n n n y     n n n n n n n y 3      n n 0 0 n n n n n n n n n n n n n
ssdk_sh acl rule add 0 29 1 ip6 n n n n n n n n n n n n n n n y 0000::0001 0000::0001 y 0000::0001 0000::0001 n n n n y mask 0x0000 0x0001   y mask 0x0001 0x0001 n n n n n y     n n n n n n n y 2      n n 0 0 n n n n n n n n n n n n n
ssdk_sh acl rule add 0 30 1 ip6 n n n n n n n n n n n n n n n y 0000::0001 0000::0001 y 0000::0001 0000::0001 n n n n y mask 0x0001 0x0001   y mask 0x0000 0x0001 n n n n n y     n n n n n n n y 1      n n 0 0 n n n n n n n n n n n n n
ssdk_sh acl rule add 0 31 1 ip6 n n n n n n n n n n n n n n n y 0000::0001 0000::0001 y 0000::0001 0000::0001 n n n n y mask 0x0001 0x0001   y mask 0x0001 0x0001 n n n n n y     n n n n n n n y 0      n n 0 0 n n n n n n n n n n n n n

ssdk_sh acl list bind 0 0 0 1
ssdk_sh acl list bind 0 0 0 2
ssdk_sh acl list bind 0 0 0 3
ssdk_sh acl list bind 0 0 0 4
ssdk_sh acl list bind 0 0 0 5



