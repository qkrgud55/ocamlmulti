BASEDIR=../..
BASEDIR1=../../..

default:
	@$(OCAMLC) -o program.byte signals.ml
	@./program.byte
	@$(OCAMLOPT) -o program.native signals.ml
	@./program.native

phc:
	$(BASEDIR1)/bin/ocamlopt -cclib '-pthread' -phcr -nostdlib -I $(BASEDIR1)/stdlib_r -o signals signals.ml

clean: defaultclean

include $(BASEDIR)/makefiles/Makefile.common
