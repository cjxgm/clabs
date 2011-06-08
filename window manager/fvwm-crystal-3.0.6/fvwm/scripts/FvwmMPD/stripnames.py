#!/usr/bin/env python
import sys

if __name__ == "__main__":
    stripped = sys.argv[1]
    index=0
    while (index > -1):
        stripped = stripped[index+1:]
        index = stripped.find('/')
    print stripped
