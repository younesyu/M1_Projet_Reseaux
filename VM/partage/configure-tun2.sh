#!/bin/sh

ip addr add 172.16.2.1/28 dev tun0
ip link set tun0 up
sysctl -w net.ipv4.ip_forward=1
ip route delete 172.16.2.128/28
ip route delete 172.16.2.160/28
ip route delete 172.16.2.144/28
ip route add 172.16.2.144/28 via 172.16.2.10 dev tun0
