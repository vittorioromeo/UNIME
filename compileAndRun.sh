#!/bin/bash

sudo ./installVeeLib.sh
gcc -lm -o /tmp/a.out "$1" && "/tmp/a.out"