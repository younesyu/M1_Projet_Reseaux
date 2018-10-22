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

## Configuration de VM1
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
    - name: eth3
    - routes:
      - name: LAN2
        ipaddr: 172.16.2.160/28
        gateway: 172.16.2.132

## Configuration de la route vers LAN1-6 via VM1-6
routes:
  network.routes:
    - name: eth4
    - routes:
      - name: LAN1-6
        ipaddr:  fc00:1234:1::/64
        gateway: 172.16.2.156

