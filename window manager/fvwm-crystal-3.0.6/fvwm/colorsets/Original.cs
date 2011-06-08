# Original FVWM-Crystal colorset
# Written by: Maciej Delmanowski <harnir@berlios.de>

SetEnv Colorset_Name "Original"
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
Foreground "white", Background "black", \
Tint "blue" 30, RootTransparent, \
fgTint, IconAlpha, IconTint

Colorset $[cs_panel_active] \
Foreground "white", Background "black", \
Hilight "black", Shadow "black", \
Tint "red" 30, RootTransparent

Colorset $[cs_panel_wininactive] \
Foreground "white", Background "gray", \
Hilight "gray", Shadow "gray", \
Tint "yellow" 30, RootTransparent

Colorset $[cs_panel_winactive] \
Foreground "white", Background "gray", \
Hilight "gray", Shadow "gray", \
Tint "green" 30, RootTransparent

Colorset $[cs_panel_border] \
Background "gray"

Colorset $[cs_menu_inactive] \
Foreground "white", Background "gray", \
Hilight "gray", Shadow "gray", \
Tint "blue" 30, RootTransparent

Colorset $[cs_menu_active] \
Foreground "white", Background "darkred", \
Hilight "gray", Shadow "gray"


# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# Additional variables
SetEnv ThumbBorderColor "gray"
SetEnv ExternalFontColor "gray40"

SetEnv TrayerTint "0x0000FF"
SetEnv TrayerAlpha "140"

