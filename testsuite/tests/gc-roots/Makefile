BASEDIR=../..
#MODULES=
MAIN_MODULE=globroots
C_FILES=globrootsprim
ADD_COMPFLAGS=-w a

POCAMLOPT=/home/phc/ocaml_internal/ocamlmulti/ocamlopt -cclib '-pthread' -phcr -nostdlib -I /home/phc/ocaml_internal/ocamlmulti/stdlib_r

globroots: globroots.ml globrootsprim.o
	$(POCAMLOPT) -c -index_file index globroots.ml
	$(POCAMLOPT) globrootsprim.o globroots.cmx
	./a.out

globrootsprim.o: globrootsprim.c
	gcc -I ../../../byterun -c globrootsprim.c

init: globrootsprim.o
	if test -f index; then rm index; else :; fi
	$(POCAMLOPT) -c globroots.ml
	$(POCAMLOPT) globrootsprim.o globroots.cmx
	cp index ../../../stdlib_r
	cd ../../.. ; make cross-phc ; cd -



