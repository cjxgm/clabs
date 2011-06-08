# CrystalZono colorset
# Written by: domi.no (domi.no@terra.es)

SetEnv Colorset_Name "CrystalZono"
SetEnv Colorset_Author "domi.no (domi.no@terra.es)"

# What colorsets do we use?
Read $./colorset-definition

# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# QuakeConsole, thumbnailed windows
Colorset $[cs_quakeconsole] \
Foreground "black", Background "black", \
Hilight "black", Shadow "black", \
IconAlpha 50


Colorset $[cs_panel_inactive] \
Foreground "black", Background "white", \
Tint "white" 0, RootTransparent, \
fgTint, IconAlpha, IconTint

Colorset $[cs_panel_active] \
Foreground "white", Background "darkgray", \
Hilight "darkgray", Shadow "darkgray", \
Tint "gray" 50, RootTransparent

Colorset $[cs_panel_wininactive] \
Foreground "#555555", Background "#777777", \
Hilight "gray", Shadow "gray", \
Tint "white" 0, RootTransparent

Colorset $[cs_panel_winactive] \
Foreground "black", Background "#777777", \
Hilight "black", Shadow "black", \
Tint "white" 0, RootTransparent

Colorset $[cs_panel_border] \
Background "#777777"

Colorset $[cs_menu_inactive] \
Foreground "black", Background "#777777", \
Hilight "#777777", Shadow "#777777", \
Tint "white" 0, RootTransparent

Colorset $[cs_menu_active] \
Foreground "black", Background "gray", \
Hilight "black", Shadow "gray"


# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# Additional variables
SetEnv ThumbBorderColor "gray"
SetEnv ExternalFontColor "gray40"

SetEnv TrayerTint "0xFFFFFF"
SetEnv TrayerAlpha "256"

