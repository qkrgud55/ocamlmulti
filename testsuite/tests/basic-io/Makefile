BASEDIR=../..
#MODULES=
MAIN_MODULE=wc
EXEC_ARGS=wc.ml

include $(BASEDIR)/makefiles/Makefile.common
include $(BASEDIR)/makefiles/Makefile.one

test: wc.ml
	@echo -n " ... testing 'wc.ml': "
	@$(POCAMLOPT) -o program.native wc.ml
	@printf " ocamlopt"; \
	./program.native $(EXEC_ARGS) > $(MAIN_MODULE).result || (echo " => failed" && exit 1); \
	$(DIFF) $(MAIN_MODULE).reference $(MAIN_MODULE).result > /dev/null || (echo " => failed" && exit 1)
	@echo " => passed"
