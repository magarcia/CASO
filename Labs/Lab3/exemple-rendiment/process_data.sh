#!/usr/bin/env bash

# Test usb write
for i in `seq 100 +5 500`
do
    echo $i $(grep $i usb-test-write.dat | cut -f2 | awk '{ total+=$1; count+=1} END {print total/count}')
done >> usb.dat

# Test hdd write
for i in `seq 100 +5 500`
do
    echo $i $(grep $i hdd-test-write.dat | cut -f2 | awk '{ total+=$1; count+=1} END {print total/count}')
done >> hdd.dat
