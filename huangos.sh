#!/bin/bash
rm -rf HuangOS
cp -r /mnt/os/HuangOS ./
chown -R huangwf:huangwf HuangOS
cd HuangOS/src
make
cd ../../
