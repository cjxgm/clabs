# IrishDesktop colorset
# Written by: fromooze <fromooze@yahoo.es>

SetEnv Colorset_Name "IrishDesktop"
SetENv Colorset_Author "fromooze <fromooze@yahoo.es>"

# What colorsets do we use?
Read $./colorset-definition

# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# QuakeConsole, thumbnailed windows
Colorset $[cs_quakeconsole] \
Foreground "black", Background "black", \
Hilight "black", Shadow "black", \
IconAlpha 70


Colorset $[cs_panel_inactive] \
Foreground "white", Background "white", \
Tint "white" 0, RootTransparent, \
fgTint, IconAlpha, IconTint

Colorset $[cs_panel_active] \
Foreground "white", Background "darkgray", \
Hilight "darkgray", Shadow "darkgray", \
Tint "white" 30, RootTransparent

Colorset $[cs_panel_wininactive] \
Foreground "white", Background "white", \
Hilight "#cccccc", Shadow "#cccccc", \
Tint "white" 20, RootTransparent

Colorset $[cs_panel_winactive] \
Foreground "orange", Background "orange", \
Hilight "orange", Shadow "orange", \
Tint "darkgreen" 50, RootTransparent

Colorset $[cs_panel_border] \
Background "white"

Colorset $[cs_menu_inactive] \
Foreground "white", Background "white", \
Hilight "white", Shadow "white", \
Tint "white" 10, RootTransparent

Colorset $[cs_menu_active] \
Foreground "darkgreen", Background "orange", \
Hilight "darkgreen", Shadow "orange"


# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# Additional variables
SetEnv ThumbBorderColor "gray"
SetEnv ExternalFontColor "gray40"

SetEnv TrayerTint "0xFFFFFF"
SetEnv TrayerAlpha "256"

