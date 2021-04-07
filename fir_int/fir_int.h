#ifndef _FIR_INT_H_
#define _FIR_INT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

typedef struct
{
	uint32_t taps;
	uint32_t len;
	int32_t *coeff;
	int16_t *out;
} fir_int_ctx_t;

void fir_int_init(fir_int_ctx_t *ctx, double *coeff);
void fir_int_calc(fir_int_ctx_t *ctx, int16_t *in);

#ifdef __cplusplus
}
#endif

#endif
