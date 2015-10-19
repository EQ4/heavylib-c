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

#ifndef _HEAVY_SIGNAL_VAR_H_
#define _HEAVY_SIGNAL_VAR_H_

#include "HvBase.h"

// __var~f
// __varset~f

typedef struct SignalVarf {
  hv_bufferf_t v;
} SignalVarf;

hv_size_t sVarf_init(SignalVarf *o, float k, float step, bool reverse);

static inline void __hv_var_f(SignalVarf *o, hv_bOutf_t bOut) {
  *bOut = o->v;
}

static inline void sVarsetf_process(SignalVarf *o, hv_bInf_t bIn) {
  o->v = bIn;
}

void sVarf_onMessage(HvBase *_c, SignalVarf *o, const HvMessage *m);



// __var~i
// __varset~i

typedef struct SignalVari {
  hv_bufferi_t v;
} SignalVari;

hv_size_t sVari_init(SignalVari *o, int k, int step, bool reverse);

static inline void __hv_var_i(SignalVari *o, hv_bOuti_t bOut) {
  *bOut = o->v;
}

#if HV_SIMD_AVX
#define __hv_var_k_i_0(_z,_a,_b,_c,_d,_e,_f,_g,_h) *_z=((__m256i)(__v8si) {_a,_b,_c,_d,_e,_f,_g,_h})
#define __hv_var_k_i_1(_z,_a,_b,_c,_d,_e,_f,_g,_h) *_z=((__m256i)(__v8si) {_h,_g,_f,_e,_d,_c,_b,_a})
#define __hv_var_k_f_0(_z,_a,_b,_c,_d,_e,_f,_g,_h) *_z=((__m256) {_a,_b,_c,_d,_e,_f,_g,_h})
#define __hv_var_k_f_1(_z,_a,_b,_c,_d,_e,_f,_g,_h) *_z=((__m256) {_h,_g,_f,_e,_d,_c,_b,_a})
#elif HV_SIMD_SSE
#define __hv_var_k_i_0(_z,_a,_b,_c,_d,_e,_f,_g,_h) *_z=((__m128i)(__v4si) {_a,_b,_c,_d})
#define __hv_var_k_i_1(_z,_a,_b,_c,_d,_e,_f,_g,_h) *_z=((__m128i)(__v4si) {_d,_c,_b,_a})
#define __hv_var_k_f_0(_z,_a,_b,_c,_d,_e,_f,_g,_h) *_z=((__m128) {_a,_b,_c,_d})
#define __hv_var_k_f_1(_z,_a,_b,_c,_d,_e,_f,_g,_h) *_z=((__m128) {_d,_c,_b,_a})
#elif HV_SIMD_NEON
#define __hv_var_k_i_0(_z,_a,_b,_c,_d,_e,_f,_g,_h) *_z=((int32x4_t) {_a,_b,_c,_d})
#define __hv_var_k_i_1(_z,_a,_b,_c,_d,_e,_f,_g,_h) *_z=((int32x4_t) {_d,_c,_b,_a})
#define __hv_var_k_f_0(_z,_a,_b,_c,_d,_e,_f,_g,_h) *_z=((float32x4_t) {_a,_b,_c,_d})
#define __hv_var_k_f_1(_z,_a,_b,_c,_d,_e,_f,_g,_h) *_z=((float32x4_t) {_d,_c,_b,_a})
#else // HV_SIMD_NONE
#define __hv_var_k_i_0(_z,_a,_b,_c,_d,_e,_f,_g,_h) *_z=_a
#define __hv_var_k_i_1(_z,_a,_b,_c,_d,_e,_f,_g,_h) *_z=_a
#define __hv_var_k_f_0(_z,_a,_b,_c,_d,_e,_f,_g,_h) *_z=_a
#define __hv_var_k_f_1(_z,_a,_b,_c,_d,_e,_f,_g,_h) *_z=_a
#endif
// r == 0: forwards, r == 1: backwards
#define __hv_var_k_i(_z,_a,_b,_c,_d,_e,_f,_g,_h,_r) __hv_var_k_i_##_r(_z,_a,_b,_c,_d,_e,_f,_g,_h)
#define __hv_var_k_f(_z,_a,_b,_c,_d,_e,_f,_g,_h,_r) __hv_var_k_f_##_r(_z,_a,_b,_c,_d,_e,_f,_g,_h)

static inline void sVarseti_process(SignalVari *o, hv_bIni_t bIn) {
  o->v = bIn;
}

void sVari_onMessage(HvBase *_c, SignalVari *o, const HvMessage *m);

#endif // _HEAVY_SIGNAL_VAR_H_
