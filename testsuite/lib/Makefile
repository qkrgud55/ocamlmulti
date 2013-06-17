# $Id: Makefile 12239 2012-03-14 10:22:02Z xclerc $

compile: compile-targets

promote: defaultpromote

clean: defaultclean

include ../makefiles/Makefile.common

compile-targets: testing.cmi testing.cmo
	@if [ -z "$(BYTECODE_ONLY)" ]; then \
	  $(MAKE) testing.cmx; \
	fi
