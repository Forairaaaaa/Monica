#!/bin/bash
# This script is a setup to allow non-root users to access the device automatically
# Should be run before the USB device attach
# Details: https://github.com/dorssel/usbipd-win/wiki/WSL-support

sudo service udev restart
sudo udevadm control --reload