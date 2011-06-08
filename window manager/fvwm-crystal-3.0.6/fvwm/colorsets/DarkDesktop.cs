# DarkDesktop colorset
# Written by: fRiQ

SetEnv Colorset_Name "DarkDesktop"
SetEnv Colorset_Author "fRiQ"

# What colorsets do we use?
Read $./colorset-definition

# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# QuakeConsole, thumbnailed windows
Colorset $[cs_quakeconsole] \
Foreground "black", Background "black", \
Hilight "black", Shadow "black", \
IconAlpha 70


Colorset $[cs_panel_inactive] \
Foreground "white", Background "#444444", \
Tint "black" 50, RootTransparent, \
fgTint, IconAlpha, IconTint

Colorset $[cs_panel_active] \
Foreground "darkgray", Background "darkgray", \
Hilight "darkgray", Shadow "darkgray", \
Tint "#000066" 50, RootTransparent

Colorset $[cs_panel_wininactive] \
Foreground "gray", Background "#777777", \
Hilight "#444444", Shadow "#444444", \
Tint "black" 50, RootTransparent

Colorset $[cs_panel_winactive] \
Foreground "white", Background "#777777", \
Hilight "#666666", Shadow "#666666", \
Tint "blue" 50, RootTransparent

Colorset $[cs_panel_border] \
Background "#777777"

Colorset $[cs_menu_inactive] \
Foreground "white", Background "#777777", \
Hilight "#777777", Shadow "#777777", \
Tint "black" 50, RootTransparent

Colorset $[cs_menu_active] \
Foreground "black", Background "gray", \
Hilight "black", Shadow "gray"


# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# Additional variables
SetEnv ThumbBorderColor "gray"
SetEnv ExternalFontColor "gray40"

SetEnv TrayerTint "0x000000"
SetEnv TrayerAlpha "128"

