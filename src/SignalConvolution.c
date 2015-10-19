/**
 * Copyright (c) 2014, 2015, Enzien Audio Ltd.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#include "SignalConvolution.h"

hv_size_t sConv_init(SignalConvolution *o, struct HvTable *table, const int size) {
  o->table = table;
  hv_size_t numBytes = hTable_init(&o->inputs, size);
  return numBytes;
}

void sConv_free(SignalConvolution *o) {
  o->table = NULL;
  hv_free(&o->inputs);
}

void sConv_onMessage(HvBase *_c, SignalConvolution *o, int letIndex,
    const HvMessage *const m, void *sendMessage) {
  switch (letIndex) {
    case 1: {
      if (msg_isHashLike(m,0)) {
        HvTable *table = ctx_getTableForHash(_c, msg_getHash(m,0));
        if (table != NULL) {
          o->table = table;
          if (hTable_getSize(&o->inputs) != hTable_getSize(table)) {
            hTable_resize(&o->inputs,
                (hv_uint32_t) hv_min_ui(hTable_getSize(&o->inputs), hTable_getSize(table)));
          }
        }
      }
      break;
    }
    case 2: {
      if (msg_isFloat(m,0)) {
        // convolution size should never exceed the coefficient table size
        hTable_resize(&o->inputs, (hv_uint32_t) msg_getFloat(m,0));
      }
      break;
    }
    default: return;
  }
}
