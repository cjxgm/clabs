# DarkGreen colorset
# Written by: Axquan <axquan@arx.pl>

SetEnv Colorset_Name "DarkGreen"
SetEnv Colorset_Author "Axquan <axquan@arx.pl>"

# What colorsets do we use?
Read $./colorset-definition

# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# QuakeConsole, thumbnailed windows
Colorset $[cs_quakeconsole] \
Foreground "black", Background "black", \
Hilight "black", Shadow "black", \
IconAlpha 50

Colorset $[cs_panel_inactive] \
Foreground "white", Background "#444444", \
Tint "darkgreen" 50, RootTransparent, \
fgTint, IconAlpha, IconTint

Colorset $[cs_panel_active] \
Foreground "darkgray", Background "darkgray", \
Hilight "darkgray", Shadow "darkgray", \
Tint "#1b64a3" 50, RootTransparent

Colorset $[cs_panel_wininactive] \
Foreground "white", Background "gray", \
Hilight "gray", Shadow "gray", \
Tint "#56c847" 20, RootTransparent

Colorset $[cs_panel_winactive] \
Foreground "white", Background "gray", \
Hilight "gray", Shadow "gray", \
Tint "blue" 50, RootTransparent

Colorset $[cs_panel_border] \
Background "gray"

Colorset $[cs_menu_inactive] \
Foreground "white", Background "gray", \
Hilight "gray", Shadow "gray", \
Tint "darkgreen" 50, RootTransparent

Colorset $[cs_menu_active] \
Foreground "white", Background "#1b64a3", \
Hilight "white", Shadow "gray"


# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# Additional variables
SetEnv ThumbBorderColor "gray"
SetEnv ExternalFontColor "gray40"

SetEnv TrayerTint "0x006400"
SetEnv TrayerAlpha "128"

