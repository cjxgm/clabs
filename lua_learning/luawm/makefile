
all:
	@lua lake
	@rm -f *.d
	@rm -f *.spec
clean:
	@lua lake clean
	@rm -f *.d
	@rm -f *.spec

debug: all
	lua test.lb
