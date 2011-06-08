# CIOS colorset
# Written by: fromooze <fromooze@yahoo.es>

SetEnv Colorset_Name "CIOS"
SetEnv Colorset_Author "fromooze <fromooze@yahoo.es>"

# What colorsets do we use?
Read $./colorset-definition

# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# QuakeConsole, thumbnailed windows
Colorset $[cs_quakeconsole] \
Foreground "black", Background "black", \
Hilight "black", Shadow "black", \
IconAlpha 50


Colorset $[cs_panel_inactive] \
Foreground "#00ffff", Background "#444444", \
Tint "#00008b" 0, RootTransparent, \
fgTint, IconAlpha, IconTint

Colorset $[cs_panel_active] \
Foreground "#00ffff", Background "darkgray", \
Hilight "darkgray", Shadow "darkgray", \
Tint "#00ffff" 30, RootTransparent

Colorset $[cs_panel_wininactive] \
Foreground "#00ffff", Background "#00ffff", \
Hilight "#ee92ee", Shadow "#ee82ee", \
Tint "#00008b" 30, RootTransparent

Colorset $[cs_panel_winactive] \
Foreground "#00008b", Background "#00008b", \
Hilight "#00008b", Shadow "#00008b", \
Tint "#00ffff" 50, RootTransparent

Colorset $[cs_panel_border] \
Background "#00ffff"

Colorset $[cs_menu_inactive] \
Foreground "#00ffff", Background "#00ffff", \
Hilight "#00ffff", Shadow "#00ffff", \
Tint "#00008b" 30, RootTransparent

Colorset $[cs_menu_active] \
Foreground "#ee82ee", Background "#00008b", \
Hilight "#00ffff", Shadow "#00ffff"


# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# Additional variables
SetEnv ThumbBorderColor "blue"
SetEnv ExternalFontColor "gray40"

SetEnv TrayerTint "0x00008b"
SetEnv TrayerAlpha "256"

