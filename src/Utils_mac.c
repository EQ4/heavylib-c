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

#include "Utils_mac.h"

#if HV_APPLE

hv_size_t hv_max_ui(hv_size_t x, hv_size_t y) {
  return (x >= y) ? x : y;
}

hv_size_t hv_min_ui(hv_size_t x, hv_size_t y) {
  return (x <= y) ? x : y;
}

int hv_max_i(int x, int y) {
  return (x >= y) ? x : y;
}

int hv_min_i(int x, int y) {
  return (x <= y) ? x : y;
}

hv_uint32_t hv_min_max_log2(hv_uint32_t x) {
  // finds ceil(log2(x))
  // http://stackoverflow.com/questions/2589096/find-most-significant-bit-left-most-that-is-set-in-a-bit-array
  return (hv_uint32_t) ((8 * sizeof(unsigned int)) - __builtin_clz(x - 1));
}

#endif // HV_APPLE
