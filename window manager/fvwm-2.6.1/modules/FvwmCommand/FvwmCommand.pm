# FvwmCommand.pm
# Collection of fvwm builtin commands for FvwmCommand
package FvwmCommand;
use Exporter;
@ISA=qw(Exporter);
@EXPORT=qw(
);

sub FvwmCommand { system "/usr/local/bin/FvwmCommand '@_'"}

sub AM { FvwmCommand "+ @_ " }
1;
