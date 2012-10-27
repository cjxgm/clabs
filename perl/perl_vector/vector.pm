#!/usr/bin/perl

use strict;
use warnings;

package Vector;

use Carp;
use overload
	'""'	=> \&_stringify,
	'neg'	=> \&_neg,
	'+'		=> \&_add,
	'-'		=> \&_sub,
	'*'		=> \&_mul,
	'/'		=> \&_div,
	'.'		=> \&_dot,
	'x'		=> \&_cross,
	'!'		=> \&_mod,
	'~'		=> \&_sqr,
	'**'	=> \&_power,
;


sub new
{
	my $me = shift;
	my $x = shift || 0;
	my $y = shift || 0;
	my $z = shift || 0;
	bless {
		x => $x,
		y => $y,
		z => $z,
	} => $me;
}

sub clone
{
	my $me = shift;
	Vector->new($me->{x}, $me->{y}, $me->{z});
}


sub _stringify
{
	my $me = shift;
	"Vector->new($me->{x}, $me->{y}, $me->{z})";
}

sub _neg
{
	my $me = shift;
	Vector->new(-$me->{x}, -$me->{y}, -$me->{z});
}

sub _add
{
	my ($me, $other, $swap) = @_;
	croak "[+] Vector expected." if ref$other ne 'Vector';
	Vector->new($me->{x}+$other->{x},
			$me->{y}+$other->{y},
			$me->{z}+$other->{z});
}

sub _sub
{
	my ($me, $other, $swap) = @_;
	croak "[-] Vector expected." if ref$other ne 'Vector';
	Vector->new($me->{x}-$other->{x},
			$me->{y}-$other->{y},
			$me->{z}-$other->{z});
}

sub _mul
{
	my ($me, $other, $swap) = @_;
	croak "[*] Vector unexpected." if ref$other eq 'Vector';
	Vector->new($me->{x}*$other, $me->{y}*$other, $me->{z}*$other);
}

sub _div
{
	my ($me, $other, $swap) = @_;
	croak "[/] Vector unexpected." if ref$other eq 'Vector';
	$other or do {
		carp "!! divided by zero.";
		return Vector->new;
	};
	$me * (1 / $other);
}

sub _dot
{
	my ($me, $other, $swap) = @_;
	ref$other ne 'Vector' and do {
		return $other . "$me" if $swap;
		return "$me" . $other;
	};
	$me->{x}*$other->{x} + $me->{y}*$other->{y} + $me->{z}*$other->{z};
}

sub _cross
{
	my ($me, $other, $swap) = @_;
	return "$me" x $other if ref$other ne 'Vector';
	Vector->new($me->{y}*$other->{z} - $me->{z}*$other->{y},
			$me->{z}*$other->{x} - $me->{x}*$other->{z},
			$me->{x}*$other->{y} - $me->{y}*$other->{x});
}

sub _mod
{
	sqrt ~shift;
}

sub _sqr
{
	my $me = shift;
	$me . $me;
}

sub _power
{
	my ($me, $other, $swap) = @_;
	(~$me) ** ($other / 2);
}


1;

