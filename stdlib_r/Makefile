#########################################################################
#                                                                       #
#                                 OCaml                                 #
#                                                                       #
#            Xavier Leroy, projet Cristal, INRIA Rocquencourt           #
#                                                                       #
#   Copyright 1999 Institut National de Recherche en Informatique et    #
#   en Automatique.  All rights reserved.  This file is distributed     #
#   under the terms of the GNU Library General Public License, with     #
#   the special exception on linking described in file ../LICENSE.      #
#                                                                       #
#########################################################################

# $Id: Makefile 11156 2011-07-27 14:17:02Z doligez $

include Makefile.shared

#allopt-$(PROFILING)
allopt: stdlib.cmxa std_exit.cmx 
	cp ../asmrun/libasmrun.a ./libasmrun.a
#	cd ../asmrun; make all

allopt-noprof:

allopt-prof: stdlib.p.cmxa std_exit.p.cmx
	rm -f std_exit.p.cmi

#installopt-$(PROFILING)
installopt: installopt-default 

installopt-default:
	cp stdlib.cmxa stdlib.a std_exit.o *.cmx $(LIBDIR)
	cd $(LIBDIR); $(RANLIB) stdlib.a

installopt-noprof:
	rm -f $(LIBDIR)/stdlib.p.cmxa; ln -s stdlib.cmxa $(LIBDIR)/stdlib.p.cmxa
	rm -f $(LIBDIR)/stdlib.p.a; ln -s stdlib.a $(LIBDIR)/stdlib.p.a
	rm -f $(LIBDIR)/std_exit.p.cmx; \
	  ln -s std_exit.cmx $(LIBDIR)/std_exit.p.cmx
	rm -f $(LIBDIR)/std_exit.p.o; ln -s std_exit.o $(LIBDIR)/std_exit.p.o

installopt-prof:
	cp stdlib.p.cmxa stdlib.p.a std_exit.p.cmx std_exit.p.o $(LIBDIR)
	cd $(LIBDIR); $(RANLIB) stdlib.p.a

stdlib.p.cmxa: $(OBJS:.cmo=.p.cmx)
	$(CAMLOPT) -a -o stdlib.p.cmxa $(OBJS:.cmo=.p.cmx)

camlheader camlheaderd camlheader_ur: header.c ../config/Makefile
	if $(SHARPBANGSCRIPTS); then \
	  echo '#!$(BINDIR)/ocamlrun' > camlheader && \
	  echo '#!$(BINDIR)/ocamlrund' > camlheaderd && \
	  echo '#!' | tr -d '\012' > camlheader_ur; \
	else \
	  $(BYTECC) $(BYTECCCOMPOPTS) $(BYTECCLINKOPTS) \
	            -DRUNTIME_NAME='"$(BINDIR)/ocamlrun"' \
	            header.c -o tmpheader$(EXE) && \
	  strip tmpheader$(EXE) && \
	  mv tmpheader$(EXE) camlheader && \
	  cp camlheader camlheader_ur && \
	  $(BYTECC) $(BYTECCCOMPOPTS) $(BYTECCLINKOPTS) \
	            -DRUNTIME_NAME='"$(BINDIR)/ocamlrund"' \
	            header.c -o tmpheader$(EXE) && \
	  strip tmpheader$(EXE) && \
	  mv tmpheader$(EXE) camlheaderd; \
	fi

.PHONY: all allopt allopt-noprof allopt-prof install installopt
.PHONY: installopt-default installopt-noprof installopt-prof clean depend
