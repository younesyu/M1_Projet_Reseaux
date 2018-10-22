# Configuration eth1
# RAPPEL: eth0 est à vagrant, ne pas y toucher

## Désactivation de network-manager
NetworkManager:
  service:
    - dead
    - enable: False
    
## Suppression de la passerelle par défaut
ip route del default:
  cmd:
    - run

##Configuration de VM1
eth1:
  network.managed:
    - enabled: True
    - type: eth
    - proto: none
    - ipaddr: 172.16.2.131
    - netmask: 28

eth2:
  network.managed:
    - enabled: True
    - type: eth
    - proto: none
    - ipaddr: 172.16.2.151
    - netmask: 28

## Configuration de la route vers LAN2 via VM2
routes:
  network.routes:
    - name: eth1
    - routes:
      - name: LAN2
        ipaddr: 172.16.2.160/28
        gateway: 172.16.2.132

## Configuration de la route vers LAN1-6 via VM1-6
routes:
  network.routes:
    - name: eth2
    - routes:
      - name: LAN16
        ipaddr: fc00:1234:1::/64
        gateway: 172.16.2.56

## Configuration de la route vers LAN2-6 via VM1-6
routes:
  network.routes:
    - name: eth3
    - routes:
      - name: LAN26
        ipaddr: fc00:1234:2::/64
        gateway: 172.16.2.56

## Configuration de la route vers LAN4 via VM2
routes:
  network.routes:
    - name: eth4
    - routes:
      - name: LAN4
        ipaddr: 172.16.2.176/28
        gateway: 172.16.2.132






