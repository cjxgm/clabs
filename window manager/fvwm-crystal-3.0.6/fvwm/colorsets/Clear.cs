# Midnight colorset
# Written by: Rafal Bisingier <ravbc@man.poznan.pl>

SetEnv Colorset_Name "Clear"
SetEnv Colorset_Author "ravbc"

# What colorsets do we use?
Read $./colorset-definition

DefaultColors rgb:88/88/88 rgb:11/11/11

# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# QuakeConsole, thumbnailed windows
Colorset $[cs_quakeconsole] \
Foreground rgb:00/00/00, Background rgb:00/00/00, \
Hilight rgb:00/00/00, Shadow rgb:00/00/00, \
IconAlpha 70

Colorset $[cs_panel_inactive] \
Foreground rgb:cc/cc/cc, Background rgb:1c/1c/1c, \
Hilight rgb:55/55/55, Shadow rgb:55/55/55, \
RootTransparent, Tint rgb:1c/1c/1c 20, \
IconAlpha 60, IconTint rgb:00/00/00 5

Colorset $[cs_panel_active] \
Foreground rgb:ff/ff/ff, Background rgb:13/08/70, \
Hilight rgb:16/0c/6a, Shadow rgb:16/0c/6a, \
RootTransparent, Tint rgb:13/08/70 70

Colorset $[cs_panel_wininactive] \
Foreground rgb:99/99/99, Background rgb:00/00/00, \
Hilight rgb:33/33/33, Shadow rgb:33/33/33, \
RootTransparent, Tint rgb:99/99/99 50

Colorset $[cs_panel_winactive] \
Foreground rgb:ff/ff/ff, Background rgb:00/33/66, \
Hilight rgb:44/66/aa, Shadow rgb:44/66/aa, \
RootTransparent, Tint rgb:00/00/ff 30

Colorset $[cs_panel_border] \
Background rgb:44/44/44

Colorset $[cs_menu_inactive] \
Foreground rgb:ff/ff/ff, Background rgb:10/10/10, \
Hilight rgb:33/56/56, Shadow rgb:33/56/56, Plain, NoShape

Colorset $[cs_menu_active] \
Foreground rgb:00/00/00, Background rgb:77/88/99, \
Hilight rgb:55/55/55, Shadow rgb:55/55/55, Plain, NoShape

# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# Additional variables
SetEnv ThumbBorderColor rgb:99/99/99
SetEnv ExternalFontColor rgb:99/99/99

SetEnv TrayerTint "0x1c1c1c"
SetEnv TrayerAlpha "205"

