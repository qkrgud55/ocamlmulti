BASEDIR=../..
include $(BASEDIR)/makefiles/Makefile.several
include $(BASEDIR)/makefiles/Makefile.common


bdd: bdd.ml
	$(POCAMLOPT) -index_file index -o ./program bdd.ml
	./program > native.result; \
	$(DIFF) reference native.result || (echo " => failed" && exit 1); \
	
init:
	cp index ../../../stdlib_r
	cd ../../.. ; make cross-phc ; cd -


