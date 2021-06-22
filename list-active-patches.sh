#!/bin/bash

cat patches.h | grep '#define' | grep 1 | sort
