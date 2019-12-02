#
# makefile to compile all programs for the AED.Project
#

.PHONY:		all
all:		occurrences manipulate_file

occurrences:	occurrences.c 
	cc -Wall -g occurrences.c -o occurrences

