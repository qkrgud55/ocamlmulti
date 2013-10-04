BASEDIR=../..
#MODULES=
MAIN_MODULE=intext
C_FILES=intextaux

include $(BASEDIR)/makefiles/Makefile.one
include $(BASEDIR)/makefiles/Makefile.common

init:
	cp index ../../../stdlib_r
	cd ../../.. ; make cross-phc ; cd -

intext.cmx: intext.ml
	$(POCAMLOPT) -c -index_file index intext.ml
 
