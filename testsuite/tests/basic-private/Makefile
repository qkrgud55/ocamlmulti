BASEDIR=../..
MODULES=length
MAIN_MODULE=tlength

include $(BASEDIR)/makefiles/Makefile.one
include $(BASEDIR)/makefiles/Makefile.common

POCAMLOPT=/home/phc/ocaml_internal/ocamlmulti/ocamlopt -cclib '-pthread' -phcr -nostdlib -I /home/phc/ocaml_internal/ocamlmulti/stdlib_r

tlength: length.ml tlength.ml length.mli
	if test -f index; then rm index; else :; fi
	$(POCAMLOPT) -c length.mli
	$(POCAMLOPT) -c length.ml
	$(POCAMLOPT) -c tlength.ml
	$(POCAMLOPT) length.cmx tlength.cmx
	cp index ../../../stdlib_r
	cd ../../.. ; make cross-phc ; cd -
	$(POCAMLOPT) -c -index_file index length.ml
	$(POCAMLOPT) -c -index_file index tlength.ml
	$(POCAMLOPT) length.cmx tlength.cmx
	./a.out
