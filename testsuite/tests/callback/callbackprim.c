#include "mlvalues.h"
#include "memory.h"
#include "callback.h"
#include "context.h"

value mycallback1(pctxt ctx, value fun, value arg)
{
  value res;
  res = caml_callback_r(ctx, fun, arg);
  return res;
}

value mycallback2(pctxt ctx, value fun, value arg1, value arg2)
{
  value res;
  res = caml_callback2_r(ctx, fun, arg1, arg2);
  return res;
}

value mycallback3(pctxt ctx, value fun, value arg1, value arg2, value arg3)
{
  value res;
  res = caml_callback3_r(ctx, fun, arg1, arg2, arg3);
  return res;
}

value mycallback4(pctxt ctx, value fun, value arg1, value arg2, value arg3, value arg4)
{
  value args[4];
  value res;
  args[0] = arg1;
  args[1] = arg2;
  args[2] = arg3;
  args[3] = arg4;
  res = caml_callbackN_r(ctx, fun, 4, args);
  return res;
}

value mypushroot(pctxt ctx, value v, value fun, value arg)
{
  Begin_root(v)
    caml_callback_r(ctx, fun, arg);
  End_roots();
  return v;
}

value mycamlparam (pctxt ctx, value v, value fun, value arg)
{
  CAMLparam3_r (ctx, v, fun, arg);
  CAMLlocal2_r (ctx, x, y);
  x = v;
  y = caml_callback_r (ctx, fun, arg);
  v = x;
  CAMLreturn_r (ctx, v);
}
