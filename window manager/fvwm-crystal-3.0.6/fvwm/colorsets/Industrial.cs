# Industrial colorset
# Written by: harnir

SetEnv Colorset_Name "Industrial"
SetEnv Colorset_Author "harnir"

# What colorsets do we use?
Read $./colorset-definition

# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# QuakeConsole, thumbnailed windows
Colorset $[cs_quakeconsole] \
Foreground "black", Background "black", \
Hilight "black", Shadow "black", \
IconAlpha 70


Colorset $[cs_panel_inactive] \
Foreground "black", Background "#f6f6f6", \
Tint "#f6f6f6" 100, RootTransparent, \
fgTint "black" 100, IconAlpha, IconTint

Colorset $[cs_panel_active] \
Foreground "white", Background "darkgray", \
Hilight "darkgray", Shadow "darkgray", \
Tint "#6384a3" 100, RootTransparent

Colorset $[cs_panel_wininactive] \
Foreground "black", Background "#f6f6f6", \
Hilight "#444444", Shadow "#444444", \
Tint "#f6f6f6" 100, RootTransparent

Colorset $[cs_panel_winactive] \
Foreground "white", Background "#9bb6d0", \
Hilight "#666666", Shadow "#666666", \
Tint "#9bb6d0" 100, RootTransparent

Colorset $[cs_panel_border] \
Background "#333333"

Colorset $[cs_menu_inactive] \
Foreground "black", Background "#f6f6f6", \
Hilight "black", Shadow "#777777", \
Tint "#f6f6f6" 100, RootTransparent

Colorset $[cs_menu_active] \
Foreground "white", Background "#6384a3", \
Hilight "white", Shadow "black"


# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# Additional variables
SetEnv ThumbBorderColor "black"
SetEnv ExternalFontColor "gray40"

SetEnv TrayerTint "0xf6f6f6"
SetEnv TrayerAlpha "0"

