#!/bin/bash
make pdf && (chromium ./output/out.pdf &)&