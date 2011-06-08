# Red - Cyan colorset
# Written by: Maciej Delmanowski <harnir@linux.net.pl>

SetEnv Colorset_Name "Red - Cyan"
SetEnv Colorset_Author "Maciej Delmanowski <harnir@linux.net.pl>"

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
Tint "#930000" 50, RootTransparent, \
fgTint, IconAlpha, IconTint

Colorset $[cs_panel_active] \
Foreground "darkgray", Background "darkgray", \
Hilight "darkgray", Shadow "darkgray", \
Tint "#ff4949" 50, RootTransparent

Colorset $[cs_panel_wininactive] \
Foreground "gray", Background "#777777", \
Hilight "#444444", Shadow "#444444", \
Tint "#44514f" 50, RootTransparent

Colorset $[cs_panel_winactive] \
Foreground "white", Background "#777777", \
Hilight "#666666", Shadow "#666666", \
Tint "#00b294" 50, RootTransparent

Colorset $[cs_panel_border] \
Background "#777777"

Colorset $[cs_menu_inactive] \
Foreground "white", Background "#777777", \
Hilight "#777777", Shadow "#777777", \
Tint "#930000" 50, RootTransparent

Colorset $[cs_menu_active] \
Foreground "black", Background "#00b294", \
Hilight "black", Shadow "gray"


# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# Additional variables
SetEnv ThumbBorderColor "gray"
SetEnv ExternalFontColor "gray40"

SetEnv TrayerTint "0x930000"
SetEnv TrayerAlpha "128"

