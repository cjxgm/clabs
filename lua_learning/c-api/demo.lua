
function __m(name)
	print("Hello, " .. name .. "!")
	print(Cjfc.getinfo(1))
	print(Cjfc.getinfo(2))

	a, b = Cjfc.getinfo(3)
	print(a)
	print(b)

	return 1
end

MAIN = __m
