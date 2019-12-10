#
# makefile to compile all programs for the AED.Project
#

occurrences:	occurrences.c 
	cc -Wall -O2 occurrences.c -o occurrences -lm

