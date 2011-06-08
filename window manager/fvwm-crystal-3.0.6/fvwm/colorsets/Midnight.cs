# Midnight colorset
# Written by: Maciej Delmanowski <harnir@linux.net.pl>

SetEnv Colorset_Name "Midnight"
SetEnv Colorset_Author "Maciej Delmanowski <harnir@linux.net.pl>"

# What colorsets do we use?
Read $./colorset-definition

# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# QuakeConsole, thumbnailed windows
Colorset $[cs_quakeconsole] \
Foreground "black", Background "black", \
Hilight "black", Shadow "black", \
IconAlpha 70


Colorset $[cs_panel_inactive] \
Foreground "gray", Background "#444444", \
Tint "black" 20, RootTransparent, \
fgTint, IconAlpha, IconTint

Colorset $[cs_panel_active] \
Foreground "darkgray", Background "darkgray", \
Hilight "darkgray", Shadow "darkgray", \
Tint "black" 40, RootTransparent

Colorset $[cs_panel_wininactive] \
Foreground "#444444", Background "#444444", \
Hilight "#444444", Shadow "#444444", \
Tint "black" 30, RootTransparent

Colorset $[cs_panel_winactive] \
Foreground "gray", Background "gray", \
Hilight "#777777", Shadow "#777777", \
Tint "black" 60, RootTransparent

Colorset $[cs_panel_border] \
Background "#444444"

Colorset $[cs_menu_inactive] \
Foreground "lightgray", Background "gray", \
Hilight "#444444", Shadow "#444444", \
Tint "black" 30, RootTransparent

Colorset $[cs_menu_active] \
Foreground "lightgray", Background "#666666", \
Hilight "#333333", Shadow "darkgray"


# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# Additional variables
SetEnv ThumbBorderColor "gray"
SetEnv ExternalFontColor "gray40"

SetEnv TrayerTint "0X000000"
SetEnv TrayerAlpha "205"
