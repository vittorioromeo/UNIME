#!/bin/bash

TMPFILE="/tmp/tmp.f"
TMPEXEC="/tmp/tmp.x"

# echo -e -n "\t" > $TMPFILE
# sed -r ':a;N;$!ba;s/\n/\n\t/g' $1 >> $TMPFILE
cat $1 > $TMPFILE
gcc -lgfortran $TMPFILE -o $TMPEXEC && $TMPEXEC