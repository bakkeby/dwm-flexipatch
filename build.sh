#!/bin/bash
set -e

notify-send -u critical 'Compiling and installing/updating dwm...' && \
DESTDIR=~/local PREFIX='' make clean install && \
sudo cp -farv ~/local/bin/dwm /bin/dwm && ls -lha /bin/dwm && \
notify-send -u critical "DWM will be restarted now..." && \
pkill -HUP dwm
