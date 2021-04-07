#include "fir_int.h"

void fir_int_init(fir_int_ctx_t *ctx, double *coeff_f)
{
	for (uint32_t i = 0; i < ctx->taps; i++)
		ctx->coeff[i] = coeff_f[i] * 0xFFFF;
}

void fir_int_calc(fir_int_ctx_t *ctx, int16_t *in)
{
	for (uint32_t i = 0; i < ctx->len; i++)
	{
		int32_t tmp = 0;
		for (uint32_t j = 0; j < ctx->taps; j++)
		{
			if (i >= j)
				tmp += in[i - j] * ctx->coeff[j];
			else
				tmp += in[ctx->len - (j - i) - 1] * ctx->coeff[j];
		}
		ctx->out[i] = tmp >> 16;
	}
}
