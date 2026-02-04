#ifndef MPFI_AGM_H
#define MPFI_AGM_H

#include <mpfi.h>

static void mpfi_agm_step(mpfi_ptr res_a, mpfi_ptr res_g, mpfi_srcptr op1, mpfi_srcptr op2)
{
    mpfi_t tmp_a, tmp_g;

    mpfi_init2(tmp_a, mpfi_get_prec(res_a));
    mpfi_init2(tmp_g, mpfi_get_prec(res_g));

    mpfi_add(tmp_a, op1, op2); mpfi_div_ui(res_a, tmp_a, 2UL);

    mpfi_mul(tmp_g, op1, op2); mpfi_sqrt(res_g, tmp_g);

    mpfi_clear(tmp_a);
    mpfi_clear(tmp_g);
}

#endif /* MPFI_AGM_H */
