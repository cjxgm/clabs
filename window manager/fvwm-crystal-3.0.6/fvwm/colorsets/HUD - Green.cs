# HUD - Green colorset
# Written by: Maciej Delmanowski <harnir@linux.net.pl>

SetEnv Colorset_Name "HUD - Green"
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
Foreground "#04ba00", Background "black", \
Tint, RootTransparent, \
fgTint, IconAlpha, IconTint

Colorset $[cs_panel_active] \
Foreground "green", Background "darkgray", \
Hilight "darkgray", Shadow "darkgray", \
Tint "#04ba00" 40, RootTransparent

Colorset $[cs_panel_wininactive] \
Foreground "green", Background "black", \
Hilight "#444444", Shadow "#444444", \
Tint "#04ba00" 20, RootTransparent

Colorset $[cs_panel_winactive] \
Foreground "green", Background "black", \
Hilight "#04ba00", Shadow "#04ba00", \
Tint "#04ba00" 60, RootTransparent

Colorset $[cs_panel_border] \
Background "#04ba00"

Colorset $[cs_menu_inactive] \
Foreground "#2bdc27", Background "gray", \
Hilight "#444444", Shadow "#444444", \
Tint "#04ba00" 30, RootTransparent

Colorset $[cs_menu_active] \
Foreground "lightgray", Background "#666666", \
Hilight "#333333", Shadow "darkgray"


# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# Additional variables
SetEnv ThumbBorderColor "gray"
SetEnv ExternalFontColor "green"

SetEnv TrayerTint "0x000000"
SetEnv TrayerAlpha "256"

