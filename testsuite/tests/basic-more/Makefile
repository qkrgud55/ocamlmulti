BASEDIR=../..
MODULES=testing

include $(BASEDIR)/makefiles/Makefile.several
include $(BASEDIR)/makefiles/Makefile.common

POCAMLOPT=/home/phc/ocaml_internal/ocamlmulti/ocamlopt -cclib '-pthread' -phcr -nostdlib -I /home/phc/ocaml_internal/ocamlmulti/stdlib_r

tbuffer: tbuffer.ml
	if test -f index; then rm index; else :; fi
	$(POCAMLOPT) -c testing.mli
	$(POCAMLOPT) -c testing.ml
	$(POCAMLOPT) -c tbuffer.ml
	$(POCAMLOPT) testing.cmx tbuffer.cmx
	cp index ../../../stdlib_r
	cd ../../.. ; make cross-phc ; cd -
	$(POCAMLOPT) -c -index_file index testing.ml
	$(POCAMLOPT) -c -index_file index tbuffer.ml
	$(POCAMLOPT) testing.cmx tbuffer.cmx
	./a.out

tformat: tformat.ml
	if test -f index; then rm index; else :; fi
	$(POCAMLOPT) -c tformat.ml
	$(POCAMLOPT) -c testing.ml
	$(POCAMLOPT) testing.cmx tformat.cmx
	cp index ../../../stdlib_r
	cd ../../.. ; make cross-phc ; cd -
	$(POCAMLOPT) -c -index_file index testing.ml
	$(POCAMLOPT) -c -index_file index tformat.ml
	$(POCAMLOPT) testing.cmx tformat.cmx
	./a.out

tprintf: tprintf.ml
	if test -f index; then rm index; else :; fi
	$(POCAMLOPT) -c tprintf.ml
	$(POCAMLOPT) -c testing.ml
	$(POCAMLOPT) testing.cmx tprintf.cmx
	cp index ../../../stdlib_r
	cd ../../.. ; make cross-phc ; cd -
	$(POCAMLOPT) -c -index_file index testing.ml
	$(POCAMLOPT) -c -index_file index tprintf.ml
	$(POCAMLOPT) testing.cmx tprintf.cmx
	./a.out
