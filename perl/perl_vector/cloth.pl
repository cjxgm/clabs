#!/usr/bin/env perl
# see http://www.gamasutra.com/resource_guide/20030121/jacobson_03.shtml

use strict;
use warnings;
use vector;

use SDL;
use SDL::Event;
use SDLx::App;

my $app = SDLx::App->new(
		width  => 400,
		height => 400,
		title  => 'Cloth');

$app->add_event_handler(\&event);
#$app->add_move_handler (\&move);
$app->add_show_handler (\&render);
&init;
$app->run();

sub event {
	my $event      = shift;
	my $controller = shift;
	$controller->stop if $event->type == SDL_QUIT;
	if ($event->type == SDL_MOUSEMOTION) {
	#	$target->{x} = $event->button_x;
	#	$target->{y} = $event->button_y;
	}
}

sub init
{
}

sub drawp {
#	my $p = shift;
#	$app->draw_circle_filled([$p->{x}, $p->{y}], 2, [255, 0, 0, 255]);
}

sub render {
#	my ($delta, $app) = @_;
#	$app->draw_rect([0, 0, $app->w, $app->h], 0); # clear screen
#	foreach my $i (0..$#ps) {
#		drawp $ps[$i];
#	}
	$app->update();
}

