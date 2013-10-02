#include <string.h>
#include "mlvalues.h"
#include "callback.h"
#include "context.h"

/* Functions callable directly from C */

int fib(int n)
{
  value * fib_closure = caml_named_value_r(get_ctx(), "fib");
  return Int_val(caml_callback_r(get_ctx(), 
            *fib_closure, Val_int(n)));
}

char * format_result(int n)
{
  value * format_result_closure = caml_named_value_r(get_ctx(), "format_result");
  return strdup(String_val(caml_callback_r(get_ctx(),
            *format_result_closure, Val_int(n))));
}
