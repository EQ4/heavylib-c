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

#include "Utils_windows.h"

#if HV_WIN

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
#if HV_MSVC
  // finds ceil(log2(x))
  // http://stackoverflow.com/questions/2589096/find-most-significant-bit-left-most-that-is-set-in-a-bit-array
  // http://msdn.microsoft.com/en-us/library/fbxyd7zd%28v=VS.80%29.aspx
  unsigned long z = 0;
  _BitScanReverse(&z, x);
  return (hv_uint32_t) (z+1);
#else
  return (hv_uint32_t) ((8 * sizeof(unsigned int)) - __builtin_clz(x-1));
#endif // HV_MSVC
}

#if HV_MSVC
int hv_snprintf(char* str, hv_size_t size, const char* format, ...) {
  // http://stackoverflow.com/questions/2915672/snprintf-and-visual-studio-2010
	int count = -1;
	va_list ap;
	va_start(ap, format);

	if (size != 0) {
		count = _vsnprintf_s(str, size, _TRUNCATE, format, ap);
	}
	if (count == -1) {
		count = _vscprintf(format, ap);
	}
	va_end(ap);
	return count;
}
#endif // HV_MSVC
#endif // HV_WIN

