#!/bin/sh

set -x
# set -e

rmmod -f mydev
insmod mydev.ko

./writer HAO &
./reader 192.168.1.55 8888 /dev/mydev
