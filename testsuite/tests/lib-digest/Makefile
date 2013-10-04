BASEDIR=../..
#MODULES=
MAIN_MODULE=md5
ADD_COMPFLAGS=-w a

POCAMLOPT=/home/phc/ocaml_internal/ocamlmulti/ocamlopt -cclib '-pthread' -phcr -nostdlib -I /home/phc/ocaml_internal/ocamlmulti/stdlib_r

md5: md5.ml
	$(POCAMLOPT) -index_file index md5.ml
	./a.out

init:
	cp index ../../../stdlib_r
	cd ../../.. ; make cross-phc ; cd -

