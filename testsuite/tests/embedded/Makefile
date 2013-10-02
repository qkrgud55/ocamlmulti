BASEDIR=../..

POCAMLOPT=/home/phc/ocaml_internal/ocamlmulti/ocamlopt -cclib '-pthread' -phcr -nostdlib -I /home/phc/ocaml_internal/ocamlmulti/stdlib_r

default: compilep run

compilep: 
	$(POCAMLOPT) -ccopt -g -ccopt -I -ccopt $(TOPDIR)/byterun cmstub.c
	$(POCAMLOPT) -ccopt -g cmmain.c
	$(POCAMLOPT) -c -g -index_file index cmcaml.ml
	$(POCAMLOPT) -g -o program cmstub.o cmcaml.cmx cmmain.o

compile:
	@$(OCAMLC) -ccopt -I -ccopt $(TOPDIR)/byterun cmstub.c
	@$(OCAMLC) -ccopt -I -ccopt $(TOPDIR)/byterun cmmain.c
	@$(OCAMLC) -c cmcaml.ml
	@$(OCAMLC) -custom -o program cmstub.o cmcaml.cmo cmmain.o

run:
	@printf " ... testing 'cmmain':"
	@./program > program.result
	@$(DIFF) program.reference program.result > /dev/null || (echo " => failed" && exit 1)
	@echo " => passed"

promote: defaultpromote

clean: defaultclean
	@rm -f *.result ./program

include $(BASEDIR)/makefiles/Makefile.common
