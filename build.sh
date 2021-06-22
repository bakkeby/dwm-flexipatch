#!/bin/bash
set -e

notify-send 'Generating final config...'
./generate-final-config.sh
notify-send -u critical 'Compiling and installing/updating dwm...' && \
make clean install && \
sudo cp -farv $HOME/local/bin/dwm /bin/dwm && sudo ls -lha /bin/dwm && \
notify-send -u critical "DWM will be restarted now..." && \
pkill -HUP dwm
