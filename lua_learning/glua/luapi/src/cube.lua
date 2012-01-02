
Cube = {}

function Cube:new(o)
	o = o or {}
	o.x = o.x or 0
	o.y = o.y or 0
	o.z = o.z or -100
	o.w = o.w or 10
	o.h = o.h or 10
	o.d = o.d or 10

	setmetatable(o, self)
	self.__index = self
	return o;
end

function Cube:render()
	glua.render_cube(self.x, self.y, self.z, self.w, self.h, self.d)
end
