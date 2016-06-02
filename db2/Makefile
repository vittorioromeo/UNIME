PY=python
PANDOC=pandoc

BASEDIR=$(CURDIR)
TEMPDIR=$(BASEDIR)/temp
INPUTDIR=$(BASEDIR)/source
OUTPUTDIR=$(BASEDIR)/output

pdf:
	pp -en "$(INPUTDIR)"/**.md > "$(TEMPDIR)"/merged.md
	pandoc "$(TEMPDIR)"/merged.md \
	-S \
	-o "$(OUTPUTDIR)/out.pdf" \
	--highlight-style pygments \
	-V fontsize=12pt \
	-V papersize=a4paper \
	-V documentclass:report \
	-V geometry:margin=2.5cm \
	-N \
	--latex-engine=xelatex \
	--verbose

.PHONY: pdf
