#!/usr/bin/env perl
while (<STDIN>) {
	if (/hello\s(\w+)/i) {
		print "say hello to $1\n"
	}
	elsif (/goodbye\s(\w+)/i) {
		print "say goodbye to $1\n"
	}
}
