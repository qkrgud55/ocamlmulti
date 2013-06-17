/***********************************************************************/
/*                                                                     */
/*                                OCaml                                */
/*                                                                     */
/*            Xavier Leroy, projet Cristal, INRIA Rocquencourt         */
/*                                                                     */
/*  Copyright 1996 Institut National de Recherche en Informatique et   */
/*  en Automatique.  All rights reserved.  This file is distributed    */
/*  under the terms of the GNU Library General Public License, with    */
/*  the special exception on linking described in file ../../LICENSE.  */
/*                                                                     */
/***********************************************************************/

/* $Id: geteuid.c 11156 2011-07-27 14:17:02Z doligez $ */

#include <mlvalues.h>
#include "unixsupport.h"

CAMLprim value unix_geteuid(value unit)
{
  return Val_int(geteuid());
}
