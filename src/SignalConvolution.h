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

#ifndef _SIGNAL_CONVOLUTION_H_
#define _SIGNAL_CONVOLUTION_H_

#include "HvBase.h"
#include "HvTable.h"
#include "HeavyMath.h"

typedef struct SignalConvolution {
  struct HvTable *table;
  struct HvTable inputs;
} SignalConvolution;

hv_size_t sConv_init(SignalConvolution *o, struct HvTable *coeffs, const int size);

void sConv_free(SignalConvolution *o);

void sConv_onMessage(HvBase *_c, SignalConvolution *o, int letIndex,
    const HvMessage *const m, void *sendMessage);

static inline void __hv_conv_f(SignalConvolution *o, hv_bInf_t bIn, hv_bOutf_t bOut) {
  hv_assert(o->table != NULL);
  hv_assert(hTable_getSize(&o->inputs) <= hTable_getSize(o->table));

#if HV_SIMD_AVX
#warning SignalConvolution not implemented
  *bOut = bIn; // TODO(mhroth): for now pass through the input
#elif HV_SIMD_SSE
  const float *const coeffs = hTable_getBuffer(o->table);
  const hv_uint32_t s = o->size - HV_N_SIMD;
  float *inputs = hTable_getBuffer(&o->inputs);
  hv_memcpy(o->inputs, o->inputs+HV_N_SIMD, s*sizeof(float));
  __hv_store_f(o->inputs+s, bIn);
  __m128 f = _mm_setzero_ps();
  for (int i = 0; i < o->size; ++i) {
    __hv_fma_f(_mm_loadu_ps(o->inputs+i), _mm_load1_ps(coeffs+s-i), f, &f);
  }
  *bOut = f;
//
//  __hv_store_f(o->inputs+4*o->size, bIn);
//  for (int i = 0; i < HV_N_SIMD; i++) {
//    hv_memcpy(o->inputs+i*o->size, o->inputs+(HV_N_SIMD-1)*o->size-i-1, (o->size-HV_N_SIMD)*sizeof(float));
//  }
//
//  __m128 f = _mm_setzero_ps();
//  for (int i = 0; i < o->size; i+=HV_N_SIMD) {
//    for (int j = 0; j < HV_N_SIMD; ++j) {
//      __hv_fma_f(_mm_load_ps(o->inputs+j*o->size+i), _mm_load_ps(coeffs+i+j), f);
//    }
//  }
#elif HV_SIMD_NEON
#warning SignalConvolution not implemented
  *bOut = bIn; // TODO(mhroth): for now pass through the input
#else // HV_SIMD_NONE
  const hv_uint32_t s = hTable_getSize(&o->inputs) - 1;
  float *const inputs = hTable_getBuffer(&o->inputs);
  hv_memcpy(inputs, inputs+1, s*sizeof(float));
  inputs[s] = bIn;
  const float *const coeffs = hTable_getBuffer(o->table);
  float f = 0.0f;
  for (int i = 0; i <= s; ++i) {
    f = hv_fma_f(inputs[s-i], coeffs[i], f);
  }
  *bOut = f;
#endif
}

#endif // _SIGNAL_CONVOLUTION_H_
