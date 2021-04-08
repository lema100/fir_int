#include "fir_int.h"

void fir_int_init(fir_int_ctx_t *ctx, double *coeff_f, int32_t *coeff_i, uint32_t taps)
{
	ctx->step = 0;
	ctx->len = 0;
	ctx->taps = taps;
	ctx->coeff = coeff_i;
	for (uint32_t i = 0; i < ctx->taps; i++)
		ctx->coeff[i] = coeff_f[i] * 0xFFFF;
}

void fir_int_calc(fir_int_ctx_t *ctx, int16_t *in)
{
	ctx->step = 0;
	for (uint32_t i = 0; i < ctx->len; i++)
		fir_int_bystep_calc(ctx, in);
}

inline void fir_int_bystep_calc(fir_int_ctx_t *ctx, int16_t *in)
{
	int32_t tmp = 0;
	for (uint32_t j = 0; j < ctx->taps; j++)
	{
		if (ctx->step >= j)
			tmp += in[ctx->step - j] * ctx->coeff[j];
	}
	ctx->out[ctx->step] = tmp >> 16;
	ctx->step++;
}
