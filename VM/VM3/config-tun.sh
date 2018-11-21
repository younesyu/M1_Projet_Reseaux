#!/bin/sh
ip route delete 172.16.2.128/28
ip route delete 172.16.2.160/28
ip route delete 172.16.2.144/28
ip route add 172.16.2.144/28 via 172.16.2.186 dev eth2  #-->LAN4

