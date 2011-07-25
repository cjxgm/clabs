
PLUGIN = myblur5

all: $(PLUGIN)
clean:
	rm -f $(PLUGIN)
rebuild: clean all
debug: all

$(PLUGIN): $(PLUGIN).c
	gimptool --install $<

