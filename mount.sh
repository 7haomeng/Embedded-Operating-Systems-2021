###################################PXA###################################
portmap&
mount -o tcp 192.168.1.55:/home/chinghaomeng/export_point /mnt

####################################ZC###################################

#Export the cross compiler for ZC702 and activate it when cross compiling
export ARCH=arm
export CROSS_COMPILE=arm-linux-gnueabihf-
source /tools/Xilinx/SDK/2018.3/settings64.sh

#Network IP
ip link set eth0 up
ip addr add 192.168.1.202/24 dev eth0
ping -c 4 192.168.1.55

#NFS
mkdir /mnt
mount -o tcp,nolock 192.168.1.55:/home/chinghaomeng/export_point /mnt
cd /mnt

#Create Device
mknod /dev/mydev c 244 0
