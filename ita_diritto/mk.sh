#!/bin/bash

pandoc --highlight-style pygments -V geometry:"top=3cm, bottom=3cm, left=2.5cm, right=2.5cm" --smart -t latex -s ./$1.md -o $1.pdf && chromium ./$1.pdf
