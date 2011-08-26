require 'cairo'

function conky_draw()
	-- Can only be ran in conky
	if conky_window == nil then
		return
	end

	-- initialize
	local cs = cairo_xlib_surface_create(
			conky_window.display,
			conky_window.drawable,
			conky_window.visual,
			conky_window.width,
			conky_window.height)
	local cr = cairo_create(cs)

	draw_clock(cr)
	draw_mem(cr)
	draw_net(cr)
end

function draw_clock(cr)
	-- get time
	local sec  = os.date("%S")    
	local min  = os.date("%M") + sec / 60
	local hour = os.date("%H") + min / 60
	-- calculate something
	local angle_sec  = sec  / 60 * math.pi * 2 - math.pi / 2
	local angle_min  = min  / 60 * math.pi * 2 - math.pi / 2
	local angle_hour = hour / 24 * math.pi * 2 - math.pi / 2
	local dsec  = math.pi / 18
	local dmin  = math.pi / 14
	local dhour = math.pi / 10
	-- some settings
	local r  = 50
	local cx = conky_window.width  - r * 2 - 10
	local cy = conky_window.height - r * 2 - 10

	-- text time
	cairo_move_to(cr, cx - r - 150, cy + 50)
	cairo_set_source_rgba(cr, 1, 1, 1, 0.8)
	cairo_set_font_size(cr, 32)
	cairo_show_text(cr, os.date("%H:%M:%S"))
	cairo_stroke(cr)

	-- clock body
	cairo_arc(cr, cx, cy, r, 0, math.pi * 2)
	cairo_set_source_rgba(cr, 0.4, 0.4, 0.8, 0.4)
	cairo_set_line_width(cr, 6)
	cairo_stroke(cr)

	-- sec
	cairo_set_source_rgba(cr, 0.3, 0.8, 0.2, 0.8)
	cairo_set_line_width(cr, 2)
	cairo_move_to(cr, cx, cy)
	cairo_line_to(cr,
		cx + math.cos(angle_sec) * (r - 10),
		cy + math.sin(angle_sec) * (r - 10))
	cairo_stroke(cr)
	cairo_arc(cr, cx, cy, r, angle_sec - dsec, angle_sec + dsec)
	cairo_set_line_width(cr, 10)
	cairo_stroke(cr)

	-- min
	cairo_set_source_rgba(cr, 1.0, 1.0, 0.8, 0.6)
	cairo_set_line_width(cr, 3)
	cairo_move_to(cr, cx, cy)
	cairo_line_to(cr,
		cx + math.cos(angle_min) * (r - 14),
		cy + math.sin(angle_min) * (r - 14))
	cairo_stroke(cr)
	cairo_arc(cr, cx, cy, r, angle_min - dmin, angle_min + dmin)
	cairo_set_line_width(cr, 12)
	cairo_stroke(cr)

	-- hour
	cairo_set_source_rgba(cr, 0.6, 0.8, 0.3, 0.4)
	cairo_set_line_width(cr, 4)
	cairo_move_to(cr, cx, cy)
	cairo_line_to(cr,
		cx + math.cos(angle_hour) * (r - 20),
		cy + math.sin(angle_hour) * (r - 20))
	cairo_stroke(cr)
	cairo_arc(cr, cx, cy, r, angle_hour - dhour, angle_hour + dhour)
	cairo_set_line_width(cr, 15)
	cairo_stroke(cr)
end

function draw_mem(cr)
	-- get usage
	local mempct  = tonumber(conky_parse("$memperc")) / 100
	local swappct = tonumber(conky_parse("$swapperc")) / 100
	local top_mem_name_1 = conky_parse("${top_mem name 1}")
	local top_mem_pct_1  = conky_parse("${top_mem mem  1}") / 100
	local top_mem_name_2 = conky_parse("${top_mem name 2}")
	local top_mem_pct_2  = conky_parse("${top_mem mem  2}") / 100
	local top_mem_name_3 = conky_parse("${top_mem name 3}")
	local top_mem_pct_3  = conky_parse("${top_mem mem  3}") / 100
	-- calculate something
	local angle_mem  = mempct  * math.pi * 2 - math.pi
	local angle_swap = swappct * math.pi * 2 - math.pi
	local angle_top_mem_1 = top_mem_pct_1 * math.pi * 2
	local angle_top_mem_2 = top_mem_pct_2 * math.pi * 2
	local angle_top_mem_3 = top_mem_pct_3 * math.pi * 2
	-- some settings
	local r  = 80
	local cx = 150
	local cy = 150

	-- text usage
	cairo_move_to(cr, cx - 60, cy + 20)
	cairo_set_source_rgba(cr, 1, 1, 1, 0.8)
	cairo_set_font_size(cr, 16)
	cairo_show_text(cr, conky_parse("$mem / $memmax"))
	cairo_stroke(cr)

	-- swap body
	cairo_arc(cr, cx, cy, r+10, 0, math.pi * 2)
	cairo_set_source_rgba(cr, 1, 1, 1, 0.4)
	cairo_set_line_width(cr, 1)
	cairo_stroke(cr)
	-- swap total usage
	cairo_arc(cr, cx, cy, r+10, math.pi, angle_swap)
	cairo_set_source_rgba(cr, 1, 1, 1, 0.4)
	cairo_set_line_width(cr, 4)
	cairo_stroke(cr)

	-- mem body
	cairo_arc(cr, cx, cy, r, 0, math.pi * 2)
	cairo_set_source_rgba(cr, 1, 1, 1, 0.4)
	cairo_set_line_width(cr, 1)
	cairo_stroke(cr)
	-- mem total usage
	cairo_arc(cr, cx, cy, r, math.pi, angle_mem)
	cairo_set_source_rgba(cr, 1, 1, 1, 0.4)
	cairo_set_line_width(cr, 4)
	cairo_stroke(cr)

	cairo_set_font_size(cr, 12)
	cairo_set_line_width(cr, 1)
	-- top mem 1
	cairo_move_to(cr, cx, cy)
	cairo_arc(cr, cx, cy, r, math.pi, angle_top_mem_1 - math.pi)
	cairo_set_source_rgba(cr, 1, 1, 1, top_mem_pct_1)
	cairo_fill(cr)

	cairo_save(cr)
	cairo_move_to(cr, cx, cy)
	cairo_rotate(cr, -math.pi)
	cairo_rotate(cr, angle_top_mem_1 / 2)
	cairo_set_source_rgba(cr, 0, 0, 0, 0.8)
	cairo_show_text(cr, "  " .. top_mem_name_1)
	cairo_stroke(cr)
	cairo_restore(cr)

	-- top mem 2
	cairo_move_to(cr, cx, cy)
	cairo_arc(cr, cx, cy, r,
		math.pi + angle_top_mem_1,
		angle_top_mem_2 - math.pi + angle_top_mem_1)
	cairo_set_source_rgba(cr, 1, 1, 1, top_mem_pct_2)
	cairo_fill(cr)

	cairo_save(cr)
	cairo_move_to(cr, cx, cy)
	cairo_rotate(cr, -math.pi)
	cairo_rotate(cr, angle_top_mem_1)
	cairo_rotate(cr, angle_top_mem_2 / 2)
	cairo_set_source_rgba(cr, 0, 0, 0, 0.8)
	cairo_show_text(cr, "  " .. top_mem_name_2)
	cairo_stroke(cr)
	cairo_restore(cr)

	-- top mem 3
	cairo_move_to(cr, cx, cy)
	cairo_arc(cr, cx, cy, r,
		math.pi + angle_top_mem_1 + angle_top_mem_2,
		angle_top_mem_3 - math.pi + angle_top_mem_1 + angle_top_mem_2)
	cairo_set_source_rgba(cr, 1, 1, 1, top_mem_pct_3)
	cairo_fill(cr)

	cairo_save(cr)
	cairo_move_to(cr, cx, cy)
	cairo_rotate(cr, -math.pi)
	cairo_rotate(cr, angle_top_mem_1)
	cairo_rotate(cr, angle_top_mem_2)
	cairo_rotate(cr, angle_top_mem_3 / 2)
	cairo_set_source_rgba(cr, 0, 0, 0, 0.8)
	cairo_show_text(cr, "  " .. top_mem_name_3)
	cairo_stroke(cr)
	cairo_restore(cr)
end

function draw_net(cr)
	-- get speed
	local sdown = conky_parse("${downspeed eth0}")    
	local sup   = conky_parse("${upspeed   eth0}")    
	-- calculate something
	-- some settings
	local cx = 20
	local cy = 30

	cairo_set_source_rgba(cr, 1, 1, 1, 0.3)
	cairo_rectangle(cr, 10, 13, 240, 25)
	cairo_fill(cr)

	-- text speed
	cairo_move_to(cr, cx, cy)
	cairo_set_source_rgba(cr, 0, 0, 0, 0.8)
	cairo_set_font_size(cr, 16)
	cairo_show_text(cr, "down: " .. sdown .. "/s"
					.. "     up: " .. sup .. "/s")
	cairo_stroke(cr)
end

