#!/usr/bin/env bash

qemu-system-i386 -m 512 -net nic,model=rtl8139 -net user \
    -net user,hostfwd=tcp::5555-:22 \
    -drive cache=writeback,index=0,media=disk,file=$(echo debian-hurd-*.img)
