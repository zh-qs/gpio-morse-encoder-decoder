#!/bin/bash

export BRPATH=/malina/turs/buildroot-2022.02

export PATH=$BRPATH/output/host/usr/bin:$PATH
make ARCH=arm CROSS_COMPILE=aarch64-none-linux-gnu- morse
