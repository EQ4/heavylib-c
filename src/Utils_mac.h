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

#ifndef _HEAVY_UTILS_MAC_H_
#define _HEAVY_UTILS_MAC_H_

#include "Utils.h"

#if HV_APPLE
#include <alloca.h>
#if HV_SIMD_AVX || HV_SIMD_SSE
#include <immintrin.h>
#include <mm_malloc.h>
#elif HV_SIMD_NEON
#include <arm_neon.h>
#endif
#include <assert.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HV_EXPORT
#define HV_FORCE_INLINE inline __attribute__((always_inline))

// Memory management
#define hv_alloca(_n) alloca(_n)
#if HV_SIMD_AVX || HV_SIMD_SSE
  #define hv_malloc(_n) _mm_malloc(_n, 32) // 32 to ensure AVX compatability (16 otherwise)
  #define hv_free(x) _mm_free(x)
#else
  #define hv_malloc(_n) malloc(_n)
  #define hv_free(x) free(x)
#endif
#define hv_realloc(a, b) realloc(a, b)
#define hv_memcpy(a, b, c) memcpy(a, b, c)
#define hv_memset(a, b) memset(a, 0, b)

// Strings
#define hv_strlen(a) strlen(a)
#define hv_strncat(a, b, c) strncat(a, b, c)
#define hv_strncpy(a, b, c) strncpy(a, b, c)
#define hv_strcmp(a, b) strcmp(a, b)
#define hv_strncmp(a, b, c) strncmp(a, b, c)
#define hv_snprintf(a, b, c, ...) snprintf(a, b, c, __VA_ARGS__)

// Math
#define hv_max_f(a, b) fmaxf(a, b)
#define hv_min_f(a, b) fminf(a, b)
#define hv_max_d(a, b) fmax(a, b)
#define hv_min_d(a, b) fmin(a, b)
#define hv_sin_f(a) sinf(a)
#define hv_sinh_f(a) sinhf(a)
#define hv_cos_f(a) cosf(a)
#define hv_cosh_f(a) coshf(a)
#define hv_tan_f(a) tanf(a)
#define hv_tanh_f(a) tanhf(a)
#define hv_asin_f(a) asinf(a)
#define hv_asinh_f(a) asinhf(a)
#define hv_acos_f(a) acosf(a)
#define hv_acosh_f(a) acoshf(a)
#define hv_atan_f(a) atanf(a)
#define hv_atanh_f(a) atanhf(a)
#define hv_atan2_f(a, b) atan2f(a, b)
#define hv_exp_f(a) expf(a)
#define hv_abs_f(a) fabsf(a)
#define hv_sqrt_f(a) sqrtf(a)
#define hv_log_f(a) logf(a)
#define hv_log2_f(a) log2f(a)
#define hv_log10_f(a) log10f(a)
#define hv_ceil_f(a) ceilf(a)
#define hv_floor_f(a) floorf(a)
#define hv_round_f(a) roundf(a)
#define hv_pow_f(a, b) powf(a, b)
#define hv_fma_f(a, b, c) ((a*b)+c) // TODO(joe): use 'fmaf(a, b, c)' once emscripten supports it

// Utilities
#define hv_assert(e) assert(e)
#define hv_clear_buffer(b, n) memset(b, 0, (n)*sizeof(float))

#endif // HV_APPLE
#endif // _HEAVY_UTILS_MAC_H_
