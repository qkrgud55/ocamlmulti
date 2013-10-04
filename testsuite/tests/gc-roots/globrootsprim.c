/* For testing global root registration */

#include "mlvalues.h"
#include "memory.h"
#include "alloc.h"

struct block { value v; };

#define Block_val(v) ((struct block *) (v))

value gb_get(pctxt ctx, value vblock)
{
  return Block_val(vblock)->v;
}

value gb_classic_register(pctxt ctx, value v)
{
  struct block * b = stat_alloc(sizeof(struct block));
  b->v = v;
  caml_register_global_root_r(ctx, &(b->v));
  return (value) b;
}

value gb_classic_set(pctxt ctx, value vblock, value newval)
{
  Block_val(vblock)->v = newval;
  return Val_unit;
}

value gb_classic_remove(pctxt ctx, value vblock)
{
  caml_remove_global_root_r(ctx, &(Block_val(vblock)->v));
  return Val_unit;
}

value gb_generational_register(pctxt ctx, value v)
{
  struct block * b = stat_alloc(sizeof(struct block));
  b->v = v;
  caml_register_generational_global_root_r(ctx, &(b->v));
  return (value) b;
}

value gb_generational_set(pctxt ctx, value vblock, value newval)
{
  caml_modify_generational_global_root_r(ctx, &(Block_val(vblock)->v), newval);
  return Val_unit;
}

value gb_generational_remove(pctxt ctx, value vblock)
{
  caml_remove_generational_global_root_r(ctx, &(Block_val(vblock)->v));
  return Val_unit;
}
