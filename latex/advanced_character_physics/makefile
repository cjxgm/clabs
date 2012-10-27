
all: acp
acp: clean acp.latex references.bib
	@echo -e "\e[1;32m[pdflatex] first time compiling...\e[0m"
	pdflatex acp.latex
	@echo -e "\e[1;32m[bibtex] creating bib...\e[0m"
	bibtex acp.aux
	@echo -e "\e[1;32m[pdflatex] two more times compiling...\e[0m"
	pdflatex acp.latex
	pdflatex acp.latex
preview:
	evince acp.pdf &
clean:
	rm -f *.aux
	rm -f *.bbl
	rm -f *.blg
	rm -f *.log
cleanall: clean
	rm -f *.pdf

