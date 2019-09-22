#!/bin/bash
#
#
# ------------------------------------------------------------------
# [Author] Title
#          Description
# ------------------------------------------------------------------

echo BB-I2C1 > /sys/devices/bone_capemgr.9/slots
echo BB-UART4 > /sys/devices/bone_capemgr.9/slots
ls -l /sys/bus/i2c/devices/i2c-*
ls -l /dev/ttyO*


