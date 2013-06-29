#!/usr/bin/perl -w

my $src = <<'EOF';
#include <Elementary.h>
#include "util.h"


// simpler callback add
#define $$$$($object, $event, $callback, $arg) \
	evas_object_smart_callback_add($object, $event, \
			(void *)($callback), (void *)($arg))
#define $$$($object, $event, $callback, $arg) \
	evas_object_event_callback_add($object, $event, \
			(void *)($callback), (void *)($arg))

static Evas_Object * win;
EAPI_MAIN int elm_main(int argc, char * argv[]);

ELM_MAIN()

EAPI_MAIN int elm_main(int argc, char * argv[])
{
	win = elm_win_util_standard_add("test-elm", "test-elm");
	evas_object_resize(win, 800, 600);
	$$$$(win, "delete,request", &elm_exit, NULL);
EOF
while (<>) { $src .= $_ }
$src .= <<'EOF';
	evas_object_show(win);

	elm_run();
	elm_shutdown();

	return 0;
}
EOF

open F, '>/tmp/test-elm.c';
print F $src;
close F;

my $flags = `(pkg-config --cflags --libs elementary)`;
chomp $flags;
$flags .= '-Ilibcrude -Llibcrude -lcrude';

`rm -f /tmp/test-elm`;
`gcc -o /tmp/test-elm /tmp/test-elm.c $flags`;
`/tmp/test-elm`;

