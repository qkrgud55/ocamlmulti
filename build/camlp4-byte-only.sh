#!/bin/sh

#########################################################################
#                                                                       #
#                                 OCaml                                 #
#                                                                       #
#         Nicolas Pouillard, projet Gallium, INRIA Rocquencourt         #
#                                                                       #
#   Copyright 2008 Institut National de Recherche en Informatique et    #
#   en Automatique.  All rights reserved.  This file is distributed     #
#   under the terms of the Q Public License version 1.0.                #
#                                                                       #
#########################################################################

# $Id: camlp4-byte-only.sh 11156 2011-07-27 14:17:02Z doligez $

set -e
cd `dirname $0`/..
. build/targets.sh
set -x
$OCAMLBUILD $@ byte_stdlib_mixed_mode $OCAMLC_BYTE $OCAMLLEX_BYTE $CAMLP4_BYTE
