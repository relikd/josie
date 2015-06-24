#################################################################################
##
## Makefile for LaTeX documents
##
## usage: 
##    all   - to make all images and the whole thesis
##    clean - to clean all directories
##
#################################################################################
.PHONY: all clean publish makepdf makehtml
.SUFFIXES: .pdf .ps .dvi .eps .tex .html

#################################################################################
## file names
#################################################################################
FILE        = thesis
BIBTEXDB    = literatur.bib
TEXSRC      = $(FILE).tex
FILEPDF     = $(FILE).pdf
FILEPS      = $(FILE).ps

#################################################################################
## programs etc.
#################################################################################
LATEX       = latex
PDFLATEX    = pdflatex
BIBTEX      = bibtex
BIBOPT      = -min-crossrefs=99
CD          = cd

#################################################################################
## rules 
#################################################################################
.tex.pdf:
	$(PDFLATEX) $< && $(BIBTEX) $(FILE) && $(PDFLATEX) $< && $(PDFLATEX) $<

.tex.dvi:
	$(LATEX) $< && $(BIBTEX) $(BIBOPT) $* && $(LATEX) $< $& $(LATEX) $<

.dvi.ps:
	$(DVIPS) -f $< -o $@

#################################################################################
## make the document
#################################################################################
all: makepdf 

makepdf: $(FILEPDF)

$(FILEPDF): $(TEXSRC) $(BIBTEXDB)

#################################################################################
## clean all directories
#################################################################################
clean:
	$(RM) *.aux *.dvi *.ps *.pdf *.bbl *.log *.toc *~ 
	$(RM) *.blg *.lot *.out *.brf *.lof *.tps 
	$(RM) *.run.xml *.lol *.bcf *-blx.bib
	$(RM) $(FILEPDF)
