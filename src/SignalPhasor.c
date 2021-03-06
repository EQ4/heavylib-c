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

#include "SignalPhasor.h"

// input phase is in the range of [0,1]. It is independent of o->phase.
#if HV_SIMD_AVX
static void sPhasor_updatePhase(SignalPhasor *o, float p) {
  o->phase = _mm256_set_ps(
      p+1.0f+7.0f*o->step.f2sc, p+1.0f+6.0f*o->step.f2sc,
      p+1.0f+5.0f*o->step.f2sc, p+1.0f+4.0f*o->step.f2sc,
      p+1.0f+3.0f*o->step.f2sc, p+1.0f+2.0f*o->step.f2sc,
      p+1.0f+o->step.f2sc,      p+1.0f);

  // ensure that o->phase is still in range [1,2]
  o->phase = _mm256_or_ps(_mm256_andnot_ps(
      _mm256_set1_ps(-INFINITY), o->phase), _mm256_set1_ps(1.0f));
#elif HV_SIMD_SSE
static void sPhasor_updatePhase(SignalPhasor *o, hv_uint32_t p) {
  o->phase = _mm_set_epi32(3*o->step.s+p, 2*o->step.s+p, o->step.s+p, p);
#elif HV_SIMD_NEON
static void sPhasor_updatePhase(SignalPhasor *o, hv_uint32_t p) {
  o->phase = (uint32x4_t) {p, o->step.s+p, 2*o->step.s+p, 3*o->step.s+p};
#else // HV_SIMD_NONE
static void sPhasor_updatePhase(SignalPhasor *o, hv_uint32_t p) {
  o->phase = p;
#endif
}

static void sPhasor_updateFrequency(SignalPhasor *o, float f, double r) {
#if HV_SIMD_AVX
  o->step.f2sc = (float) (f/r);
  o->inc = _mm256_set1_ps((float) (8.0f*f/r));
  sPhasor_updatePhase(o, o->phase[0]);
#elif HV_SIMD_SSE
  o->step.s = (hv_int32_t) (f*(4294967296.0/r));
  o->inc = _mm_set1_epi32(4*o->step.s);
  sPhasor_updatePhase(o, (hv_uint32_t) (o->phase[0] & 0xFFFFFFFFL));
#elif HV_SIMD_NEON
  o->step.s = (hv_int32_t) (f*(4294967296.0/r));
  o->inc = vdupq_n_s32(4*o->step.s);
  sPhasor_updatePhase(o, vgetq_lane_u32(o->phase, 0));
#else // HV_SIMD_NONE
  o->step.s = (hv_int32_t) (f*(4294967296.0/r));
  o->inc = o->step.s;
  // no need to update phase
#endif
}

hv_size_t sPhasor_init(SignalPhasor *o, double samplerate) {
#if HV_SIMD_AVX
  o->phase = _mm256_set1_ps(1.0f);
  o->inc = _mm256_setzero_ps();
  o->step.f2sc = (float) (1.0/samplerate);
#elif HV_SIMD_SSE
  o->phase = _mm_setzero_si128();
  o->inc = _mm_setzero_si128();
  o->step.f2sc = (float) (4294967296.0/samplerate);
#elif HV_SIMD_NEON
  o->phase = vdupq_n_u32(0);
  o->inc = vdupq_n_s32(0);
  o->step.f2sc = (float) (4294967296.0/samplerate);
#else // HV_SIMD_NONE
  o->phase = 0;
  o->inc = 0;
  o->step.f2sc = (float) (4294967296.0/samplerate);
#endif
  return 0;
}

void sPhasor_onMessage(HvBase *_c, SignalPhasor *o, int letIn, const HvMessage *m) {
  if (letIn == 1) {
    if (msg_isFloat(m,0)) {
      float phase = msg_getFloat(m,0);
      while (phase < 0.0f) phase += 1.0f; // wrap phase to [0,1]
      while (phase > 1.0f) phase -= 1.0f;
#if HV_SIMD_AVX
      sPhasor_updatePhase(o, phase);
#else // HV_SIMD_SSE || HV_SIMD_NEON || HV_SIMD_NONE
      sPhasor_updatePhase(o, (hv_int32_t) (phase * 4294967296.0));
#endif
    }
  }
}

hv_size_t sPhasor_k_init(SignalPhasor *o, float frequency, double samplerate) {
  sPhasor_updateFrequency(o, frequency, samplerate);
  sPhasor_updatePhase(o, 0);
  return 0;
}

void sPhasor_k_onMessage(HvBase *_c, SignalPhasor *o, int letIn, const HvMessage *m) {
  if (msg_isFloat(m,0)) {
    switch (letIn) {
      case 0: sPhasor_updateFrequency(o, msg_getFloat(m,0), ctx_getSampleRate(_c)); break;
      case 1: {
        float phase = msg_getFloat(m,0);
        while (phase < 0.0f) phase += 1.0f; // wrap phase to [0,1]
        while (phase > 1.0f) phase -= 1.0f;
#if HV_SIMD_AVX
        sPhasor_updatePhase(o, phase);
#else // HV_SIMD_SSE || HV_SIMD_NEON || HV_SIMD_NONE
        sPhasor_updatePhase(o, (hv_uint32_t) (phase * 4294967296.0));
#endif
        break;
      }
      default: break;
    }
  }
}
