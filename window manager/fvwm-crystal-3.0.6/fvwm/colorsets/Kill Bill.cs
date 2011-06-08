# Kill Bill colorset
# Written by: Maciej Delmanowski <harnir@linux.net.pl>

SetEnv Colorset_Name "Kill Bill"
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
Foreground "black", Background "black", \
Tint "yellow" 70, RootTransparent, \
fgTint, IconAlpha, IconTint

Colorset $[cs_panel_active] \
Foreground "black", Background "black", \
Hilight "black", Shadow "black", \
Tint "yellow" 40, RootTransparent

Colorset $[cs_panel_wininactive] \
Foreground "black", Background "black", \
Hilight "yellow", Shadow "yellow", \
Tint "gold" 30, RootTransparent

Colorset $[cs_panel_winactive] \
Foreground "black", Background "black", \
Hilight "yellow", Shadow "yellow", \
Tint "yellow" 90, RootTransparent

Colorset $[cs_panel_border] \
Background "yellow"

Colorset $[cs_menu_inactive] \
Foreground "black", Background "black", \
Hilight "black", Shadow "black", \
Tint "yellow" 70, RootTransparent

Colorset $[cs_menu_active] \
Foreground "black", Background "#b1a700", \
Hilight "red", Shadow "red"


# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# Additional variables
SetEnv ThumbBorderColor "yellow"
SetEnv ExternalFontColor "black"

SetEnv TrayerTint "0xFFFF00"
SetEnv TrayerAlpha "77"

