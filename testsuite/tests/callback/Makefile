BASEDIR=../..

C=$(NATIVECC) -I $(TOPDIR)/byterun

POCAMLOPT=/home/phc/ocaml_internal/ocamlmulti/ocamlopt -cclib '-pthread' -phcr -nostdlib -I /home/phc/ocaml_internal/ocamlmulti/stdlib_r

tcallback: tcallback.ml callbackprim.o
	$(POCAMLOPT) -c -index_file index testing.ml
	$(POCAMLOPT) -c -index_file index tcallback.ml
	$(POCAMLOPT) callbackprim.o testing.cmx tcallback.cmx
	./a.out

callbackprim.o: callbackprim.c
	gcc -I ../../../byterun -c callbackprim.c

init:
	if test -f index; then rm index; else :; fi
	$(POCAMLOPT) -c tcallback.ml
	$(POCAMLOPT) -c testing.mli
	$(POCAMLOPT) -c testing.ml
	$(POCAMLOPT) callbackprim.o testing.cmx tcallback.cmx
	cp index ../../../stdlib_r
	cd ../../.. ; make cross-phc ; cd -

default: run-byte run-opt

common:
	@$(CC) -c callbackprim.c

run-byte: common
	@printf " ... testing 'bytecode':"
	@$(OCAMLC) -c tcallback.ml
	@$(OCAMLC) -o ./program -custom unix.cma callbackprim.$(O) tcallback.cmo
	@./program > bytecode.result
	@$(DIFF) reference bytecode.result || (echo " => failed" && exit 1)
	@echo " => passed"

run-opt: common
	@if [ -z "$(BYTECODE_ONLY)" ]; then \
	  printf " ... testing 'native':"; \
	  $(OCAMLOPT) -c tcallback.ml; \
	  $(OCAMLOPT) -o ./program unix.cmxa callbackprim.$(O) tcallback.cmx; \
	  ./program > native.result; \
	  $(DIFF) reference native.result || (echo " => failed" && exit 1); \
	  echo " => passed"; \
	fi

promote: defaultpromote

clean: defaultclean
	@rm -f *.result ./program

include $(BASEDIR)/makefiles/Makefile.common
