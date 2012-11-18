
all: about_zju
about_zju: clean about_zju.latex
	@echo -e "\e[1;32m[xelatex] compiling...\e[0m"
	xelatex about_zju.latex
	xelatex about_zju.latex
	@echo -e "\e[1;32m[rm] cleaning up...\e[0m"
	rm -f *.aux
	rm -f *.bbl
	rm -f *.blg
	rm -f *.log
	rm -f *.toc
	rm -f *.out
	rm -f *.snm
	rm -f *.nav
preview:
	evince about_zju.pdf &
clean:
	@echo -e "\e[1;32m[rm] cleaning up...\e[0m"
	rm -f *.aux
	rm -f *.bbl
	rm -f *.blg
	rm -f *.log
	rm -f *.toc
	rm -f *.out
	rm -f *.snm
	rm -f *.nav
cleanall: clean
	rm -f about_zju.pdf

