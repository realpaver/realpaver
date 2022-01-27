# This file is part of Realpaver. License: see COPYING file.

.PHONY: html

all: html pdf

html: Doxyfile

if DOXYGEN
	doxygen ./Doxyfile
else
	@echo Sorry, doxygen does not seem to be present on your system
endif

DISTCLEANFILES = html/*
CLEANFILES = html/*
MAINTAINERCLEANFILES = html/*
