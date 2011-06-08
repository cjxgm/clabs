#@+leo-ver=4
#@+node:@file Makefile
#
# Makefile for pywm window manager
#

# set this to your Python 2.2 interpreter
PYTHONCMD=/usr/bin/python
BINDIR=/usr/bin

# set this to the full command for your favourite documentation generator
DOCCMD = epydoc -n "PYWM Reference Manual" --show-imports pywm

all:
	echo "Available targets:"
	echo "  pywm - build the pywm window manager"
	echo "  install - install pywm into python load path (requires root)"
	echo "  doc - generate pywm documentation"

pywm:
	$(PYTHONCMD) setup.py build

install:
	$(PYTHONCMD) setup.py install
	cp pywm-run.py $(BINDIR)

doc:
	$(DOCCMD)

#@-node:@file Makefile
#@-leo
