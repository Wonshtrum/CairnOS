#include "bits.h"


#define EXCHANGE(X, N, P) \
| ((X & (255ULL << (8*(P-1)))) << 8*(N-2*P+1))\
| ((X & (255ULL << (8*(N-P)))) >> 8*(N-2*P+1))

#define REVERSE_64(X, N) EXCHANGE(X, N/8, 4) REVERSE_48(X, N)
#define REVERSE_48(X, N) EXCHANGE(X, N/8, 3) REVERSE_32(X, N)
#define REVERSE_32(X, N) EXCHANGE(X, N/8, 2) REVERSE_16(X, N)
#define REVERSE_16(X, N) EXCHANGE(X, N/8, 1)

#define _REVERSE(T, N) T reverse_endian(T x) {\
	return 0 REVERSE_ ## N (x, N);\
}
#define REVERSE(N) _REVERSE(int ## N ## _t, N) _REVERSE(uint ## N ## _t, N)


REVERSE(64);
REVERSE(32);
REVERSE(16);
