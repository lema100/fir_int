#include "fir_int.h"

void fir_int_init(fir_int_ctx_t *ctx, double *coeff_f, int32_t *coeff_i, uint32_t taps, int16_t *out, uint32_t coeff_depth)
{
	ctx->step = 0;
	ctx->taps = taps;
	ctx->coeff = coeff_i;
	ctx->out = out;
	ctx->coeff_depth = coeff_depth;
	for (uint32_t i = 0; i < ctx->taps; i++)
		ctx->coeff[i] = coeff_f[i] * (1 << ctx->coeff_depth);
}

void fir_int_calc(fir_int_ctx_t *ctx, int16_t *in, uint32_t len)
{
	ctx->step = 0;
	while (len--)
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
	ctx->out[ctx->step] = tmp >> ctx->coeff_depth;
	ctx->step++;
}
