#!/bin/bash

cat patches.h | grep '#define' | grep -e '1$' | sort
