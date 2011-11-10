#!/bin/sh

rmmod /ppc-dump.ko
insmod /ppc-dump.ko
rm -rf /dev/ppc_dump

dmesg| tail | grep major

major=`dmesg| tail | grep major | awk '{print $1}' | awk -F":"  '{print $2}'`
minor=`dmesg| tail | grep major | awk '{print $2}' | awk -F":"  '{print $2}'`

major=`echo $major | awk '{print $1}'`
minor=`echo $minor | awk '{print $1}'`

echo "major:$major minor:$minor"

mknod /dev/ppc_dump c $major $minor

cat /dev/ppc_dump
