
Window = {}

function Window.create(info)
	info.title = info.title or "GLua"
	info.size  = info.size  or {640, 480}
	info.fps   = info.fps   or 30
	spf = 1000 / info.fps;

	glua.window_create(info.title, info.size[1], info.size[2], spf)
end

