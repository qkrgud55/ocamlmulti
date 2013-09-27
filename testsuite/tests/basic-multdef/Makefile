BASEDIR=../..
MODULES=multdef
MAIN_MODULE=usemultdef

include $(BASEDIR)/makefiles/Makefile.one
include $(BASEDIR)/makefiles/Makefile.common

POCAMLOPT=/home/phc/ocaml_internal/ocamlmulti/ocamlopt -cclib '-pthread' -phcr -nostdlib -I /home/phc/ocaml_internal/ocamlmulti/stdlib_r

usemultdef: usemultdef.ml multdef.ml multdef.mli
	if test -f index; then rm index; else :; fi
	$(POCAMLOPT) -c multdef.mli
	$(POCAMLOPT) -c multdef.ml
	$(POCAMLOPT) -c usemultdef.ml
	$(POCAMLOPT) multdef.cmx usemultdef.cmx
	cp index ../../../stdlib_r
	cd ../../.. ; make cross-phc ; cd -
	$(POCAMLOPT) -c -index_file index multdef.ml
	$(POCAMLOPT) -c -index_file index usemultdef.ml
	$(POCAMLOPT) multdef.cmx usemultdef.cmx
	./a.out
