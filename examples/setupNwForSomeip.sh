sudo route add -net 224.0.0.0 netmask 240.0.0.0 dev lo
sudo ifconfig lo multicast
sudo ufw disable
sudo ufw status verbose
# replace network-interface with correct interface, eg: eth0, en0
sudo route add -nv 224.224.224.245 dev <network-interface>