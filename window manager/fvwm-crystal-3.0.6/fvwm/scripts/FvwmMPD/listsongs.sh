#!/usr/bin/env bash
if [[ $2 = 'files' ]]; then
    mpc ls "$1"|grep -o -e ".*.mp3\|.*.ogg\|.*.flac"| while read line; do
	`dirname $0`/stripnames.py "${line}"
    done    
else
    mpc ls "$1"|grep -v -e ".*.mp3\|.*.ogg\|.*.flac"| while read line; do
	echo ${line}
    done
fi
