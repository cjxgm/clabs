
Window.create{
	title = "Hello, world!",
	fps   = 60,
	size  = { 320, 240 }
}

renders = {
	Cube:new{}, Cube:new{ x = 20, y = 20 }
}

t = 0
Window.render = function()
	for _, o in pairs(renders) do
		o:render()
	end

	renders[1].x = math.sin(t)*20 - 10
	renders[2].y = math.sin(t*2)*20
	t = t + 0.05
end

