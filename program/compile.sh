#!/bin/bash

export BRPATH=/home/dev/BR_Internet_Radio/QemuVirt64/buildroot-2021.02

export PATH=$BRPATH/output/host/usr/bin:$PATH
make ARCH=arm CROSS_COMPILE=aarch64-none-linux-gnu- morse
