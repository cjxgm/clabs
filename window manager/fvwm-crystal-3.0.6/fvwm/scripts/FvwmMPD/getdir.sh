#!/usr/bin/env bash
if [ $2 = '..' ]; then
    `dirname $0`/getprevdir.py "${1}"
else
    echo "`mpc ls "$1"|head -n$2|tail -n1`"
fi
