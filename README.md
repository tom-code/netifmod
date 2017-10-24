# README #

minimalistic network interface template


### use steps ###
* make
* insmod ./n1.ko
* ip link add type tx
* ip route add 1.2.3.4 dev tx0
* ping 1.2.3.4
* ip link del tx0
* rmmod n1
