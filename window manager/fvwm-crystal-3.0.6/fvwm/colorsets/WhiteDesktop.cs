# WhiteDesktop colorset
# Written by: Social

SetEnv Colorset_Name "WhiteDesktop"
SetEnv Colorset_Author "Social"

# What colorsets do we use?
Read $./colorset-definition

# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# QuakeConsole, thumbnailed windows
Colorset $[cs_quakeconsole] \
Foreground "black", Background "black", \
Hilight "black", Shadow "black", \
IconAlpha 70


Colorset $[cs_panel_inactive] \
Foreground "black", Background "#444444", \
Tint "#aaaaaa" 50, RootTransparent, \
fgTint, IconAlpha, IconTint

Colorset $[cs_panel_active] \
Foreground "darkgray", Background "darkgray", \
Hilight "darkgray", Shadow "darkgray", \
Tint "#888888" 50, RootTransparent

Colorset $[cs_panel_wininactive] \
Foreground "#555555", Background "#777777", \
Hilight "#444444", Shadow "#444444", \
Tint "#aaaaaa" 60, RootTransparent

Colorset $[cs_panel_winactive] \
Foreground "black", Background "#777777", \
Hilight "#666666", Shadow "#666666", \
Tint "#aaaaaa" 80, RootTransparent

Colorset $[cs_panel_border] \
Background "#777777"

Colorset $[cs_menu_inactive] \
Foreground "black", Background "white", \
Hilight "white", Shadow "white", \
Tint "#aaaaaa" 50, RootTransparent

Colorset $[cs_menu_active] \
Foreground "#333333", Background "white", \
Hilight "#333333", Shadow "gray"


# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# Additional variables
SetEnv ThumbBorderColor "black"
SetEnv ExternalFontColor "gray40"

SetEnv TrayerTint "0xaaaaaa"
SetEnv TrayerAlpha "128"

