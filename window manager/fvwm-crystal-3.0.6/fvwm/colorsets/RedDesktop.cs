# RedDesktop colorset
# Written by: QsoRiX (modyfied DarkDesktop by fRiQ)

SetEnv Colorset_Name "RedDesktop"
SetEnv Colorset_Author "QsoRiX"

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
fgTint, IconAlpha, IconTint "red" 10

Colorset $[cs_panel_active] \
Foreground "darkgray", Background "darkgray", \
Hilight "darkgray", Shadow "darkgray", \
Tint "#660000" 50, RootTransparent

Colorset $[cs_panel_wininactive] \
Foreground "gray", Background "#770000", \
Hilight "#440000", Shadow "#440000", \
fgTint "red" 10, Tint "black" 50, IconTint "red" 20, RootTransparent

Colorset $[cs_panel_winactive] \
Foreground "white", Background "#770000", \
Hilight "#660000", Shadow "#660000", \
fgTint "red" 20, Tint "red" 50, IconTint "red" 20, RootTransparent

Colorset $[cs_panel_border] \
Background "#770000"

Colorset $[cs_menu_inactive] \
Foreground "white", Background "#777777", \
Hilight "#770000", Shadow "#770000", \
Tint "black" 50, RootTransparent, \
fgTint "red" 20, IconAlpha, IconTint "red" 10

Colorset $[cs_menu_active] \
Foreground "#ff8888", Background "#661111", \
Hilight "black", Shadow "gray", \
IconTint "red" 10


# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# Additional variables
SetEnv ThumbBorderColor "red"
SetEnv ExternalFontColor "gray40"

SetEnv TrayerTint "0x000000"
SetEnv TrayerAlpha "128"

