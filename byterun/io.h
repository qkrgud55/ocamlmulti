/***********************************************************************/
/*                                                                     */
/*                                OCaml                                */
/*                                                                     */
/*            Xavier Leroy, projet Cristal, INRIA Rocquencourt         */
/*                                                                     */
/*  Copyright 1996 Institut National de Recherche en Informatique et   */
/*  en Automatique.  All rights reserved.  This file is distributed    */
/*  under the terms of the GNU Library General Public License, with    */
/*  the special exception on linking described in file ../LICENSE.     */
/*                                                                     */
/***********************************************************************/

/* $Id: io.h 12331 2012-04-10 14:07:40Z doligez $ */

/* Buffered input/output */

#ifndef CAML_IO_H
#define CAML_IO_H

#include "misc.h"
#include "mlvalues.h"
#include "context.h"

/*
struct channel {
  int fd;                       
  file_offset offset;           
  char * end;                   
  char * curr;                  
  char * max;                   
  void * mutex;                 
  struct channel * next, * prev;
  int revealed;                 
  int old_revealed;             
  int refcount;                 
  int flags;                    
  char buff[IO_BUFFER_SIZE];    
};
*/

enum {
  CHANNEL_FLAG_FROM_SOCKET = 1  /* For Windows */
};

/* For an output channel:
     [offset] is the absolute position of the beginning of the buffer [buff].
   For an input channel:
     [offset] is the absolute position of the logical end of the buffer, [max].
*/

/* Functions and macros that can be called from C.  Take arguments of
   type struct channel *.  No locking is performed. */

#define putch(channel, ch) do{                                            \
  if ((channel)->curr >= (channel)->end) caml_flush_partial(channel);     \
  *((channel)->curr)++ = (ch);                                            \
}while(0)

#define putch_r(ctx, channel, ch) do{                                            \
  if ((channel)->curr >= (channel)->end) caml_flush_partial_r(ctx, channel);     \
  *((channel)->curr)++ = (ch);                                            \
}while(0)

#define getch(channel)                                                      \
  ((channel)->curr >= (channel)->max                                        \
   ? caml_refill(channel)                                                   \
   : (unsigned char) *((channel)->curr)++)

CAMLextern struct channel * caml_open_descriptor_in (int);
CAMLextern struct channel * caml_open_descriptor_out (int);
CAMLextern struct channel * caml_open_descriptor_in_r (pctxt, int);
CAMLextern struct channel * caml_open_descriptor_out_r (pctxt, int);
CAMLextern void caml_close_channel (struct channel *);
CAMLextern int caml_channel_binary_mode (struct channel *);
CAMLextern value caml_alloc_channel(struct channel *chan);
CAMLextern value caml_alloc_channel_r(pctxt, struct channel *chan);

CAMLextern int caml_flush_partial (struct channel *);
CAMLextern int caml_flush_partial_r (pctxt, struct channel *);
CAMLextern void caml_flush (struct channel *);
CAMLextern void caml_flush_r (pctxt, struct channel *);
CAMLextern void caml_putword (struct channel *, uint32);
CAMLextern int caml_putblock (struct channel *, char *, intnat);
CAMLextern int caml_putblock_r (pctxt, struct channel *, char *, intnat);
CAMLextern void caml_really_putblock (struct channel *, char *, intnat);

CAMLextern unsigned char caml_refill (struct channel *);
CAMLextern uint32 caml_getword (struct channel *);
CAMLextern int caml_getblock (struct channel *, char *, intnat);
CAMLextern int caml_really_getblock (struct channel *, char *, intnat);

/* Extract a struct channel * from the heap object representing it */

#define Channel(v) (*((struct channel **) (Data_custom_val(v))))

/* The locking machinery */

CAMLextern void (*caml_channel_mutex_free) (struct channel *);
CAMLextern void (*caml_channel_mutex_lock) (struct channel *);
CAMLextern void (*caml_channel_mutex_lock_r) (pctxt, struct channel *);
CAMLextern void (*caml_channel_mutex_unlock) (struct channel *);
CAMLextern void (*caml_channel_mutex_unlock_r) (pctxt, struct channel *);
CAMLextern void (*caml_channel_mutex_unlock_exn) (void);

CAMLextern struct channel * caml_all_opened_channels;

#define Lock(channel) \
  if (caml_channel_mutex_lock != NULL) (*caml_channel_mutex_lock)(channel)
#define Lock_r(ctx, channel) \
  if (caml_channel_mutex_lock_r != NULL) (*caml_channel_mutex_lock_r)(ctx, channel)
#define Unlock(channel) \
  if (caml_channel_mutex_unlock != NULL) (*caml_channel_mutex_unlock)(channel)
#define Unlock_r(ctx, channel) \
  if (caml_channel_mutex_unlock_r != NULL) (*caml_channel_mutex_unlock_r)(ctx, channel)
#define Unlock_exn() \
  if (caml_channel_mutex_unlock_exn != NULL) (*caml_channel_mutex_unlock_exn)()

/* Conversion between file_offset and int64 */

#ifdef ARCH_INT64_TYPE
#define Val_file_offset(fofs) caml_copy_int64(fofs)
#define File_offset_val(v) ((file_offset) Int64_val(v))
#else
CAMLextern value caml_Val_file_offset(file_offset fofs);
CAMLextern file_offset caml_File_offset_val(value v);
#define Val_file_offset caml_Val_file_offset
#define File_offset_val caml_File_offset_val
#endif

#endif /* CAML_IO_H */
